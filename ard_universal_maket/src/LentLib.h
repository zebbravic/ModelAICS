// LentLib.h

#ifndef _LENTLIB_h
#define _LENTLIB_h


#include "Adafruit_NeoPixel.h"
#include "constants.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif




struct segment
{
	word num;
	byte pin;
	byte lentSize;
	Adafruit_NeoPixel* lent;
	byte curPixel;
};


struct lent
{
	word channel;
	byte mode;
	byte RED;
	byte GREEN;
	byte BLUE;
	byte segCnt;
	word* segArr;
	unsigned long nextStep;
	bool channelState;
	byte state;
	byte curIntens;
	byte direction;
};

bool addLent(byte* params);
bool addSegment(byte* params);
byte actLent(word channel);
byte deactLent(word channel);
void checkLent();
lent lightOnOneFlow(lent tLent);
lent lightOnFill(lent tLent);
lent lightOnBlink(lent tLent);
lent lightOnWave(lent tLent);
byte getSegLent();
#endif
