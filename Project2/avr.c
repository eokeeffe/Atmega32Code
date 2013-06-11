/*
* Project2.c
*
* Created: 27/04/2013 21:49:53
*  Author: evan
*/

#include<avr/io.h>
#include "avr.h"
#include "lcd.h"
#include "keypad.h"
#include "day.h"
//globals
const char week[7][10] = {
	"Mon","Tue","Wed","Thu",
	"Fri","Sat","Sun"
};

char date[13],time[13];
short hour,min,sec;
int month,day,year;
short time_set=0,military=0;

void ini_avr(void)
{
	WDTCR = 15;
}

char getkey(void)
{
	switch(scanKeypad())
	{
		case 1:{return '1';}
		case 2:{return '2';}
		case 3:{return '3';}
		case 4:{return 'A';}
		case 5:{return '4';}
		case 6:{return '5';}
		case 7:{return '6';}
		case 8:{return 'B';}
		case 9:{return '7';}
		case 10:{return '8';}
		case 11:{return '9';}
		case 12:{return 'C';}
		case 13:{return '*';}
		case 14:{return '0';}
		case 15:{return '#';}
		case 16:{return 'D';}
		default:{return '\0';}
	}
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

void tester()
{
	while(1)
	{
		clr_lcd();
		pos_lcd(0,0);
		puts_lcd(PSTR("It Works!"));
		pos_lcd(1,0);
		puts_lcd(PSTR("Finish it!"));
		wait_avr(2000);
	}
}

void clr_str(char *s,short length)
{
	short len = 0;
	while( len < length)
	{
		s = '\0';
		s++;
		len++;
	}
}

void print_str(char *s)
{
	char c;
	short len = 0;
	while( (c = s[len]) != '\0')
	{
		put_lcd(c);
		len++;
	}
}

void display()
{
	clr_lcd();
	pos_lcd(0,0);
	print_str(date);
	pos_lcd(1,0);
	print_str(time);
}

void set_time()
{
	/************************************************************************/
	/* Set the time string ,save it in the ram                              */
	/* get only numbers here												*/
	/************************************************************************/
	char count=0,key_value,d[] = "Enter Time:";
	pos_lcd(0,0);
	print_str(d);
	while(count<8)
	{
		key_value = getkey();
		if(key_value!= '\0' && key_value != 'A' &&
		key_value != 'B' && key_value != 'C' &&
		key_value != 'D'
		)
		{
			time[count] = key_value;
			pos_lcd(1,0);
			print_str(time);
			count++;
			if(count==2||count==5){count++;}
			wait_avr(450);
		}
	}
	hour = ((time[0]-48)*10)+((time[1]-48)*1);
	min = ((time[3]-48)*10)+((time[4]-48)*1);
	sec = ((time[6]-48)*10)+((time[7]-48)*1);
	if(hour>24)
	{
		hour = hour%24;
	}
	if(min>60)
	{
		hour += min%60;
		min += sec%60;
	}
	if(sec>60)
	{
		min += sec%60;
		sec = sec%60;
	}
	time_set=1;
	wait_avr(2000);
}

void set_date()
{
	/************************************************************************/
	/* Set the date string ,save it in the ram                              */
	/* get only numbers here												*/
	/************************************************************************/
	char count=0,key_value,d[] = "Enter Date:";
	pos_lcd(0,0);
	print_str(d);
	while(count<10)
	{
		key_value = getkey();
		if(key_value!= '\0' && key_value != 'A' &&
		 key_value != 'B' && key_value != 'C' &&
		 key_value != 'D'
		 )
		{
			date[count] = key_value;
			pos_lcd(1,0);
			print_str(date);
			count++;
			if(count==2||count==5){count++;}
			wait_avr(250);
		}
	}
	month = ((date[0]-48)*10)+(date[1]-48);
	day = ((date[3]-48)*10)+(date[4]-48);
	year = ((date[6]-48)*1000)+((date[7]-48)*100)+((date[8]-48)*10)+((date[9]-48));
	
	if(isdatevalid(month,day,year))
	{
		int d = weekday(month,day,year);
		date[10]=' ';
		date[11]=week[d][0];
		date[12]=week[d][1];
		date[13]=week[d][2];
		date[14]='\0';
	}
	else
	{
		clr_lcd();
		pos_lcd(0,0);
		print_str("INVALID DATE");
		pos_lcd(1,0);
		print_str("mon/day/year");
		wait_avr(3000);
		clr_lcd();
		set_date();
	}
	wait_avr(2000);
}

void checkforinput()
{
	pos_lcd(0,10);
	switch(getkey())
	{
		case 'A':
		{
			clr_lcd();
			put_lcd('A');
			set_date();
			break;
		}
		case 'B':
		{
			clr_lcd();
			put_lcd('B');
			set_time();
			break;
		}
		case 'C':
		{
			clr_lcd();
			put_lcd('D');
			pos_lcd(0,0);
			if(military)
			{
				military=0;
				print_str("Military off");
			}
			else
			{
				military = 1;
				print_str("Military on");
			}
			wait_avr(3000);
			
			break;	
		}
		case 'D':
		{
			clr_lcd();
			pos_lcd(0,0);
			print_str("By Evan O'Keeffe");
			wait_avr(5000);
			display();
			break;
		}
		default:{break;}
	}
}

void update_time()
{
	if(sec>=60)
	{
		min += 1;
		sec = sec%60;
	}
	if(min>=60)
	{
		hour += 1;
		min = sec%60;
	}
	if(hour>24)
	{
		hour = hour%24;
	}
	
	if(military)
	{
		time[0]= (hour/10)+48;
		time[1]= (hour%10)+48;
		time[3]= (min/10)+48;
		time[4]= (min%10)+48;
		time[6]= (sec/10)+48;
		time[7]= (sec%10)+48;
		time[8]=time[9]=time[10]=' ';
	}
	else
	{
		if(hour>=12)
		{
			time[0]= ((hour-12)/10)+48;
			time[1]= ((hour-12)%10)+48;
			time[3]= (min/10)+48;
			time[4]= (min%10)+48;
			time[6]= (sec/10)+48;
			time[7]= (sec%10)+48;
			time[8]=' ';
			time[9]='p';
			time[10]='m';
		}
		else
		{
			time[0]= ((hour)/10)+48;
			time[1]= ((hour)%10)+48;
			time[3]= (min/10)+48;
			time[4]= (min%10)+48;
			time[6]= (sec/10)+48;
			time[7]= (sec%10)+48;
			time[8]=' ';
			time[9]='a';
			time[10]='m';
		}			
	}
	pos_lcd(1,0);
	print_str(time);
}

void mainloop()
{
	while(1)
	{
		display();
		if(time_set)
		{
			sec++;
			wait_avr(1000);
			update_time();
		}
		checkforinput();
	}
}

int main(void)
{
	//initialize everything else
	ini_avr();
	ini_lcd();
	for(short i=0;i<10;i++){date[i]=time[i]='0';}
	date[2]=date[5]= '/';//for the date
	time[2]=time[5]= ':';//for the time
	date[12]=time[11]='\0';//null end the strings
	//enter the main loop
	mainloop();
}