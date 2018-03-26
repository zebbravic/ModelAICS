// protoLib.h

#ifndef _PROTOLIB_h
#define _PROTOLIB_h

#define LENTS_USE 0
#if LENTS_USE==1
	#include "LentLib.h"
#endif
#include "constants.h"
#include "MemoryFree.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


struct msg
{
	byte adress;
	byte code;
	word opsSize;
	byte *operands;
};

struct DOut
{
	byte pin;
	word num;
	//unsigned long int timeout;
};

struct DOutRange
{
	word startDOut;
	word endDOut;
};

struct DOutRandomizer
{
	byte type;
	byte maxGlobal;
	byte maxPerDevice;
	byte maxCnt;
	unsigned long timeout;
	word* randomArr;
};

struct metaDOut
{
	word channel;
	byte mode;
	byte channelState;
	byte DOutCnt;
	byte* dOutArr; //from 15.03 storages indexes of dOuts. С 15.03 хранит индексы нужных DOut из массиве dOuts
	byte DOutRangeCnt;
	DOutRange* range;
	byte state;
	byte intens;
	byte direction;
	word num;
	DOutRandomizer* randomizer;
	unsigned long stepTimout;

};

struct ActCnl
{
	word channel;
	unsigned long int timout;
};



void writeMsg(msg message, byte port);
void initCntrl();
void dataRequest();
void parseRecieveMsg(msg rcvMsg);
void parseAdressMsg(msg message);
msg parseBuffer();
byte checkState();
bool addDOut(byte* params);
bool addMetaDOut(byte* params);
bool addDOutRandomizer(byte* params);
void beginInit(byte* params);
void endInit(byte* params);
void sync();
void sendSyncErr();
void sendLoopPing();

bool checkWordInArr(word value, word* arr, byte arrSize);
bool checkByteInArr(byte value, byte* arr, byte arrSize);
bool checkWordInRangeArr(word value, DOutRange* arr, byte arrSize);
byte findNumInDOutsArr(word value);

void actChannel(byte* params);
void deactChannel(byte* params);
void deactAllChannel();
void deactLastChannel();
void checkChannel();

void actMetaDOut(word channel);
void deactMetaDOut(word channel);
void checkMetaDOut();

byte getActCnlNum();

void lightOnConst(metaDOut* tMetaDout);
void lightOnBlinkDO(metaDOut* tMetaDout);
void lightOnPWM(metaDOut* tMetaDout);
void lightOnRandom(metaDOut* tMetaDout);
#endif
