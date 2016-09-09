 /* 
 * File:   Globals.h
 * Author: julio
 *
 * Created on February 7, 2015, 9:32 AM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

/*--------------------------------------------------------------------------------------------------
                                          Constants
--------------------------------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

#define FALSE                   0
#define TRUE                    (!FALSE)

#define bit_is_set(port,pin)     ((port) & (1<<(pin)))
#define bit_is_clear(port,pin)   ~((port) & (1<<(pin)))

#define sbi(port, bit)           (port) |= (1 << (bit))
#define cbi(port, bit)           (port) &= ~(1 << (bit))

#define _BV(bit)                 (1 << (bit))

#define toggle(b)                b^=0x01

#define INPUT_IS_SET             ( bit_is_set( DATAIN_PIN, DATAIN ) )
#define INPUT_IS_CLEAR           ( bit_is_clear( DATAIN_PIN, DATAIN ) )

#define _HOST_TIMEOUT      20 // Timeout given to host before admiting no comm

#define DISCRETE_SIGS_QTY   4
#define GPIO_VTS            PORTAbits.RA7
#define GPIO_PCS            PORTBbits.RB1
#define GPIO_ALT            PORTCbits.RC5
#define GPIO_FAN            PORTAbits.RA6

#define ANALOG_SIGS_QTY     9
#define AN_TPS_CHAN         0x0C    // Throttle Position tied to AN12,RB0
#define AN_ECT_CHAN         0x00    // Engine Coolant Temp tied to AN0,RA0
#define AN_MAP_CHAN         0x0B    // Manifold Air Pressure tied to AN11,RB4
#define AN_HO2_CHAN         0x01    // Heated Oxygen Sensor tied to AN1,RA1
#define AN_IAT_CHAN         0x04    // Intake Air Temp tied to AN4,RA5
#define AN_FLV_CHAN         0x08    // Fuel Level tied to AN8,RB2
#define AN_EXT_CHAN         0x0D    // Ext temp sensor tied to AN13,RB5
#define AN_BAT_CHAN         0x0A    // Battery voltage tied to AN9,RB3
#define AN_TIM_CHAN         0x1D    // PIC Internal Temperature

#define CCP_SIGS_QTY        2

#define INTERRUPT_GlobalInterruptDisable() (INTCONbits.GIE = 0)
#define INTERRUPT_GlobalInterruptEnable() (INTCONbits.GIE = 1)
#define INTERRUPT_PeripheralInterruptEnable() (INTCONbits.PEIE = 1)
#define INTERRUPT_PeripheralInterruptDisable() (INTCONbits.PEIE = 0)

#include <xc.h>
#include <stdint.h>        /* For uintX_t definition */
#include <string.h>
//#include <stdlib.h>
//#include <stddef.h>

/*--------------------------------------------------------------------------------------------------
                                       Type definitions
--------------------------------------------------------------------------------------------------*/
typedef uint8_t     byte;
typedef uint16_t    word;

/*--------------------------------------------------------------------------------------------------
                                         Prototypes
--------------------------------------------------------------------------------------------------*/
//inline void activityIndicatorOff( void );
//inline void activityIndicatorOn( void );     //Indicates that IEBus packets are being processed
//inline void usartIndicatorOff(void);
//inline void usartIndicatorOn(void);            //Indicates that spi communication is being used

#endif	/* GLOBALS_H */

