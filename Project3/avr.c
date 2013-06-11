/*
* avr.c
*
* Created: 12/05/2013 18:51:41
*  Author: Evan O'Keeffe

Plays midi tones through the speaker using fast pwm using timers 0 and 1
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "memoryStats.h"
#include "keypad.h"
#include "lcd.h"
#include "avr.h"
#include "songs.h"

const int *song;//pointer to space where the song is

#define TRUE 1
#define FALSE 0

short DURATION_SEED = 32;
int Volume = 80;//the lower the volume number , the louder it is
static char Duration = 0;
static char Tone = 0;
static char Tempo;
//is the song playing
short Splaying = FALSE;
//what song is being played
char selection = 0;

void ini_avr(void)
{
	WDTCR = 15;
}

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

void Timer0_Init(void)
{
	/*
		Initialize Timer0.
		Used to give the correct time-delays in the song
	*/
	TIMSK = (1<<OCIE0);// Enable timer0 compare interrupt
	OCR0 = 28;// Sets the compare value
	TCCR0 = (1<<WGM01)|(0<<WGM00)|(4<<CS00);// Set Clear on Timer Compare (CTC) mode, CLK/256 prescaler
}

//when the comparison interrupt signal is raised , play a tune :)
ISR(TIMER0_COMP_vect)
{
	/************************************************************************/
	/*     Sample the music using the correct tone and duration with tempo  */
	/*     of the song                                                      */
	/************************************************************************/
	
	//cli();//disable interrupts for a moment
	
	unsigned int temp_tone;
	int temp_hi;
	char loop;
	
	if(!Tone)
	{//Set the Tempo to the first number
		Duration = 0;
		Tempo = (uint8_t)pgm_read_word(song + 0);//first number is almost always 2
		Tone = 1;//Start the song from the beginning
	}
	if(!Tempo)
	{
		//if the duration of tone isn't 0
		if(Duration){ Duration--; }
			/*if the song still has notes*/
		else if( pgm_read_word(song + Tone) )
		{
			//set the duration of the tone
			Duration = ( DURATION_SEED / pgm_read_word(song + Tone));
			Tone++;
			temp_tone = (uint8_t)pgm_read_word(song + Tone);
			
			if( (temp_tone == p) | (temp_tone == P) )// if pause tone
			{CLR_BIT(TCCR1B, CS10);/*stop Timer1, prescaler(1)*/}
			else
			{SET_BIT(TCCR1B, CS10);/*start Timer1, prescaler(1)*/}
			
			
			
			temp_hi = temp_tone;
			temp_hi >>= 8;// shift 8 bits to the right for freq setting
			
			TCNT1H = 0;// reset TCNT1H/L
			TCNT1L = 0;
			
			//load the tone and frequency into the 16bit register
			ICR1H = temp_hi;// load ICR1H/L
			ICR1L = temp_tone;
			
			Tone++;
		}
		else// the end of song
		{
			Tone++;
			if(  Tone >= ninth_length )
			{
				Tone = 0;//set the counter to zero
				Splaying = FALSE;
				CLR_BIT(TCCR1B, 0);
				TCCR1A = 0;
				TCCR1B = 0;
				SET_BIT(PORTD, 5);// set OC1A high
				pos_lcd(0,0);
				puts_lcd(PSTR("End of Song"));
				wait_avr(3000);
				cli();
			}
		}
		Tempo = (uint8_t)pgm_read_word(song + 0);
	}
	else
	{Tempo--;}
		
	//sei();//enable the interrupts again
}

void SetupTimers()
{
	TCCR1A = (1<<COM1A1);
	TCCR1B = (1<<WGM13);
	SET_BIT(TCCR1B, CS10);//start timer 1 with pre-scaler 1 
	OCR1AH = 0; // set initial OCR1A
	OCR1AL = Volume;//sets the volume of the song
}

void play_song()
{
	/************************************************************************/
	/* Setup the timers and whatnot to play the song                        */
	/************************************************************************/

	SET_BIT(DDRD,5);
	SET_BIT(PORTD,5);
	
	Duration = 0;
	Tone = 1;
	Splaying = TRUE;
	
	SetupTimers();
	Timer0_Init();
	sei();//enable global interrupts
}

void set_temp()
{
	clr_lcd();
	pos_lcd(0,0);
	puts_lcd(PSTR("Enter Tempo:"));
	char length[3];
	char count=0;
	while(count<3)
	{
		char key = getkey();
		if(key>47 && key <58)
		{
			length[count++] = key;
			put_lcd(key);
			wait_avr(1000);
		}
	}
	DURATION_SEED = ((length[0]-48)*100) + ((length[1]-48)*10) + ((length[2]-48));
	pos_lcd(1,0);
	puts_lcd(PSTR("Tempo="));
	sprintf(length,"%d",DURATION_SEED);
	printString(length);
	wait_avr(3000);
	clr_lcd();
}

void set_volume()
{
	clr_lcd();
	pos_lcd(0,0);
	puts_lcd(PSTR("Enter Volume:"));
	char length[3];
	char count=0;
	while(count<3)
	{
		char key = getkey();
		if(key>47 && key <58)
		{
			length[count++] = key;
			put_lcd(key);
			wait_avr(1000);
		}
	}
	Volume = ((length[0]-48)*100) + ((length[1]-48)*10) + ((length[2]-48));
	pos_lcd(1,0);
	puts_lcd(PSTR("Vol="));
	sprintf(length,"%d",Volume);
	printString(length);
	wait_avr(3000);
	clr_lcd();
}

void load_song(char song_num)
{
	/************************************************************************/
	/* Set the pointer to the song being looked for                          */
	/************************************************************************/
	switch(song_num)
	{
		default:{song=(int*)pgm_read_word(&Songs[1]);break;}
	}
}

void get_name(char num)
{
	switch(num)
	{
		default:{puts_lcd(PSTR("Mozrt:Ninth"));break;}
	}
}

void main_loop()
{
		pos_lcd(0,0);
		puts_lcd(PSTR("Play:A,"));
		pos_lcd(1,0);
		puts_lcd(PSTR("Set Tempo?: B"));
		char key = getkey();
		switch(key)
		{
			case 'A':
			{
				Splaying=TRUE;
				load_song(selection);
				clr_lcd();
				play_song();
				break;
			}
			case 'B':
			{
				set_temp();
				break;
			}
			case 'C':
			{
				set_volume();
				break;
			}
			case 'D':
			{
				clr_lcd();
				pos_lcd(0,0);
				puts_lcd(PSTR("Song#(1-4):"));
				while(1)
				{
					char key = getkey();
					pos_lcd(1,0);
					put_lcd(key);
					if(key=='0'||key=='1' || key=='2' || key =='3')
					{
						selection = key-48;
						break;
					}
				}
				pos_lcd(1,0);
				get_name(selection);
				wait_avr(3000);
				clr_lcd();
				break;
			}
			default:{break;}
		}
}

int main(void)
{
	//clear_buffer();
	ini_avr();
	ini_lcd();
	//set the initial tempo
	DURATION_SEED = 32;
	
	
	if (!Splaying)
	{
		
		clr_lcd();
		pos_lcd(0,0);
		/*
		puts_lcd(PSTR("Playing:"));
		pos_lcd(1,0);
		get_name(selection);
		*/
		char buffer[12];
		sprintf(buffer,"Stack:%d",freeRam());
		printString(buffer);
		load_song(0);
		play_song();	
	}
	
	
	for(;;)
	{
		//main function to run
		//if(!Splaying){main_loop();}
	}
}