/*
 * UART5.c
 *
 *  Created on: Aug 25, 2024
 *      Author: Administrator
 */
#include "UART5.h"

void HC05_Init(void)
{
    SYSCTL_RCGCGPIO_R |= 0x10;
    while(!(SYSCTL_RCGCGPIO_R&0x10));

    GPIO_PORTE_AFSEL_R = 0x30;
    GPIO_PORTE_AMSEL_R = 0;
    GPIO_PORTE_PCTL_R = 0x00110000;
    GPIO_PORTE_DEN_R = 0x30;


    SYSCTL_RCGCUART_R |= 0x20;
    while(!(SYSCTL_PRUART_R & 0x20));


    UART5_CTL_R = 0;

    UART5_CC_R = 0;
    UART5_IBRD_R = 104;
    UART5_FBRD_R = 11;
    UART5_LCRH_R = (UART5_DATA_8BITS << UART5_LCRH_WLEN_BITS_POS);

    UART5_CTL_R = UART5_CTL_UARTEN_MASK | UART5_CTL_TXE_MASK | UART5_CTL_RXE_MASK;
}

void UART5_SendByte(uint8 data)
{
    while(!(UART5_FR_R & UART5_FR_TXFE_MASK));
    UART5_DR_R = data;
}

uint8 UART5_ReceiveByte(void)
{
    while(UART5_FR_R & UART5_FR_RXFE_MASK);
    return UART5_DR_R;
}

void UART5_ReceiveString(uint8 *pData)
{
    UART5FlushReceiver();
    uint32 uCounter = 0;

    pData[uCounter] = UART5_ReceiveByte();

    while(pData[uCounter] != '\r')
    {
        uCounter++;
        pData[uCounter] = UART5_ReceiveByte();
    }
    pData[uCounter] = '\0';
}

void UART5_SendString(const uint8 *pData)
{
    UART5FlushTransmitter();
    uint32 uCounter = 0;

    while(pData[uCounter] != '\0')
    {
        UART5_SendByte(pData[uCounter]);
        uCounter++;
    }
}

void UART5FlushReceiver(void) {
    // Check if the receiver FIFO is empty
    while ((UART5_FR_R & UART_FR_RXFE) == 0) {
        // Read and discard data from the FIFO to clear it
        (void)UART5_DR_R;
    }
}

// Function to flush the UART5 transmitter FIFO
void UART5FlushTransmitter(void) {
    // Wait until the transmitter FIFO is empty
    while ((UART5_FR_R & UART_FR_TXFE) == 0);
}

