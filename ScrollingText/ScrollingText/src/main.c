
/*
 * main.c
 *
 * Created: 28-3-2019 13:12:21
 *  Author: rickw
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "wait.h"
#include "letters.h"
#include "dotMatrix.h"

int scrollDirection = 0;

void adcInit();

ISR( INT4_vect )
{
	switch(getScrollingSpeed())
	{
		case SCROLLING_FAST:
			setScrollingSpeed(SCROLLING_AVERAGE);
			break;
		case SCROLLING_AVERAGE:
			setScrollingSpeed(SCROLLING_SLOW);
			break;
		case SCROLLING_SLOW:
			setScrollingSpeed(SCROLLING_FAST);
			break;
	}
}

ISR( INT5_vect )
{
	PORTB = 0b00000010;
}

ISR( INT0_vect )
{
	if(scrollDirection == 0)
	{
		scrollDirection = 1;
	}
	else
	{
		scrollDirection = 0;
	}
}

ISR( INT7_vect )
{
	PORTB = 0b00000100;
}

void adcInit()
{
	ADMUX  = 0b11000001;
	ADCSRA = 0b11000110;
}

int main( void )
{	
	DDRF = 0x00;
	DDRB = 0xFF;
	DDRD = 0xFF;
	DDRE = 0x00;
	
    EICRB |= 0b00000010;  
	//EICRA |= 0b00000010;        
    EIMSK |= 0b00010001;           

	sei();
	
	adcInit();
	dotmatrixInit();
	clearDisplay();
	
	char *word = "WARMTE";
	char *tempWord;
	
	int temp = ADCL;
	int lastTemp = temp;
	PORTB = ADCL;
	
	if(temp > 45)
	{
		tempWord = word;
		char *str = ">";
		strcat(tempWord, str);
	}
	else 
	{
		tempWord = word;
		char *str = "<";
		strcat(tempWord, str);
	}
	selectLetters(tempWord);
	
	while (1)
	{
	    scrollLeft();		
		
		wait(getScrollingSpeed()*100);
	}

	return 1;
}