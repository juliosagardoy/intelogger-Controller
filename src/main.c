l/* 
 * File:   main.c
 * Author: julio
 *
 * Created on February 7, 2015, 9:29 AM
 */


#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "configBytes.h"
#include "irq_manager.h"
#include "usart.h"
#include "adc.h"
#include "ccp.h"
#include "timer.h"
#include "serialize.h"
#include "data.h"

/* Variables that control program status */
//    discrete_sigs DiscreteSigs = 0x00;
byte __pc_active = 0; // 1 if serialport with PC is up
byte pc_timeout = 0;

static void
init_osc() {
    OSCCON = 0x68; // PLL OFF,4MHz INTOSC, OSC defined in Confbytes
    // OSTS intosc; HFIOFR disabled; HFIOFS not0.5percent_acc; PLLR disabled; T1OSCR disabled; MFIOFR disabled; HFIOFL not2percent_acc; LFIOFR disabled;
    // TUN 0x0;
    OSCTUNE = 0x00;
}

static void
init_mcu() {
    /* EUSART pins */
    TRISCbits.TRISC7 = 1; // RX
    TRISCbits.TRISC6 = 0; // TX

    /* GPIO ports direction config */
    TRISAbits.TRISA7 = 1; // VTS
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
    TRISBbits.TRISB2 = 1; // FLV
    TRISBbits.TRISB5 = 1; // EXT
    TRISBbits.TRISB3 = 1; // BAT
    FVRCONbits.TSEN = 1; // Temperature module enable
    FVRCONbits.TSRNG = 1; //Temperature module working on "high VDD range"

    /* CCP */
    TRISCbits.TRISC2 = 1; // CKP CCP1
    TRISCbits.TRISC1 = 1; // VSS CCP2

    /* PWM */
    TRISAbits.TRISA4 = 0; // BEEP output CCP5

}



/**
 * Resets BufCtl size counter, causing further operations to overwrite current BufCtl contents
 * @param pointer to datasize from BufCtl
 */
inline void
serialize_purge(byte * datasize) {
    *datasize = 0x00; // Purge serializer buffer
}

/**
 * Loop
 * @return 
 */
int loop() {
    while (1) {
        /* Program on PC side sends 0x01 before initiating a sentence
         * Check for something in the buffer */
        if (eusartRxHead > 0) {
            char lastChar;
            lastChar = EUSART_Read_1Byte();
            /* Check if it's a valid sentence start char */
            if (lastChar == 0x01) {
                EUSART_Write_1Byte(0x05); // Send ACK
                EUSART_Write_1Byte(DISCRETE_SIGS_QTY);
                EUSART_Write_1Byte(ANALOG_SIGS_QTY);
                EUSART_Write_1Byte(CCP_SIGS_QTY);
                __pc_active = 1; // PC has been identified
            }/* If timeout reached, flag off COMM */
            else if (pc_timeout == _HOST_TIMEOUT)
                __pc_active = 0;
                /* PC didn't signal, so increase timeout counter */
            else
                pc_timeout++;
        }

        /* Process inputs and current status */
        DiscreteSigs = discrete_read(); // Retrieve inputs an pack in a single byte
        AnalogSigs = analog_read();
        CcpSigs = ccp_read();

        /* Transform timer increments to rpm */
        //        ccp2rpm(&CcpSigs.rpm);
        //       ccp2rpm(&CcpSigs.vss);

#ifdef DEBUG
        __pc_active = 1;
#endif
        /* If communication with PC is established, serialize&send data */
        if (__pc_active) {
            /* Serialize IC status, datatype 0x33 */
            serialize_purge(&BufCtl.datasize);
            serialize_word_pushback(uptime_s, &BufCtl);
            serialize_word_pushback(AnalogSigs.tin, &BufCtl);
            serialize_word_pushback(AnalogSigs.ext, &BufCtl);

            transmit_sentence(0x01, 0x33, BufCtl.datasize, BufCtl.data);

            /* Serialize DiscreteSigs, datatype 0x30 */
            serialize_purge(&BufCtl.datasize);
            serialize_byte_pushback(DiscreteSigs.vts ? '1' : '0', &BufCtl);
            serialize_byte_pushback(DiscreteSigs.pcs ? '1' : '0', &BufCtl);
            serialize_byte_pushback(DiscreteSigs.alt ? '1' : '0', &BufCtl);
            serialize_byte_pushback(DiscreteSigs.fan ? '1' : '0', &BufCtl);

            transmit_sentence(0x01, 0x30, BufCtl.datasize, BufCtl.data);

            /* Serialize AnalogSigs, datatype 0x31 */
            serialize_purge(&BufCtl.datasize);
            serialize_word_pushback(AnalogSigs.tps, &BufCtl);
            serialize_word_pushback(AnalogSigs.ect, &BufCtl);
            serialize_word_pushback(AnalogSigs.map, &BufCtl);
            serialize_word_pushback(AnalogSigs.ho2, &BufCtl);
            serialize_word_pushback(AnalogSigs.iat, &BufCtl);
            serialize_word_pushback(AnalogSigs.fle, &BufCtl);
            serialize_word_pushback(AnalogSigs.bat, &BufCtl);

            transmit_sentence(0x01, 0x31, BufCtl.datasize, BufCtl.data);

            /* Serialize CcpSigs, datatype 0x32 */
            serialize_purge(&BufCtl.datasize);
            serialize_word_pushback(CcpSigs.rpm, &BufCtl);
            serialize_word_pushback(CcpSigs.vss, &BufCtl);

            transmit_sentence(0x01, 0x32, BufCtl.datasize, BufCtl.data);
        }

        //__delay_ms(PACE_MS);
        /* Wait for tmr6 tick. tmr6 configured to tick every 250 ms
         * overflow == tcy*250*16*10 = 0.01 s
         * 0.200/0.01s = 20 */
        while (tmr6_ovf < 20);
        reset_tmr6();
    }
}

/**
 * Main
 * @param argc
 * @param argv
 * @return 
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

    init_serializer();
    init_data();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();

    EUSART_PurgeBuffer();

    loop();

    adc_disable();
    return (1);
}
