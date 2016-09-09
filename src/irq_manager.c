#include "irq_manager.h"
#include "usart.h"
#include "ccp.h"
#include "timer.h"

void interrupt INTERRUPT_InterruptManager(void)
{
	// interrupt handler
	if (PIE1bits.RCIE == 1 && PIR1bits.RCIF == 1) {
		EUSART_Receive_ISR();
	} else if (PIE1bits.TXIE == 1 && PIR1bits.TXIF == 1) {
		EUSART_Transmit_ISR();
	} else if (PIE1bits.TMR1IE == 1 && PIR1bits.TMR1IF == 1) {
		PIR1bits.TMR1IF = 0;
	} else if (PIE1bits.CCP1IE == 1 && PIR1bits.CCP1IF == 1) {
		CCP1_ISR();
	} else if (PIE2bits.CCP2IE == 1 && PIR2bits.CCP2IF == 1) {
		CCP2_ISR();
	} else if (PIE1bits.TMR2IE == 1 && PIR1bits.TMR2IF == 1) {
		TMR2_ISR();
	} else if (PIE3bits.TMR6IE == 1 && PIR3bits.TMR6IF == 1) {
		TMR6_ISR();
	} else {
		//Unhandled Interrupt
	}
}
