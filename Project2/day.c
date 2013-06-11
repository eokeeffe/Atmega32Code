/*
 * day.c
 *
 * Created: 30/04/2013 21:14:04
 *  Author: evan
 */ 
#include "day.h"
//   return 1 if date is valid, 0 otherwise.
int isdatevalid(int month, int day, int year)
{
  if (day <= 0) return 0 ;
  switch( month )
	{
	  case 1  :
	  case 3  :
	  case 5  :
	  case 7  :
	  case 8  :
 	  case 10 :
	  case 12 : if (day > 31) return 0 ; else return 1 ;
	  case 4  :
	  case 6  :
	  case 9  :
	  case 11 : if (day > 30) return 0 ; else return 1 ;
	  case 2  : 
	    if ( day > 29 ) return 0 ;
      if ( day < 29 ) return 1 ;
      if (isleapyear(year)) return 1 ;   // leap year
    else return 0 ;
	}
  return 0 ;
}
//
// given month, day, year, returns day of week, eg. Monday = 0 etc.
// tested for 1901 to 2099 (seems to work from 1800 on too)
// 
int weekday(int month, int day, int year)
{	
  int ix, tx, vx;
  switch (month) {
    case 2  :
	case 6  : vx = 0; break;
	case 8  : vx = 4; break;
	case 10 : vx = 8; break;
	case 9  :
	case 12 : vx = 12; break;
	case 3  :
	case 11 : vx = 16; break;
	case 1  :
	case 5  : vx = 20; break;
	case 4  :
	case 7  : vx = 24; break;
  }
  if (year > 1900)  // 1900 was not a leap year
    year -= 1900;
  ix = ((year - 21) % 28) + vx + (month > 2);  // take care of February 
  tx = (ix + (ix / 4)) % 7 + day;              // take care of leap year
  return (tx % 7);
}