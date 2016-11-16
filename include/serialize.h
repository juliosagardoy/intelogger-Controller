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

/**
 *
 */
typedef struct {
    byte datasize; // Controls current vector size
    byte data[32]; // Array itself, max 32
} bufctl_t;

typedef bufctl_t vector;

/**
 * 
 */
typedef struct {
    byte datatype; // Control		[ 8 bits]
    word datasize; // Data size in bits	[16 bits]
    char data[16]; // Data			[8n bits]
} SerialMessage_t;

typedef SerialMessage_t serial_message;

vector BufCtl;
serial_message RxMsg;

void init_serializer(void);
void serialize_byte_(const byte, vector *);
void serialize_word_(const word, vector *);
void transmit_sentence(byte start, byte datatype, byte datasize, const byte *data);

#endif	/* SERIALIZE_H */