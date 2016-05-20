/* 
 * File:   ccp.h
 * Author: julio
 *
 * Created on March 10, 2016, 1:05 PM
 */

#ifndef CCP_H
#define	CCP_H

#include "Globals.h"

void init_ccp( void );
void ccp_reset_tmr();
void ccp_disable_tmr();
void ccp_read( word*,word* );
void CCP1_ISR( void );
void CCP2_ISR( void );
void CCP3_ISR( void );
//void CCP4_ISR( void );

#endif	/* CCP_H */

