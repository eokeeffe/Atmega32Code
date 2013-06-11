/*
 * Project4.c
 *
 * Created: 18/05/2013 22:54:11
 *  Author: Evan O'Keeffe
 *
 *	Simple Voltmeter using interrupts
 */ 


#include <avr/io.h>
#include "avr.h"
#include "lcd.h"
#include "keypad.h"

#define MAX_SAMPLES 40

//ADC voltage conversion
volatile float voltage;
//ADC result from the ISR
volatile long adcResult;
volatile float min,max,avg,total;
volatile int samples;

volatile char recording;

struct key_state
{
	char current;
	char previous;
};
struct key_state key;

char line1[16];//line 1 of the display
char line2[16];//line 2 of the display

void ini_ADC()
{
	ADMUX=(1<<REFS0); // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescalar div factor =128
}

uint16_t readADC(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ch &= 0b00000111;
	ADMUX |= ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	//Note you may be wondering why we have write one to clear it
	//This is standard way of clearing bits in I/O as said in data sheets.
	//The code writes '1' but it result in setting bit to 0

	ADCSRA|=(1<<ADIF);

	return(ADC);
}

void ini_avr(void){WDTCR = 15;}

void wait_avr(unsigned short msec)
{
	TCCR0 = 3;
	while (msec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void printString(char *string)
{
	char c,len=0;
	while((c = string[len++]) != '\0')
	{
		put_lcd(c);
	}
}

void reset()
{
	voltage = min = max = avg = 0.0;
	samples = 0;
}

void user_input()
{
	switch(getkey())
	{
		case 'A':
		{
			recording = TRUE;
			ini_values();
			ini_ADC(); 
			break;
		}
		case 'B':
		{
			recording = FALSE;
			reset();// reset the display
			break;
		}
		case 'C':{break;}
		case 'D':{break;}
		default:{break;}
	}
}

void ini_values()
{
	max = 0.0;
	min = 5.0;
	avg = 0.0;
	voltage = 0.0;
	samples = 0;
	total = 0.0;
}

int main(void)
{
	ini_avr();
	ini_lcd();
	
    for(;;)
	{
		if(recording)
		{
			/*
				Function Updates and maintains the average
				voltage over a number of samples
			*/
			adcResult = readADC(0);
			samples++;
			
			if(samples > MAX_SAMPLES){samples = 1;total = 0.0;}
			voltage = adcResult/1023.0 * 5.0;
			total += voltage;
			avg = total / samples;
			
			/* update the min and max numbers */
			//if the current value isn't 0 then set the min
			if(voltage < min  && voltage > 0.00){min = voltage;}
			//if the current value is greater than max , set max
			if(voltage > max){max = voltage;}
			
			clr_lcd();
			sprintf(line1,"IV:%2.2f AV:%2.2f",voltage,avg);
			sprintf(line2,"MX:%2.2f MN:%2.2f",max,min);
			pos_lcd(0,0);printString(line1);
			pos_lcd(1,0);printString(line2);
			
		}
		else
		{
			clr_lcd();
			sprintf(line1,"IV:-- AV:--");
			sprintf(line2,"Max:-- Min:--");
			pos_lcd(0,0);printString(line1);
			pos_lcd(1,0);printString(line2);
		}
		user_input();
		wait_avr(250);
	}
}