/* 
 * File:   irq_manager.h
 * Author: julio
 *
 * Created on February 7, 2015, 10:02 AM
 */

#ifndef IRQ_MANAGER_H
#define	IRQ_MANAGER_H

#include "Globals.h"

#define INTERRUPT_GlobalInterruptEnable() (INTCONbits.GIE = 1)
#define INTERRUPT_GlobalInterruptDisable() (INTCONbits.GIE = 0)
#define INTERRUPT_PeripheralInterruptEnable() (INTCONbits.PEIE = 1)
#define INTERRUPT_PeripheralInterruptDisable() (INTCONbits.PEIE = 0)

void interrupt INTERRUPT_InterruptManager( void );

#endif	/* IRQ_MANAGER_H */

