/*
 * File:   main.c
 * Author: julio
 *
 * Created on February 7, 2015, 9:29 AM
 */

#include "configBytes.h"
#include "irq_manager.h"
#include "usart.h"
#include "adc.h"
#include "ccp.h"
#include "timer.h"
#include "serialize.h"
#include "data.h"

/* Variables that control program status */

/**
 * __pc_active will be 1 as long as PC sends alive signal.
 * It will be 0 if PC takes more than 5s to report alive.
 */
bit __pc_active = 0; // 1 if serialport with PC is up

/**
 * Counter for PC alive
 */
byte pc_timeout = 0;

static void
init_osc() {
    OSCCON = 0x68; /* 01101000 PLL OFF,4MHz INTOSC, OSC defined in ConfBytes
    * OSTS intosc; HFIOFR disabled; HFIOFS not0.5percent_acc; PLLR disabled;
    * T1OSCR disabled; MFIOFR disabled; HFIOFL not2percent_acc; LFIOFR disabled;
    * TUN 0x0;
    */
    OSCTUNE = 0x00;
}

static void
init_mcu() {
    /* EUSART pins */
    TRISCbits.TRISC7 = 1; // RX
    TRISCbits.TRISC6 = 0; // TX

    /* GPIO ports direction config */
    TRISAbits.TRISA7 = 1; // VTS
//    TRISBbits.TRISB1 = 1; // PCS
    ANSELBbits.ANSB1 = 0; // Set pin as GPIO
//    TRISCbits.TRISC5 = 1; // ALT
//    TRISAbits.TRISA6 = 1; // FANC

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
//    TRISCbits.TRISC1 = 1; // VSS CCP2

    /* PWM */
    TRISAbits.TRISA4 = 0; // BEEP output CCP5

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
            }
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


#ifdef DEBUG
        __pc_active = 1;
#endif
        /* If communication with PC is established, serialize&send data */
        if (__pc_active) {
            /* Serialize IC status, datatype 0x33 */
            BufCtl.datasize = 0x00; // Purge serializer buffer&BufCtl.datasize);
//            serialize_word_(uptime_s, &BufCtl);   // Not used anymore
            serialize_word_(AnalogSigs.tin, &BufCtl);
            serialize_word_(AnalogSigs.ext, &BufCtl);

            transmit_sentence(0x01, 0x33, BufCtl.datasize, BufCtl.data);

            /* Serialize DiscreteSigs, datatype 0x30 */
            BufCtl.datasize = 0x00; // Purge serializer buffer&BufCtl.datasize);
            serialize_byte_(DiscreteSigs.vts ? '1' : '0', &BufCtl);
            serialize_byte_(DiscreteSigs.other1 ? '1' : '0', &BufCtl);
            serialize_byte_(DiscreteSigs.other2 ? '1' : '0', &BufCtl);
            serialize_byte_(DiscreteSigs.other3 ? '1' : '0', &BufCtl);

            transmit_sentence(0x01, 0x30, BufCtl.datasize, BufCtl.data);

            /* Serialize AnalogSigs, datatype 0x31 */
            BufCtl.datasize = 0x00; // Purge serializer buffer&BufCtl.datasize);
            serialize_word_(AnalogSigs.tps, &BufCtl);
            serialize_word_(AnalogSigs.ect, &BufCtl);
            serialize_word_(AnalogSigs.map, &BufCtl);
            serialize_word_(AnalogSigs.ho2, &BufCtl);
            serialize_word_(AnalogSigs.iat, &BufCtl);
            serialize_word_(AnalogSigs.fle, &BufCtl);
            serialize_word_(AnalogSigs.bat, &BufCtl);

            transmit_sentence(0x01, 0x31, BufCtl.datasize, BufCtl.data);

            /* Serialize CcpSigs, datatype 0x32 */
            BufCtl.datasize = 0x00; // Purge serializer buffer&BufCtl.datasize);
            serialize_word_(CcpSigs.rpm, &BufCtl);
            serialize_word_(CcpSigs.other1, &BufCtl);

            transmit_sentence(0x01, 0x32, BufCtl.datasize, BufCtl.data);
        }

        //__delay_ms(PACE_MS);
        /* Wait for tmr6_ovf_count to count 32=2s/62.5ms */
        while (!__200ms_flag);
        __200ms_flag = 0;
        
        //tmr6_reset();
    }
}

/**
 * Main
 * @param argc
 * @param argv
 * @return
 */
void
main() {
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
    tmr6_stop();
    INTERRUPT_PeripheralInterruptDisable();
    INTERRUPT_GlobalInterruptDisable();
}
