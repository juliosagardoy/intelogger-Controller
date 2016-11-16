/**
 * timer.c
 *
 */

#include "timer.h"

/*
 * Init timer2 for uptime count
 */
void
init_tmr2() {
//    tmr2_ovf = 0x00;
//    __125ms_tick = 2;
//    __200ms_flag = 0;
//    __200ms_flag = 16;
//    /* Tcy=250ns * 64 * 16 = 256us */
//    T2CONbits.T2CKPS = 0x03; // Prescaler @ 1:16
//    T2CONbits.T2OUTPS = 0x0F; // Postscaler @ 1:10
//    TMR2 = 6; // Preload at 6, irq ea. 250*250ns*16*10 = 10ms
//    tmr6_start();
}

/*
 * Init timer6 for program execution pace

 */
void
init_tmr6() {
    tmr6_ovf = 0x00;
    __200ms_flag = 0x00;

    /* Configure overflow: Fcy=4MHz; Tcy=250ns * 64 * 16 = 256us per tick */
    T6CONbits.T6CKPS = 0b11; // Prescaler @ 1:64
    T6CONbits.T6OUTPS = 0b1111; // Postscaler @ 1:16
    TMR6 = 6; // Preload at 6, then irq ea. (2^8-6)*250us*16*10 = 10ms
    PIE3bits.TMR6IE = 1;
    T6CONbits.TMR6ON = 1;
}

/**
 * 
 */
void
tmr6_start() {
    T6CONbits.TMR6ON = 1;
}

/**
 * 
 */
void
tmr6_stop() {
    T6CONbits.TMR6ON = 0;
}

/**
 * 
 */
void
tmr6_reset() {
    TMR6 = 6;
    tmr6_ovf = 0x00;
}

/**
 * TMR6 is replacing TMR2, reserved for other stuff
 */
void TMR2_ISR() {
//    /* 62.5ms * 2 = 125ms */
//    if (__125ms_tick > 0)
//        __125ms_tick--;
//    else
//    {
//        __125ms_tick = 2;
//        __200ms_flag = 1;
//    }
//    /* 62.5ms * 16 = 1s */
//    if (__200ms_flag < 16)
//        __200ms_flag++;
//    else if(__200ms_flag = 16)
//        uptime_s++;
//    else
//        __200ms_flag = 0;
}

/**
 * Interruption of Timer6 happens every 10ms
 */
void TMR6_ISR() {
    /* Will increase uptime_s if 10ms*20=200ms passed
     * if not, it will increase overflow count */
    if (tmr6_ovf >= 20) {
        __200ms_flag = 1;
        tmr6_reset();
    } else
        tmr6_ovf++;
}