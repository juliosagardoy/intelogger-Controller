/* 
 * File:   main.c
 * Author: julio
 *
 * Created on February 7, 2015, 9:29 AM
 */


#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "configBytes.h"
#include "Globals.h"
#include "irq_manager.h"
#include "usart.h"
#include "adc.h"
#include "ccp.h"
#include "serialize.h"
#include "timer.h"

typedef struct {
    byte datatype; // Control		[ 8 bits]
    word datasize; // Data size in bits	[16 bits]
    char data[16]; // Data			[8n bits]
} SerialMessage_t;

typedef SerialMessage_t serial_message;

typedef byte discrete_sigs;

typedef struct {
    word tps;
    word map;
    word ect;
    word ho2;
    word iat;
    word fle;
    word tim;
} AnalogSigs_t;

typedef AnalogSigs_t analog_sigs;

typedef struct {
    word rpm;
    word vss;
    word ffm;
    //	word ffr;
} CcpSigs_t;

typedef CcpSigs_t ccp_sigs;

static void
init_osc() {
    // SPLLEN disabled; SCS FOSC defined in Configuration; IRCF 8MHz;
    // 0b111000
    //OSCCON = 0xB8;
    OSCCONbits.IRCF = 0x0F; // INTOSC @ 16MHz
    // OSTS intosc; HFIOFR disabled; HFIOFS not0.5percent_acc; PLLR disabled; T1OSCR disabled; MFIOFR disabled; HFIOFL not2percent_acc; LFIOFR disabled;
    OSCSTAT = 0x00;
    // TUN 0x0;
    OSCTUNE = 0x00;
}

static void
init_mcu() {
    /* EUART pins */
    TRISCbits.TRISC7 = 1; // RX
    TRISCbits.TRISC6 = 0; // TX

    /* GPIO ports direction config */
    TRISAbits.TRISA7 = 1; // VTS
    ANSELAbits.ANSA4 = 0; // Set pin as GPIO
    TRISBbits.TRISB1 = 1; // PCS
    ANSELBbits.ANSB1 = 0; // Set pin as GPIO
    TRISCbits.TRISC5 = 1; // ALT
    TRISAbits.TRISA6 = 1; // FANC

    /* Analog i/o */
    TRISBbits.TRISB0 = 1; // TPS;
    TRISAbits.TRISA0 = 1; // ECT
    TRISBbits.TRISB4 = 1; // MAP
    TRISAbits.TRISA1 = 1; // HO2S
    TRISAbits.TRISA5 = 1; // IAT
    TRISBbits.TRISB2 = 1; // Fuel Level
    TRISAbits.TRISA4 = 1; // Battery
    FVRCONbits.TSEN = 1; // PIC Temperature module enable
    FVRCONbits.TSRNG = 1;

    /* CCP */
    TRISCbits.TRISC2 = 1; // CKP
    TRISCbits.TRISC1 = 1; // VSS
    TRISBbits.TRISB5 = 0; // BEEP output
    APFCONbits.CCP3SEL = 1; // CCP3 on RB5
}

/*
 * Retrieve current discrete values from ECM
 * These are all stuffed in a single byte using bit-shifting
 */
void
discrete_read(discrete_sigs *DiscreteSigs) {
    *DiscreteSigs |= GPIO_VTS;
    *DiscreteSigs |= GPIO_PCS << 1;
    *DiscreteSigs |= GPIO_ALT << 2;
    *DiscreteSigs |= GPIO_FAN << 3;
}

/*
 * Given the CCP value, converts the value to RPM
 */
inline void
ccp2rpm(word *value) {
    /* TMR inc each 2.78us & Prescaler@1:1 -> RPM = 60/2.78u*TMRval = 21600000/TMRval */
    *value = (word) round(21600000.0 / (double) (*value));
}

/*
 * Transmit the current contents of BufCtl
 */
void
transmit_BufCtl(byte start, byte datatype, byte datasize, byte * data) {
    EUSART_Write_1Byte(start); // Send sentence start char
    EUSART_Write_1Byte(datatype);
    EUSART_Write_1Byte(datasize);
    EUSART_WriteString(data);
}

/*
 * Resets BufCtl size counter, causing further operations to overwrite current BufCtl contents
 */
inline void
purge_BufCtl(byte * datasize) {
    *datasize = 0x00; // Purge serializer buffer
}

/*
 * Main program
 */
