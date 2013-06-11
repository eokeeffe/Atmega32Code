//touch button blinking lights

#include<avr/io.h>
#include "avr.h"

#define LED PB0
#define BUTTON PA0
#define DELAY 500
//simple macros for setting pins on high/low or input/output
#define set_low(port,pin) port&= ~(1<<pin)
#define set_high(port,pin) port |= (1<<pin)
#define set_input(portdir,pin) portdir &= ~(1<<pin)
#define set_output(portdir,pin) portdir |= (1<<pin)

void ini_avr(void)
{

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

int main(void)
{
	//set the direction of port 40 to be an input
	set_input(PIN,BUTTON);
	//set direction of port 1 to be an output
	set_output(DDRD,LED);
	
	while(1)
	{
		//if(GET_BIT(PIN,BUTTON))
		//{
			set_high(PORTD,LED);
			wait_avr(DELAY);
			set_high(PORTD,LED);
			wait_avr(DELAY);
		//}
	}
}