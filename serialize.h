/* 
 * File:   serialize.h
 * Author: julio
 *
 * Created on March 4, 2016, 11:04 AM
 */

#ifndef SERIALIZE_H
#define	SERIALIZE_H

#include "Globals.h"
#include "usart.h"

typedef struct {
	byte datasize;
	byte data[32];
} bufctl_t;

typedef bufctl_t bufctl;

void serialize_byte( const byte, bufctl *);
void serialize_word( const word,  bufctl *);

#endif	/* SERIALIZE_H */

