/* 
 * File:   timer.h
 * Author: julio
 *
 * Created on March 22, 2016, 8:25 AM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "Globals.h"

word tmr2_ovf;
word tmr6_ovf;
word uptime_s;  // Uptime counts seconds up to 65536*60*60=18h

void init_tmr2( void );
void init_tmr6( void );
void reset_tmr6( void );
void TMR2_ISR( void );
void TMR6_ISR( void );

#endif	/* TIMER_H */

