
/*
 * dotmatrix.c
 *
 * Created: 28-3-2019 13:11:33
 *  Author: rickw
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "letters.h"
#include "dotMatrix.h"


int buffer[350];
int updateCount = 0;
int bufferCount = 0;
int scrollingSpeed = SCROLLING_FAST;

void startDotmatrix(void)
{
	TWCR = (0x80 | 0x20 | 0x04);
	while( 0x00 == (TWCR & 0x80) );
}

void stopDotmatrix(void)
{
	TWCR = (0x80 | 0x10 | 0x04);
}

void writeToDotmatrix(unsigned char data)
{
	TWDR = data;
	TWCR = (0x80 | 0x04);
	while (0 == (TWCR & 0x80));
}

void dotmatrixInit(void)
{
	TWSR = 0;
	TWBR = 32;	 // TWI clock set to 100kHz, prescaler = 0
	
	int ints[] = { 0, 2, 8 };
	for (int i = 0; i < 3; i++)
	{
		startDotmatrix();
		writeToDotmatrix(0xE0 + ints[i]);	// Display I2C addres + R/W bit
		writeToDotmatrix(0x21);	// Internal osc on (page 10 HT16K33)
		stopDotmatrix();
		
		startDotmatrix();
		writeToDotmatrix(0xE0 + ints[i]);	// Display I2C address + R/W bit
		writeToDotmatrix(0xA0);	// HT16K33 pins all output
		stopDotmatrix();
		
		startDotmatrix();
		writeToDotmatrix(0xE0 + ints[i]);	// Display I2C address + R/W bit
		writeToDotmatrix(0xE7);	// Display Dimming 8/16 duty cycle
		stopDotmatrix();
		
		startDotmatrix();
		writeToDotmatrix(0xE0 + ints[i]);	// Display I2C address + R/W bit
		writeToDotmatrix(0x81);	// Blink OFF - Display On
		stopDotmatrix();
	}
}

void clearDisplay(void)
{
	for (int i = 0; i < 8; i++)
	{
		startDotmatrix();
		writeToDotmatrix(0xE0);	// Display I2C addres + R/W bit
		writeToDotmatrix(0x00 + (i * 2));	//Select row on matrix
		writeToDotmatrix(0x00);	// wtrit data to matrix
		stopDotmatrix();
	}
	
	for (int i = 0; i < 8; i++)
	{
		startDotmatrix();
		writeToDotmatrix(0xE2);			// Display I2C addres + R/W bit
		writeToDotmatrix(0x00 + (i * 2));	//Select row on matrix
		writeToDotmatrix(0x00);			// data
		stopDotmatrix();
	}
	
	for (int i = 0; i < 8; i++)
	{
		startDotmatrix();
		writeToDotmatrix(0xE8);	// Display I2C addres + R/W bit
		writeToDotmatrix(0x00 + (i * 2));	//Select row on matrix
		writeToDotmatrix(0x00);	// data
		stopDotmatrix();
	}
}

void clearDotmatrixBuffer(void)
{
	for (int i = 0; i < 300; i++)
		buffer[i] = 0;
}

void selectLetters(char *word)
{
	for (int i = 0; i < strlen(word); i++)
	{
		letter letr = getLetter(word[i]);
		if(letr.character != ' ')
		{
			for(int j = 0; j < letr.size; j++)
			{
				buffer[bufferCount] = letr.structure[j];
				bufferCount++;
			}
		
			buffer[bufferCount] = 0b00000000;
			bufferCount++;
		}
		else
		{
			for(int j = 0; j<4; j++)
			{
				buffer[bufferCount] = 0b00000000;
				bufferCount++;
			}
		}
	}
	
	updateCount = bufferCount + 23;
}

void scrollRight(void)
{
	int i = 0;
	for (int j = 0; j <= 2; j ++)
	{
		startDotmatrix();
		switch (j)
		{
			case 0: writeToDotmatrix(0xE0); break; // select display
			case 1: writeToDotmatrix(0xE2); break;
			case 2: writeToDotmatrix(0xE8); break;
		}
		
		for (int k = 0; k <= 14; k += 2)
		{
			writeToDotmatrix(k);				   // write the right data of a letter to the right row
			writeToDotmatrix(buffer[(i - updateCount + bufferCount + 1 >= 0)? i - updateCount + bufferCount + 1 : 299]);
			
			i++;
		}
		
		stopDotmatrix();
	}
	
	updateCount++;
	if (updateCount > bufferCount + 23) // start from the beginning if the whole word is passed
	{
		updateCount = 0;
	}
}

void scrollLeft(void)
{
	int i = 0;
	for (int j = 0; j <= 2; j ++)
	{
		startDotmatrix();
		switch (j)
		{
			case 0: writeToDotmatrix(0xE0); break; // select display
			case 1: writeToDotmatrix(0xE2); break;
			case 2: writeToDotmatrix(0xE8); break;
		}
		
		for (int k = 0; k <= 14; k += 2)
		{
			writeToDotmatrix(k);				   // write the right data of a letter to the right row
			writeToDotmatrix(buffer[(i - updateCount + bufferCount + 1 >= 0)? i - updateCount + bufferCount + 1 : 299]);
			
			i++;
		}
		
		stopDotmatrix();
	}
	
	updateCount--;
	if (updateCount <= 0)  // start from the beginning if the whole word is passed
	{
		updateCount = bufferCount + 23;
	}
}

void testDotmatrix(void)
{
	startDotmatrix();
	writeToDotmatrix(0xE0);
	
	letter letr = getLetter('A');
	for(int i = 0; i < letr.size; i++)
	{
		writeToDotmatrix(i * 2);
		writeToDotmatrix(letr.structure[i]);
	}
	
	stopDotmatrix();
}

void setScrollingSpeed(int s)
{
	scrollingSpeed = s;
}

int getScrollingSpeed(void)
{
	return scrollingSpeed;
}