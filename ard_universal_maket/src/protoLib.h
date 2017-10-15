// protoLib.h

#ifndef _PROTOLIB_h
#define _PROTOLIB_h

#include "LentLib.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define MODE_BYTES 0
#define MODE_ASCII 48

#define DOUT_MODE_CONST 0
#define DOUT_MODE_BLINK 1
#define DOUT_MODE_PWM 2

#define WORKMODE_BOOT 0
#define WORKMODE_INIT 1
#define WORKMODE_NORMAL  2
//group Initializing codes

#define CODE_REBOOTED 0
#define CODE_NEED_DATA 1

#define CODE_INIT_SYNC 2
#define CODE_SYNC 3
#define CODE_END_SYNC 4
#define CODE_BLOCK_ERR 5

// group Pin Modes Codes
// #define CODE_INIT_DOUT 9
#define CODE_INIT_DOUT 16
#define CODE_INIT_METADOUT 17

// #define CODE_INIT_DOUT 6
// #define CODE_INIT_METADOUT 7

// #define CODE_ACT_CNL 8
#define CODE_ACT_CNL 24
#define CODE_DEACT_CNL 25
#define CODE_DEACT_ALL 26
// #define CODE_DEACT_ALL 9
#define CODE_DEACT_LAST 27



struct msg
{
	byte adress;
	byte code;
	byte opsSize;
	byte *operands;
};

struct DOut
{
	byte pin;
	byte num;
	//unsigned long int timeout;
};

struct metaDOut
{
	byte channel;
	byte mode;
	byte channelState;
	byte DOutCnt;
	byte* dOutArr;
	byte state;
	byte intens;
	byte direction;
	unsigned long stepTimout;

};

struct ActCnl
{
	byte channel;
	unsigned long int timout;
};



void writeMsg(msg message, byte port);
void initCntrl(byte tmode, byte taddress);
void dataRequest();
void parseRecieveMsg(msg rcvMsg);
void parseAdressMsg(msg message);
msg parseBuffer();
byte checkState();
bool addDOut(byte* params);
bool addMetaDOut(byte* params);
void beginInit(byte* params);
void endInit(byte* params);
void sync();
void sendSyncErr();

void actChannel(byte* params);
void deactChannel(byte* params);
void deactAllChannel();
void deactLastChannel();
void checkChannel();

void actMetaDOut(byte channel);
void deactMetaDOut(byte channel);
void checkMetaDOut();

byte getActCnlNum();

metaDOut lightOnConst(metaDOut tMetaDout);
metaDOut lightOnBlinkDO(metaDOut tMetaDout);
metaDOut lightOnPWM(metaDOut tMetaDout);
#endif
