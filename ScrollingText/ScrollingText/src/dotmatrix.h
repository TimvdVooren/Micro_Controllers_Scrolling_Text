
/*
 * dotmatrix.h
 *
 * Created: 28-3-2019 13:12:21
 *  Author: rickw
 */ 
#ifndef DOTMATRIX_H
#define DOTMATRIX_H

#define SCROLLING_FAST		1
#define SCROLLING_AVERAGE	4
#define SCROLLING_SLOW		8

void startDotmatrix(void);
void stopDotmatrix(void);
void writeToDotmatrix(unsigned char data);
void dotmatrixInit(void);

void clearDisplay(void);
void clearDotmatrixBuffer(void);
void selectLetters(char *word);
void scrollRight(void);
void scrollLeft(void);

void testDotmatrix(void);
void setScrollingSpeed(int adjustedSpeed);
int getScrollingSpeed(void);

#endif
