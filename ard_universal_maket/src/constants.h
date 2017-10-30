// constants.h

#ifndef _CONSTANTS_h
#define _CONSTANTS_h


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