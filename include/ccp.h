/* 
 * File:   ccp.h
 * Author: julio
 *
 * Created on March 10, 2016, 1:05 PM
 */

#ifndef CCP_H
#define	CCP_H

#include "Globals.h"

void init_ccp(void);
word ccp_module_read(const byte);
void CCP1_ISR(void);
void CCP2_ISR(void);

#endif	/* CCP_H */

