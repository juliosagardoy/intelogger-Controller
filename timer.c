/**
 * timer.c
 *
 */

#include "timer.h"

/*
 * Init timer2 for uptime count
 */
void
init_tmr2()
{
	tmr2_ovf = 0x00;
	/* Tcy=250ns * 64 * 16 = 256us */
	T2CONbits.T2CKPS = 0x03;	// Prescaler @ 1:16
	T2CONbits.T2OUTPS = 0x0F;	// Postscaler @ 1:10
	TMR2 = 6;			// Preload at 6, irq ea. 250*250ns*16*10 = 10ms
	T2CONbits.TMR2ON = 1;
}

/*
 * Init timer6 for program execution pace
 */
void
init_tmr6()
{
	tmr6_ovf = 0x00;
	/* Tcy=250ns * 64 * 16 = 256us */
	T6CONbits.T6CKPS = 0x03;	// Prescaler @ 1:16
	T6CONbits.T6OUTPS = 0x0F;	// Postscaler @ 1:10
	TMR6 = 6;			// Preload at 6, irq ea. 250*250ns*16*10 = 10ms
	T6CONbits.TMR6ON = 1;
}

void
reset_tmr6()
{
	TMR6 = 6;
	tmr6_ovf = 0x00;
}

void TMR2_ISR()
{
	/* oflow == tcy*250*16*10 = 0.01 s 
	 * 1/0.01s = 100
     * When tmr2_ovf = 100, we know 1 s has passed
	 */
	if(tmr2_ovf < 100)
		tmr2_ovf++;
	else
	{
		TMR2 = 6;
		tmr2_ovf = 0x00;
		uptime_s++;		
	}
}

void TMR6_ISR()
{
	tmr6_ovf++;
}