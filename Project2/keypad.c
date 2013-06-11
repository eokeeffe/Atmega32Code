/*
 * keypad.c
 *
 * Created: 28/04/2013 00:57:51
 *  Author: evan
 */ 
#include <avr/io.h>
#include "avr.h"

unsigned char getSingleButton(unsigned char row,unsigned char column)
{
	//set C3-C0 outputs to 1 except bit column
	//and not C4-C7 are not modified
	DDRC = 0x00;
	DDRC |= 1 << row;
	PORTC |= 1 << column;
	//wait to read the input
	wait_avr(2);
	//now read pin with number row
	//if button is being pressed then the state will change from
	// 1 to 0 
	return ((PINC & (1 << column)) == 0 ? 1:0);
}

unsigned char scanKeypad()
{
	//get the row (pins 0-3)
	//then get the column (pins 4-7)
	//eq = (r*4)+(c-4)+1
	unsigned char row,col;
	for(row=0;row<4;row++)
	{
		for(col=4;col<8;col++)
		{
			if(getSingleButton(row,col))
			{
				return (row*4) + (col-4) + 1;
			}
		}
	}
	return 0;
}