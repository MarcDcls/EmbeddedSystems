#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"
#include "bluetooth.h"

void init_bluetooth(void)
{
//init timer normal mode
TCCR0A &= ~(1<<WGM20);
TCCR0A &= ~(1<<WGM21);

//set timer's prescaler at 64 : new freq ~200 000
TCCR2B |=(1<<CS22);	

DDRD |= _BV(PD6);
USART_Init(MYUBRR);
}

//while(1){
//    int cmpt = 0 ;
//    if (TOV0==1){
//        TIFR0 &= ~(1<<TOV0);
//        cmpt++;
//        if(cmpt==784){
//            cmpt=0;
//            USART_Transmit('h');
//        }
//    }
// }

//while(1){
//    _delay_ms(2000);
//	unsigned char data='h';
//    USART_Transmit(data);
//	unsigned char data1='e';
//    USART_Transmit(data1);
//    unsigned char data2='l';
//    USART_Transmit(data2);
//    unsigned char data3='l';
//    USART_Transmit(data3);
//    unsigned char data4='0';
//    USART_Transmit(data4);
//    unsigned char data5=' ';
//    USART_Transmit(data5);
//}
