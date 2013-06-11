/*
 * day.h
 *
 * Created: 30/04/2013 21:13:51
 *  Author: evan
 */ 


#ifndef DAY_H_
#define DAY_H_

// years ending with 00 have to be divisible by 400 to leap
// note the "&&" is a DaniWeb problem and should be a double & for AND
#define isleapyear(year) ((!(year % 4) && (year % 100)) || (!(year % 400) && (year % 1000)))
int isdatevalid(int month, int day, int year);
int weekday(int month, int day, int year);

#endif /* DAY_H_ */