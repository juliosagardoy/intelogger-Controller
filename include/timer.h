/* 
 * File:    timer.h
 * Author:  julio
 *
 * Created on March 22, 2016, 8:25 AM
 * 
 * Title:   Timer resources
 * Description: 
 *  -   Timer1: Assigned to ccp module, see cpp.h
 *  -   Timer2: Not used
 *  -   Timer3: Not used
 *  -   Timer4: Not used
 *  -   Timer5: Not used
 *  -   Timer6: Irqs every 62.5 ms
 */

#ifndef TIMER_H
#define	TIMER_H

#include "Globals.h"

byte tmr2_ovf;
byte tmr6_ovf_count;
byte __1s_tick; // Helper for uptime_s count
byte __125ms_tick; // Helper for 125 ms increments count
bit __125ms_flag;
word uptime_s;  // Uptime counts seconds up to 65536*60*60=18h

void init_tmr2(void);
void init_tmr6(void);
void tmr6_start(void);
void tmr6_stop(void);
void tmr6_reset(void);
void TMR2_ISR(void);
void TMR6_ISR(void);

#endif	/* TIMER_H */

