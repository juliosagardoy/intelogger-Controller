/* 
 * File:   configBytes.h
 * Author: julio
 *
 * Created on January 30, 2015, 2:43 PM
 */

#ifndef CONFIGBYTES_H
#define	CONFIGBYTES_H

#pragma config IESO = OFF    // Internal/External Switchover->Internal/External Switchover mode is disabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config PWRTE = OFF    // Power-up Timer Enable->PWRT disabled
#pragma config FOSC = INTOSC    // Oscillator Selection->Internal clock
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is disabled
#pragma config MCLRE = ON    // MCLR Pin Function Select->MCLR/VPP pin function is MCLR
#pragma config CP = OFF    // Flash Program Memory Code Protection->Program memory code protection is disabled
#pragma config CPD = OFF    // Data Memory Code Protection->Data memory code protection is disabled
#pragma config WDTE = OFF    // Watchdog Timer Enable->WDT disabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config VCAPEN = OFF    // Voltage Regulator Capacitor Enable->All VCAP pin functionality is disabled
#pragma config LVP = OFF    // Low-Voltage Programming Enable->High-voltage on MCLR/VPP must be used for programming
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config PLLEN = OFF    // PLL Enable->4x PLL disabled
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#endif	/* CONFIGBYTES_H */
