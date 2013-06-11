/*
 * memoryStats.c
 *
 * Created: 18/05/2013 22:42:29
 *  Author: evan
 */ 

int freeRam () 
{
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}