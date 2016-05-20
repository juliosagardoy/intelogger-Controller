/**
 * ccp.c
 *
 */
#include "ccp.h"

static struct currval_t{
	word ccp1, ccp2;
};
/*
 * Initialites MCU to enable CCP and associated TMR1 resources
 */
void init_ccp()
{
	T1CONbits.TMR1CS = 0x02; // Timer1 external clock on T1CKI@360kHz
	T1CONbits.T1CKPS = 0x00; // Prescale 1:1 -> Tinc=2.78us
	CCP1CON = 0x05; // CKP CCP every falling edge
	CCP2CON = 0x05; // VSS CCP every falling edge
	CCP3CON = 0X05; // FFM CCP " "
//	CCP4CON = 0x05; // FFR CCP " "
	T1CONbits.TMR1ON = 1;
}

/*
 * Returns current CCPRxH and CCPRxL registers
 */
void ccp_read(word* rpm, word *vss)
{
	struct currval_t currval;
	
	*rpm = currval.ccp1 - *rpm;
	*vss = currval.ccp2 - *vss;
}

void CCP1_ISR()
{
	struct currval_t currval;
	currval.ccp1 |= CCPR1H << 8;
	currval.ccp1 |= CCPR1L;
}

void CCP2_ISR()
{
	struct currval_t currval;
	currval.ccp2 |= CCPR2H << 8;
	currval.ccp2 |= CCPR2L;
}

void CCP3_ISR()
{

}
/*
void CCP4_ISR()
{
	struct currval_t currval;
	currval.ccp4 |= CCPR4H << 8;
	currval.ccp4 |= CCPR4L;
}
 */