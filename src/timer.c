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
    tmr2_ovf = 0x00;
    __125ms_tick = 2;
    __125ms_flag = 0;
    __1s_tick = 16;
    /* Tcy=250ns * 64 * 16 = 256us */
    T2CONbits.T2CKPS = 0x03; // Prescaler @ 1:16
    T2CONbits.T2OUTPS = 0x0F; // Postscaler @ 1:10
    TMR2 = 6; // Preload at 6, irq ea. 250*250ns*16*10 = 10ms
    tmr6_start();
}

/**
 * 
 */
void
tmr6_start() {
    T6CONbits.TMR2ON = 1;
}

/**
 * 
 */
void
tmr6_stop() {
    T6CONbits.TMR2ON = 0;
}

/*
 * Init timer6 for program execution pace
 * DO NOT USE
 */
void
init_tmr6() {
    tmr6_ovf_count = 0x00;
    __125ms_flag = 0;
    /* TMR6cy=250ns * 64 * 16 = 256us */
    T6CONbits.T6CKPS = 0b11; // Prescaler @ 1:64
    T6CONbits.T6OUTPS = 0b1111; // Postscaler @ 1:16
    TMR6 = 6; // Preload at 6 to count 250 pulses, then irq=(2^8-6)*250us = 62.5ms
    tmr6_start();
}

void
tmr6_reset() {
    TMR6 = 6;
    tmr6_ovf_count = 0x00;
}

void TMR2_ISR() {
    /* 62.5ms * 2 = 125ms */
    if (__125ms_tick > 0)
        __125ms_tick--;
    else
    {
        __125ms_tick = 2;
        __125ms_flag = 1;
    }
    /* 62.5ms * 16 = 1s */
    if (__1s_tick < 16)
        __1s_tick++;
    else if(__1s_tick = 16)
        uptime_s++;
    else
        __1s_tick = 0;
}

/**
 * Interruption of Timer6 happens every 62.5ms
 */
void TMR6_ISR() {
    /* Will increase uptime_s if 62.5ms*16=1s passed
     * if not, it will increase ovf count*/
    if (tmr6_ovf_count >= 16) {
        __1s_tick++;
        tmr6_reset();
    } else
        tmr6_ovf_count++;
}