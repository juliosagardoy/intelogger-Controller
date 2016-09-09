/**
 * ccp.c
 *
 */

#include "ccp.h"

word rpm = { 0 };
word vss = { 0 };

/*
 * Initialites MCU to enable CCP and associated TMR1 resources
 */
void init_ccp()
{

	T1CONbits.TMR1CS = 0x00; // Timer1 internal osc divided by 4, Tinc=1us
	T1CONbits.T1CKPS = 0x00; // Prescale 1:1 -> Tinc=1us
	CCP1CON = 0x05; // CKP CCP every falling edge
	CCP2CON = 0x05; // VSS CCP every falling edge
	T1CONbits.TMR1ON = 1;
    
    rpm=0;
    vss=0;
}

/**
 * Will return the latest values stored at ccp(chan) register
 * @param c channel number
 * @return word with register value
 */
word ccp_module_read(const byte c)
{
    switch (c)
    {
        case 1:
            return rpm;
        case 2:
            return vss;
        default:
            return 0;
    }
}

void CCP1_ISR()
{
	rpm |= CCPR1H << 8;
	rpm |= CCPR1L;
}

void CCP2_ISR()
{
	vss |= CCPR2H << 8;
	vss |= CCPR2L;
}
