/* 
 * File:   usart.h
 * Author: julio
 *
 * Created on February 7, 2015, 9:55 AM
 */

#ifndef USART_H
#define	USART_H

#include "Globals.h"

/**
  Section: Macro Declarations
 */
#define EUSART_TX_BUFFER_SIZE 16
#define EUSART_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
 */

uint8_t eusartTxHead;
uint8_t eusartTxTail;
uint8_t eusartTxBuffer[EUSART_TX_BUFFER_SIZE];
uint8_t eusartTxBufferRemaining;

uint8_t eusartRxHead;
uint8_t eusartRxTail;
uint8_t eusartRxBuffer[EUSART_RX_BUFFER_SIZE];
uint8_t eusartRxCount;

void init_eusart();
uint8_t EUSART_Read_1Byte(void);
uint16_t EUSART_Read_2Bytes(void);
void EUSART_Write_1Byte(uint8_t);
void EUSART_Write_2Bytes(word);
void EUSART_WriteCString(const char*);
void EUSART_WriteString(char*);
void EUSART_PurgeBuffer(void);
void EUSART_Receive_ISR(void);
void EUSART_Transmit_ISR(void);
#endif	/* USART_H */

