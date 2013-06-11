/*
 * Blinking_Lights.c
 *
 * Created: 08/04/2013 12:42:06
 *  Author: evan
 */ 



#include<avr/io.h>
#include "avr.h"

#define DELAY 500


void ini_avr(void)
{
	DDRA = 0x00;//all pins of PORTA declared as input
	DDRB = 0x01;//all pins of PORTB declared as output
	
	PORTA = 0x00;//set PORTA pins to off
	PORTB = 0x00;//set PORTB pins to off
}

void wait_avr(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--)
	{
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void my_wait(int msec)
{
	//simply go through a loop
	//so at 1MHz the estimated number of instructions to
	//go for about 500ms is 5000 , I found that the number of around
	//about 5500 but again this could be done to environment in which I tested it
	while(msec--);
}

int main(void)
{
	//initialize the pin setups
	ini_avr();
	
	for(;;)
	{
		if(!GET_BIT(PINA,0))
		{
			PORTB = 0x01;//High State
			wait_avr(DELAY);//delay
			PORTB = 0x00;//Low State
			wait_avr(DELAY);//delay
		}
	}
}