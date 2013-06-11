/*
 * avr.h
 *
 * Created: 27/04/2013 21:51:14
 *  Author: evan
 */ 
#ifndef __avr__
#define __avr__

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
/*Clock speed for the AVR*/
#define XTAL_FRQ 1000000lu

#define SET_BIT(p,i) ((p) |=  (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) &   (1 << (i)))

#define WDR() asm volatile("wdr"::)
#define NOP() asm volatile("nop"::)
#define RST() for(;;);

void ini_avr(void);
void wait_avr(unsigned short msec);
void printString(char *string);
void Timer0_Init(void);
void SetupTimers();
void play_song();
void set_temp();
void set_volume();
void load_song(char song_num);
void clear_buffer();
void get_name(char num);

#endif
