
/*
 * wait.c
 *
 * Created: 28-3-2019 13:18:52
 *  Author: rickw
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void wait(int ms)
{
	for (int i = 0; i < ms; i++)
	{
		_delay_ms(1);
	}
}