int
main(int argc, char** argv) {
    init_osc();
    init_mcu();
    init_tmr2();
    init_tmr6();
    init_eusart();
    init_adc();
    init_ccp();

    serial_message RxMsg;
    analog_sigs AnalogSigs;
    ccp_sigs CcpSigs;
    bufctl BufCtl;
    discrete_sigs DiscreteSigs = 0x00;
    byte COMM = 0;

    /* Enable global interrupts */
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    memset(&RxMsg, 0x00, sizeof (RxMsg));
    memset(&AnalogSigs, 0x00, sizeof (AnalogSigs));
    memset(&CcpSigs, 0x00, sizeof (CcpSigs));
    memset(&BufCtl, 0x00, sizeof (BufCtl));

    EUSART_PurgeBuffer();
    EUSART_WriteCString("Diagnostics for Honda OBD2\n");
    EUSART_WriteCString("v0.1-BETA\n");

    while (1) {
        //toggle(LED_RED);
        /* Check for something in the buffer */
        if (eusartRxHead > 0) {
            char lastChar;
            lastChar = EUSART_Read_1Byte();
            /* Check for valid sentence start char */
            if (lastChar == 0x01) {
                EUSART_Write_1Byte(0x05);
                COMM = 1;   // PC has been identified
            }
        }

        /* Process inputs and current status */
        discrete_read(&DiscreteSigs); // Retrieve inputs an pack in a single byte

        adc_enable();
        AnalogSigs.tps = adc_read(AN_TPS_CHAN);
        AnalogSigs.ect = adc_read(AN_ECT_CHAN);
        AnalogSigs.map = adc_read(AN_MAP_CHAN);
        AnalogSigs.ho2 = adc_read(AN_HO2_CHAN);
        AnalogSigs.iat = adc_read(AN_IAT_CHAN);
        AnalogSigs.fle = adc_read(AN_FLV_CHAN);
        AnalogSigs.tim = adc_read(AN_TIM_CHAN);
        adc_disable();

        /* Read current values stored within CCP buffers */
        ccp_read(&CcpSigs.rpm, &CcpSigs.vss);
        /* Transform timer increments to rpm */
        ccp2rpm(&CcpSigs.rpm);
        ccp2rpm(&CcpSigs.vss);

        /* If communication with PC is established, serialize&send data */
        if (COMM) {
            /* Serialize IC status, datatype 0x33 */
            purge_BufCtl(&BufCtl.datasize);
            serialize_word(uptime_s, &BufCtl);
            serialize_word(AnalogSigs.tim, &BufCtl);
            /* Transfer */
            transmit_BufCtl(0x01, 0x33, BufCtl.datasize, BufCtl.data);

            /* Serialize DiscreteSigs, datatype 0x30 */
            purge_BufCtl(&BufCtl.datasize);
            serialize_byte(DiscreteSigs, &BufCtl);
            /* Transfer */
            transmit_BufCtl(0x01, 0x30, BufCtl.datasize, BufCtl.data);

            /* Serialize AnalogSigs, datatype 0x31 */
            purge_BufCtl(&BufCtl.datasize);
            serialize_word(AnalogSigs.tps, &BufCtl);
            serialize_word(AnalogSigs.ect, &BufCtl);
            serialize_word(AnalogSigs.map, &BufCtl);
            serialize_word(AnalogSigs.ho2, &BufCtl);
            serialize_word(AnalogSigs.iat, &BufCtl);
            serialize_word(AnalogSigs.fle, &BufCtl);
            /* Transfer */
            transmit_BufCtl(0x01, 0x31, BufCtl.datasize, BufCtl.data);

            /* Serialize CcpSigs, datatype 0x32 */
            purge_BufCtl(&BufCtl.datasize);
            serialize_word(CcpSigs.rpm, &BufCtl);
            serialize_word(CcpSigs.vss, &BufCtl);

            /* Transfer */
            transmit_BufCtl(0x01, 0x32, BufCtl.datasize, BufCtl.data);
        }
        //__delay_ms(PACE_MS);
        /* Wait for tmr6 tick. tmr6 configured to tick every 250 ms
         * overflow == tcy*250*16*10 = 0.01 s
         * 0.200/0.01s = 20 */
        while (tmr6_ovf < 20);
        reset_tmr6();
    }

    adc_disable();
    return (EXIT_SUCCESS);
}
