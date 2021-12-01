#ifndef __USART_H
#define __USART_H

void USART_Init(unsigned int ubrr);

void USART_Transmit(const unsigned char data);

unsigned char USART_Receive(void);

#endif