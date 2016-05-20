/**
 * adc.c
 *
*/
#include "adc.h"

/*
 * Init ADC
 */
void init_adc()
{
	ADCON1bits.ADPREF = 0;	// Vref+ set to VDD
	ADCON1bits.ADNREF = 0;	// Vref- set to VSS
	ADCON1bits.ADCS = 0b101;	// Conv freq, Tad 1us, 10bits=10*1us=10us per pin
	ADCON1bits.ADFM = 1;	// Right-justified result
}
/*
 * Enable ADC 
 */ 
void inline adc_enable()
{
	ADCON0bits.ADON = 1;	
}

/*
 * Disable ADC
 */
void inline adc_disable()
{
	ADCON0bits.ADON = 0;
}

/*
 * Calculates and returns a conversion from given ADC channel number
 */
word
adc_read( const byte chan )
{
	word res = 0x00;
	byte sum;
	
	ADCON0bits.CHS = chan;		// Select req. channel to ADC
	
//	__delay_us(10);			// Wait the required acquisition time
	ADCON0bits.GO_nDONE = 1;	// Start conversion
	while (ADCON0bits.GO);		// Wait for conversion to end, about. 10us
	res |= ADRESH << 8;
	res |= ADRESL;
	
	return res;
}
