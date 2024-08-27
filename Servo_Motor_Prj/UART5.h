/*
 * UART5.h
 *
 *  Created on: Aug 25, 2024
 *      Author: Administrator
 */

#ifndef UART5_H_
#define UART5_H_

#include "tm4c123gh6pm.h"

/*****************************************************************************
UART5 Masks
*****************************************************************************/
#define UART5_DATA_5BITS          0x0
#define UART5_DATA_6BITS          0x1
#define UART5_DATA_7BITS          0x2
#define UART5_DATA_8BITS          0x3
#define UART5_LCRH_WLEN_BITS_POS  5
#define UART5_CTL_UARTEN_MASK     0x00000001
#define UART5_CTL_TXE_MASK        0x00000100
#define UART5_CTL_RXE_MASK        0x00000200
#define UART5_FR_TXFE_MASK        0x00000080
#define UART5_FR_RXFE_MASK        0x00000010

void HC05_Init(void);
void Leds_Init(void);
void UART5_SendByte(uint8 data);
uint8 UART5_ReceiveByte(void);
void UART5_ReceiveString(uint8 *pData);
void UART5_SendString(const uint8 *pData);
void UART5FlushReceiver(void);
void UART5FlushTransmitter(void);

#endif /* UART5_H_ */
