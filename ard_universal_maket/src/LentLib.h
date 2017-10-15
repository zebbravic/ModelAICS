// LentLib.h

#ifndef _LENTLIB_h
#define _LENTLIB_h


#include "Adafruit_NeoPixel.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#define STEP_TIMEOUT 1000L
#define STEP_TIMEOUT_FLOWPIXEL 200L
#define STEP_TIMEOUT_PWM 100L
#define CHANNEL_TIMEOUT 180000L

#define LENTMODE_ONE_FLOAT 0
#define LENTMODE_FILL 1
#define LENTMODE_BLINK 2
#define LENTMODE_WAVE 3


#define CODE_INIT_SEGM 18
#define CODE_INIT_LENT 19

// #define CODE_INIT_SEGM 6
// #define CODE_INIT_LENT 7

struct segment
{
	byte num;
	byte pin;
	byte lentSize;
	Adafruit_NeoPixel* lent;
	byte curPixel;
};


struct lent
{
	byte channel;
	byte mode;
	byte RED;
	byte GREEN;
	byte BLUE;
	byte segCnt;
	byte* segArr;
	unsigned long nextStep;
	bool channelState;
	byte state;
	byte curIntens;
	byte direction;
};

bool addLent(byte* params);
bool addSegment(byte* params);
byte actLent(byte channel);
byte deactLent(byte channel);
void checkLent();
lent lightOnOneFlow(lent tLent);
lent lightOnFill(lent tLent);
lent lightOnBlink(lent tLent);
lent lightOnWave(lent tLent);
byte getSegLent();
#endif
