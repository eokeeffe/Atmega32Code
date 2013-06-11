/*
 * keypad.h
 *
 * Created: 28/04/2013 00:57:37
 *  Author: evan
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

char getkey(void);
unsigned char getSinglebutton(unsigned char row,unsigned char column);
unsigned char scanKeypad();

#endif /* KEYPAD_H_ */