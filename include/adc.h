/* 
 * File:   adc.h
 * Author: julio
 *
 * Created on March 3, 2016, 11:59 AM
 */

#ifndef ADC_H
#define	ADC_H

#include "Globals.h"

void init_adc(void);
inline void adc_enable(void);
inline void adc_disable(void);
word adc_read(const byte);

#endif	/* ADC_H */

