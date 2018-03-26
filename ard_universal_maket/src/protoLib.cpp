//
//
//

#include "protoLib.h"
#define ADDRESS 1
#define MODE_CTRL MODE_BYTES

word dOutNum=0, metaDOutNum=0, actCnlNum=0, state=WORKMODE_BOOT, dOutRandomizerCnt=0;
word dOutNumCur=0, metaDOutNumCur=0;
DOut** dOuts;
metaDOut** metaDOuts;
ActCnl** cnlArr;
unsigned long curSyncTimeout=0;
byte syncSize, syncTimeout,curSessSize=0;
msg buffMessage;





void writeMsg(msg message, byte port)
{
	if(port==0)
	{
		Serial.write(message.adress+MODE_CTRL);
		Serial.write(message.code+MODE_CTRL);
		Serial.write((((message.opsSize) >> 8) & 0xFF)+MODE_CTRL);

		//Serial.print((((message.opsSize+mode) >> 8) & 0xFF));
		Serial.write((((message.opsSize)) & 0xFF)+MODE_CTRL);


		for (int i = 0; i < message.opsSize; i++)
		{
			Serial.write(message.operands[i]+MODE_CTRL);
		}
	}
	else
	{
		Serial3.write(message.adress+MODE_CTRL);
		Serial3.write(message.code+MODE_CTRL);
		Serial3.write((((message.opsSize) >> 8) & 0xFF)+MODE_CTRL);
		Serial3.write((((message.opsSize)) & 0xFF)+MODE_CTRL);

		for (int i = 0; i < message.opsSize; i++)
		{
			Serial3.write(message.operands[i]+MODE_CTRL);
		}
	}
}

void initCntrl()
{
	// if(taddress==1)
	// {
	// // 	segment* seg;
	// // 	seg=new segment[29];
  // // seg[0].lent=new Adafruit_NeoPixel(3,2);
  // // seg[1].lent=new Adafruit_NeoPixel(3,3);
  // // seg[2].lent=new Adafruit_NeoPixel(11,4);
  // // seg[3].lent=new Adafruit_NeoPixel(23,5);
  // // seg[4].lent=new Adafruit_NeoPixel(21,6);
  // // seg[5].lent=new Adafruit_NeoPixel(3,7);
  // // seg[6].lent=new Adafruit_NeoPixel(2,8);
  // // seg[7].lent=new Adafruit_NeoPixel(19,9);
  // // seg[8].lent=new Adafruit_NeoPixel(8,10);
  // // seg[9].lent=new Adafruit_NeoPixel(14,11);
  // // seg[10].lent=new Adafruit_NeoPixel(10,12);
  // // seg[11].lent=new Adafruit_NeoPixel(10,16);
  // // seg[12].lent=new Adafruit_NeoPixel(8,17);
  // // seg[13].lent=new Adafruit_NeoPixel(66,18);
  // // seg[14].lent=new Adafruit_NeoPixel(5,19);
  // // seg[15].lent=new Adafruit_NeoPixel(7,20);
  // // seg[16].lent=new Adafruit_NeoPixel(57,21);
  // // seg[17].lent=new Adafruit_NeoPixel(33,22);
  // // seg[18].lent=new Adafruit_NeoPixel(17,23);
  // // seg[19].lent=new Adafruit_NeoPixel(47,24);
  // // seg[20].lent=new Adafruit_NeoPixel(19,25);
  // // seg[21].lent=new Adafruit_NeoPixel(11,26);
  // // seg[22].lent=new Adafruit_NeoPixel(16,27);
  // // seg[23].lent=new Adafruit_NeoPixel(3,28);
  // // seg[24].lent=new Adafruit_NeoPixel(30,29);
	// // seg[25].lent=new Adafruit_NeoPixel(12,30);
  // // seg[26].lent=new Adafruit_NeoPixel(12,31);
  // // seg[27].lent=new Adafruit_NeoPixel(1,32);
  // // seg[28].lent=new Adafruit_NeoPixel(50,33);
  // for(byte j=0;j<3;j++)
  // {
  // for(byte i=0;i<29;i++)
  // {
  //       seg[i].lent->begin();
  //       seg[i].lent->show();
  // }
  // }
	// for(byte i=0;i<29;i++)
  // {
  //       delete seg[i].lent;
  // }
	// delete[] seg;
	// }
	// else
	{
	for(byte i=2;i<13;i++)
	{
		pinMode(i,OUTPUT);
		digitalWrite(i,HIGH);
		delay(20);
		digitalWrite(i,LOW);
		// pinMode(i,INPUT);
	}
	for(byte i=16;i<55;i++)
	{
		pinMode(i,OUTPUT);
		digitalWrite(i,HIGH);
		delay(20);
		digitalWrite(i,LOW);
	}
}
		//pinMode(i,INPUT);

	Serial.begin(9600);
	Serial3.begin(9600);
	buffMessage.operands = new byte[64];
	msg initmsg;
	initmsg.adress = 0;
	initmsg.code = CODE_REBOOTED ;
	initmsg.opsSize = 1;
	byte* ops = new byte[initmsg.opsSize];
	ops[0] = ADDRESS;
	initmsg.operands = ops;
	writeMsg(initmsg,0);

	delete[] ops;
}
byte checkState()
{
	return state;
}
void dataRequest()
{
	msg initmsg;
	initmsg.adress = 0;
	initmsg.code = CODE_NEED_DATA;
	initmsg.opsSize = 1;
	byte* ops = new byte[initmsg.opsSize];
	// for(word i=0;i<initmsg.opsSize;i++)
	// {
	// 	ops[i]=0+mode;
	// }
	ops[0] = ADDRESS;
	initmsg.operands = ops;
	writeMsg(initmsg,0);
	delete[] ops;
}
msg parseBuffer()
{
#if MODE_CTRL == MODE_ASCII
		Serial.println("Entered parseBuffer");
#endif

	//Serial.print(Serial.available());
	// Serial.println(Serial.available());
	if (Serial.available() > 3)
	{
		buffMessage.adress = Serial.read()-MODE_CTRL;
		buffMessage.code = Serial.read()-MODE_CTRL;
		buffMessage.opsSize = (Serial.read()-MODE_CTRL)*256;
		buffMessage.opsSize = buffMessage.opsSize+(Serial.read()-MODE_CTRL);
		// switch (buffMessage.code)
		// {
		// case CODE_REBOOTED: buffMessage.opsSize = 1;
		// 	break;
		// case CODE_NEED_DATA: buffMessage.opsSize = 1;
		// 	break;
		//  case CODE_INIT_SYNC: buffMessage.opsSize = 2;
		//  	break;
		// case CODE_SYNC: buffMessage.opsSize=1;
		// 	break;
		// case CODE_END_SYNC: buffMessage.opsSize=1;
		// 	break;
		// case CODE_BLOCK_ERR: buffMessage.opsSize=1;
		// 	break;
		// case CODE_INIT_DOUT: buffMessage.opsSize = 3;
		// 	break;
		// case CODE_INIT_SEGM: buffMessage.opsSize=4;
		// 	break;
		// case CODE_ACT_CNL: buffMessage.opsSize = 2;
		// 	break;
		// case CODE_DEACT_CNL: buffMessage.opsSize = 2;
		// 	break;
		// case CODE_DEACT_ALL: buffMessage.opsSize = 0;
		// 	break;
		// case CODE_DEACT_LAST: buffMessage.opsSize = 0;
		// 	break;
		// case CODE_INIT_LENT: buffMessage.opsSize = 7;
		// 	break;
		// case CODE_INIT_METADOUT:
		// 	buffMessage.opsSize=4;
		// 	break;
		// default:
		// 	buffMessage.opsSize = 0;
		// 	break;
		// }
		// Serial.println();
		// Serial.println(buffMessage.opsSize);
#if MODE_CTRL==MODE_ASCII

		Serial.print("4 bytes succes\r\n");
		Serial.print(buffMessage.adress);
		Serial.print(" ");
		Serial.print(buffMessage.code);
		Serial.print(" ");
		Serial.print(buffMessage.opsSize);
#endif
		// msg infoMsg;
		// infoMsg.adress = 0;
		// infoMsg.code = CODE_INFO;
		// infoMsg.opsSize = 4;
		// byte* ops1 = new byte[infoMsg.opsSize];
		// ops1[0]=buffMessage.adress;
		// ops1[1]=buffMessage.code;
		// ops1[2]=(buffMessage.opsSize>>8)&0xFF;
		// ops1[3]=(buffMessage.opsSize)&0xFF;
		// infoMsg.operands = ops1;
		// writeMsg(infoMsg,0);
		// delete[] ops1;
		// sendLoopPing();
		if(buffMessage.opsSize>0)
		{
			while (Serial.available()<buffMessage.opsSize)
			{
				true;
			}
		byte i = 0;
		// byte* ops = new byte[buffMessage.opsSize];
		// buffMessage.operands = ops;
		while (i != buffMessage.opsSize)
		{
			buffMessage.operands[i] = Serial.read() - MODE_CTRL;
			// Serial.println(buffMessage.operands[i]);
			i++;
		}
#if MODE_CTRL==MODE_ASCII
		Serial.print("\r\nops\r\n");
		for(byte i = 0; i<buffMessage.opsSize;i++)
		{
			Serial.print(buffMessage.operands[i]);
			Serial.print(" ");
		}
#endif
		// infoMsg.adress = 0;
		// infoMsg.code = 129;
		// infoMsg.opsSize = buffMessage.opsSize;
		// infoMsg.operands = buffMessage.operands;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		// msg infoMsg;
		// infoMsg.adress = 0;
		// infoMsg.code = 130;
		// infoMsg.opsSize = 2;
		// int a = Serial.available();
		// ops1 = new byte[infoMsg.opsSize];
		// ops1[0]=(a>>8)&0xFF;
		// ops1[1]=(a)&0xFF;
		// infoMsg.operands = ops1;
		// writeMsg(infoMsg,0);
		// delete[] ops1;
		// if(buffMessage.code==CODE_INIT_LENT)
		// {
		// 	byte* opsLent = new byte[buffMessage.opsSize+buffMessage.operands[6]];
		// 	while(Serial.available()<buffMessage.operands[6])
		// 		{
		// 			true;
		// 		}
		// 		byte j=0;
		// 		while(j!=buffMessage.opsSize*2)
		// 		{
		// 			opsLent[j]=ops[j];
		// 			j++;
		// 		}
		// 		buffMessage.opsSize+=buffMessage.operands[6];
		// 		while(j!=buffMessage.opsSize*2)
		// 		{
		// 			opsLent[j]=Serial.read()-mode;
		// 			j++;
		// 		}
		// 		delete[] buffMessage.operands;
		// 		buffMessage.operands=opsLent;
		//
		// 	}
		// 	if(buffMessage.code==CODE_INIT_METADOUT)
		// 	{
		// 		byte* opsMeta = new byte[buffMessage.opsSize+buffMessage.operands[3]];
		// 		while(Serial.available()<buffMessage.operands[3])
		// 		{
		// 			true;
		// 		}
		// 		byte j=0;
		// 		while(j!=buffMessage.opsSize*2)
		// 		{
		// 			opsMeta[j]=ops[j];
		// 			j++;
		// 		}
		// 		buffMessage.opsSize+=buffMessage.operands[3];
		// 		while(j!=buffMessage.opsSize*2)
		// 		{
		// 			opsMeta[j]=Serial.read()-mode;
		// 			j++;
		// 		}
		// 		delete[] buffMessage.operands;
		// 		buffMessage.operands=opsMeta;
		//
		// 	}
		 }
		return buffMessage;
	}
#if MODE_CTRL==MODE_ASCII

		Serial.println("Exited parseBuffer");
#endif

}

void parseAdressMsg(msg message)
{
#if MODE_CTRL==MODE_ASCII
		Serial.println("Entered parseAdressMsg");
#endif
	if (message.adress == ADDRESS||message.adress==255)
	{
		parseRecieveMsg(message);
		if (message.adress == 255)
			writeMsg(message, 1);
	}
	else
	{
		if (message.adress == 0)
			writeMsg(message, 0);
		else
			writeMsg(message, 1);
	}
}

void parseRecieveMsg(msg rcvMsg)
{
#if MODE_CTRL==MODE_ASCII

		Serial.println("entered parseRecieveMsg");
		Serial.println(rcvMsg.code);
#endif
	switch (rcvMsg.code)
	{
	case CODE_INIT_DOUT:
		if(state==WORKMODE_INIT){ if(addDOut(rcvMsg.operands)) sync();}
		break;
		#if LENTS_USE == 1
			case CODE_INIT_SEGM:
				if(state==WORKMODE_INIT){ if(addSegment(rcvMsg.operands)) sync();}
				break;
			case CODE_INIT_LENT:
				if(state==WORKMODE_INIT){ if(addLent(rcvMsg.operands)) sync();}
				break;
		#endif
	case CODE_INIT_SYNC:
		if(state==WORKMODE_BOOT) beginInit(rcvMsg.operands);
		break;
	case CODE_INIT_METADOUT:
		if(state==WORKMODE_INIT) { if(addMetaDOut(rcvMsg.operands)) sync();}
		break;
	case CODE_INIT_DOUTRANDOMIZER:
		if(state==WORKMODE_INIT) {if(addDOutRandomizer(rcvMsg.operands)) sync();}
		break;
	case CODE_END_SYNC:
		if(state==WORKMODE_INIT) {endInit(rcvMsg.operands);};
		break;
	case CODE_ACT_CNL:
		if(state==WORKMODE_NORMAL)	actChannel(rcvMsg.operands);
		break;
	case CODE_DEACT_CNL:
		if(state==WORKMODE_NORMAL) deactChannel(rcvMsg.operands);
		break;
	case CODE_DEACT_ALL:
		if(state==WORKMODE_NORMAL) deactAllChannel();
		break;
	case CODE_DEACT_LAST:
		if(state==WORKMODE_NORMAL) deactLastChannel();
		break;
	default:
		break;
	}

}

void beginInit(byte* params)
{
#if MODE_CTRL==MODE_ASCII

		Serial.println("Entered beginInit");
#endif
	state=WORKMODE_INIT;
	syncSize=params[0];
	syncTimeout=params[1];
	dOutNum = word(params[2],params[3]);
	metaDOutNum = word(params[4],params[5]);
	dOutRandomizerCnt = word(params[6],params[7]);
	dOuts = new DOut*[dOutNum];
	metaDOuts = new metaDOut*[metaDOutNum];
	#if LENTS_USE == 1
	#endif
#if MODE_CTRL==MODE_ASCII

		Serial.println("Exited beginInit");
#endif
	// Serial.println("entered init");
}

void endInit(byte* params)
{
	#if LENTS_USE==1
	if(params[0]==getSegLent()+dOutNum+metaDOutNum+dOutRandomizerCnt)
	{
		msg initmsg;
		initmsg.adress = 0;
		initmsg.code = CODE_END_SYNC;
		initmsg.opsSize = 2;
		byte* ops = new byte[initmsg.opsSize];
		ops[1] = params[0];
		ops[0]=ADDRESS;
		initmsg.operands = ops;
		writeMsg(initmsg,0);
		delete[] ops;
		state=WORKMODE_NORMAL;
		}
	else
	{
		sendSyncErr();
	}
	#else
	if(params[0]==dOutNum+metaDOutNum+dOutRandomizerCnt)
	{
		msg initmsg;
		initmsg.adress = 0;
		initmsg.code = CODE_END_SYNC;
		initmsg.opsSize = 2;
		byte* ops = new byte[initmsg.opsSize];
		ops[1] = params[0];
		ops[0]=ADDRESS;
		initmsg.operands = ops;
		writeMsg(initmsg,0);
		delete[] ops;
		state=WORKMODE_NORMAL;
		}
	else
	{
		sendSyncErr();
	}
	#endif

}

void sync()
{
	if(syncSize>0)
	{
		curSessSize++;
		if(curSessSize==syncSize)
		{
			msg initmsg;
			initmsg.adress = 0;
			initmsg.code = CODE_SYNC;
			initmsg.opsSize = 1;
			byte* ops = new byte[initmsg.opsSize];
			ops[0]=ADDRESS;
			initmsg.operands = ops;
			writeMsg(initmsg,0);
			delete[] ops;
			curSessSize=0;
		}
	}
}

void sendSyncErr()
{
	msg initmsg;
	initmsg.adress = 0;
	initmsg.code = CODE_BLOCK_ERR;
	initmsg.opsSize = 2;
	byte* ops = new byte[initmsg.opsSize];
	ops[0]=ADDRESS;
	#if LENTS_USE == 1
	ops[1]=getSegLent()+dOutNum+metaDOutNum+dOutRandomizerCnt;
	#else
		ops[1]=dOutNum+metaDOutNum+dOutRandomizerCnt;
	#endif
	initmsg.operands = ops;
	writeMsg(initmsg,0);
	delete[] ops;
}


void sendLoopPing()
{
	msg pingMsg;
	pingMsg.adress = 0;
	pingMsg.code = CODE_LOOP_PING;
	pingMsg.opsSize = 7;
	byte* ops = new byte[pingMsg.opsSize];
	ops[0] = ADDRESS;
	// // ops[1] = 0;
	// // ops[2] = 0;
	// // ops[3] = 0;
	// // ops[4] = 0;
	// int freeRam = freeMemory();
	unsigned long time = millis();
	ops[1] = ((time >> 24) & 0xFF) ;
 	ops[2] = ((time >> 16) & 0xFF) ;
 	ops[3] = ((time >> 8) & 0xFF);
 	ops[4] = ((time & 0xFF));
	// ops[5] = ((freeRam>>8)&0xFF);
	// ops[6] = freeRam&0xFF;
	pingMsg.operands = ops;
	writeMsg(pingMsg,0);
	delete[] ops;
	#if MODE_CTRL == MODE_ASCII
			Serial.print("FreeRAM");
			// Serial.println(freeRam);
			Serial.println(sizeof(metaDOut));
			Serial.println(sizeof(DOut));
			Serial.println(sizeof(DOut*));
			Serial.println(sizeof(metaDOut*));
	#endif

}

bool addDOut(byte* params)
{
	// DOut** tDouts = new DOut*[dOutNum + 1];
	bool flag=false;
	word num=word(params[1],params[2]);
	// for (byte i = 0; i < dOutNum; i++)
	// {
	// 	tDouts[i] = dOuts[i];
	// 	if(dOuts[i]->num==num)
	// 	{
	// 		flag=true;
	// 		break;
	// 	}
	// }
	if(!flag)
	{
		// msg infoMsg;
		// infoMsg.adress = 0;
		// infoMsg.code = 134;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		DOut* tDOut = new DOut;
		// infoMsg.adress = 0;
		// infoMsg.code = 135;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		tDOut->pin = params[0];
		tDOut->num = num;
		dOuts[dOutNumCur] = tDOut;
		// delete[] dOuts;
		dOutNumCur++;
		pinMode(tDOut->pin, OUTPUT);
		// infoMsg.adress = 0;
		// infoMsg.code = 136;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
	}
	// else
	// {
	// 	delete[] tDouts;
	// }
	return (!flag);
}


bool addMetaDOut(byte* params)
{
	// metaDOut** tMetaDOuts = new metaDOut*[metaDOutNum + 1];
	bool flag=false;
	// for (byte i = 0; i < metaDOutNum; i++)
	// {
	// 	tMetaDOuts[i] = metaDOuts[i];
	// 	// if(metaDOuts.num==params[1])
	// 	// {
	// 	// 	flag=true;
	// 	// 	break;
	// 	// }
	// }
	if(!flag)
	{
		msg infoMsg;
		// infoMsg.adress = 0;
		// infoMsg.code = 144;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		metaDOut* tMetaDOut = new metaDOut;
		// infoMsg.adress = 0;
		// infoMsg.code = 145;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		tMetaDOut->channel=word(params[0],params[1]);
		tMetaDOut->mode=params[2];
		tMetaDOut->num = word(params[3],params[4]);
		tMetaDOut->DOutCnt=params[5];
		tMetaDOut->channelState=false;
		tMetaDOut->stepTimout=0;
		tMetaDOut->direction=1;
		tMetaDOut->intens=0;
		// Serial.println(params[0]);
		// Serial.println(params[1]);
		// Serial.println(params[2]);

		// infoMsg.adress = 0;
		// infoMsg.code = 146;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		if(tMetaDOut->DOutCnt>0)
		{
			byte* tDoutArr = new byte[tMetaDOut->DOutCnt];
			for(byte i=0;i<tMetaDOut->DOutCnt;i++)
			{
					word tDOutNum=word(params[6+i*2],params[7+i*2]);
					tDoutArr[i]=findNumInDOutsArr(tDOutNum);

			}
			tMetaDOut->dOutArr=tDoutArr;
			msg infoMsg;
			infoMsg.adress = 0;
			infoMsg.code = 128;
			infoMsg.opsSize = tMetaDOut->DOutCnt;
			infoMsg.operands = tDoutArr;

			writeMsg(infoMsg,0);
		}
		// infoMsg.adress = 0;
		// infoMsg.code = 147;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		byte offset = 6+tMetaDOut->DOutCnt*2;
		// infoMsg.adress = 0;
		// infoMsg.code = 150;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		tMetaDOut->DOutRangeCnt=params[offset];
		if(tMetaDOut->DOutRangeCnt>0)
		{
			DOutRange* tDOutRangeArr = new DOutRange[tMetaDOut->DOutRangeCnt];
			for(byte i=0;i<tMetaDOut->DOutRangeCnt;i++)
			{
					tDOutRangeArr->startDOut=word(params[offset+1+i*4],params[offset+2+i*4]);
					tDOutRangeArr->endDOut=word(params[offset+3+i*4],params[offset+4+i*4]);

			}
			tMetaDOut->range=tDOutRangeArr;
		}
		// infoMsg.adress = 0;
		// infoMsg.code = 148;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		metaDOuts[metaDOutNumCur] = tMetaDOut;
		// delete[] metaDOuts;
		metaDOutNumCur++;
		// infoMsg.adress = 0;
		// infoMsg.code = 149;
		// infoMsg.opsSize = 0;
		// writeMsg(infoMsg,0);
		// sendLoopPing();
		// metaDOuts = tMetaDOuts;
	}

	// else
	// {
	// 	delete[] tMetaDOuts;
	// }
	return (!flag);
}


bool addDOutRandomizer(byte* params)
{

	bool flag=false;
	word metaNum = word(params[0],params[1]);
	byte i=0;
#if MODE_CTRL==MODE_ASCII

		Serial.println("Entered addDOutRandomizer");
		Serial.println(metaNum);
#endif
	while (metaDOuts[i]->num!=metaNum && i<metaDOutNum)
	{
		i++;
	}
#if MODE_CTRL==MODE_ASCII

		Serial.println(i);
		Serial.println(metaDOutNum);
#endif
	if(i<metaDOutNum)
	{
		DOutRandomizer* tRandomizer = new DOutRandomizer;
		tRandomizer->type = params[2];
		tRandomizer->maxGlobal = params[3];
		tRandomizer->maxPerDevice = params[4];
		tRandomizer->maxCnt = tRandomizer->maxPerDevice;
		tRandomizer->timeout = params[5]*1000UL;
#if MODE_CTRL==MODE_ASCII
			Serial.println("Entered addDOutRandomizer creation");
			Serial.println(metaNum);
#endif
		word* tRArr = new word[tRandomizer->maxCnt];
		for(byte j = 0; j<tRandomizer->maxCnt;j++)
		{
			if(metaDOuts[i]->DOutCnt>0)
			{
				tRArr[j]=metaDOuts[i]->dOutArr[0];
			}
			else
			{
				if(metaDOuts[i]->DOutRangeCnt>0)
				{
					tRArr[j]=metaDOuts[i]->range[0].startDOut;
				}
			}
		}
		tRandomizer->randomArr = tRArr;
		metaDOuts[i]->randomizer = tRandomizer;
		flag = true;
		// dOutRandomizerCnt++;
#if  MODE_CTRL==MODE_ASCII

			Serial.println("Finished addDOutRandomizer");
			Serial.println(dOutRandomizerCnt);
#endif
	}
	// msg initmsg;
	// initmsg.adress = 0;
	// initmsg.code = CODE_INFO;
	// initmsg.opsSize = 5;
	// byte* ops = new byte[initmsg.opsSize];
	// ops[0]=(metaDOuts[i].num>>8)&0xFF;
	// ops[1]=metaDOuts[i].num&0xFF;
	// ops[2]=metaDOuts[i].randomizer->maxCnt;
	// ops[3]=metaDOuts[i].randomizer->randomArr[0];
	// ops[4]=dOutRandomizerCnt;
	// initmsg.operands = ops;
	// writeMsg(initmsg,0);
	// delete[] ops;
	return (flag);
}






void actChannel(byte* params)
{
#if MODE_CTRL==MODE_ASCII

		Serial.println("Entered actChannel");
#endif

	word channel=word(params[0],params[1]);
	//Serial.println(channel);
	actMetaDOut(channel);
	unsigned long int milli=millis();
#if LENTS_USE ==1
	actLent(channel);
	#endif
	bool flag=false;
	ActCnl** tCnlArr = new ActCnl*[actCnlNum + 1];
	for (byte i = 0; i < actCnlNum; i++)
	{
		if(cnlArr[i]->channel!=channel)
		{
			tCnlArr[i] = cnlArr[i];
		}
		else
		{
			flag=true;
			break;
		}
	}
	if(!flag)
	{

		ActCnl* tCnl = new ActCnl;
		tCnl->channel = channel;
		tCnl->timout = milli + CHANNEL_TIMEOUT;
		// tCnl.timout = milli + 30L * 1000L;
		// Serial.println(milli);
		// Serial.println(tCnl.timout);
		tCnlArr[actCnlNum] = tCnl;
		delete[] cnlArr;
		cnlArr = tCnlArr;
		actCnlNum++;
		#if MODE_CTRL==MODE_ASCII

			Serial.println("ActChannel successfull add");
		#endif
	}
	else
	{
		delete[] tCnlArr;
	}
}

void deactChannel(byte* params)
{
	word channel=word(params[0],params[1]);
	if (actCnlNum > 0)
	{
		deactMetaDOut(channel);
		#if LENTS_USE ==1
		deactLent(channel);
		#endif
		//Serial.println(channel);
		bool flag=false;
		ActCnl** tCnlArr = new ActCnl*[actCnlNum - 1];
		for (byte i = 0; i < actCnlNum; i++)
		{
			if (cnlArr[i]->channel==channel)
				flag=true;
			if(flag==true)
			{
				if(cnlArr[i]->channel!=channel)
					tCnlArr[i-1] = cnlArr[i];
			}
			else
			{
				tCnlArr[i]=cnlArr[i];
			}
		}
		delete[] cnlArr;
		cnlArr = tCnlArr;
		actCnlNum--;

	}
}

void deactLastChannel()
{
	if(actCnlNum>0)
	{
		byte params[2]={highByte(cnlArr[actCnlNum-1]->channel),lowByte(cnlArr[actCnlNum-1]->channel)};
		msg infoMsg;
		infoMsg.adress = 0;
		infoMsg.code = 128;
		infoMsg.opsSize = 2;
		byte* ops=new byte[2];
		infoMsg.operands=ops;
		infoMsg.operands[0] = highByte(cnlArr[actCnlNum-1]->channel);
			infoMsg.operands[0] = lowByte(cnlArr[actCnlNum-1]->channel);

		writeMsg(infoMsg,0);
		deactChannel(params);
	}
}
void deactAllChannel()
{
	while(actCnlNum>0)
	{
		deactLastChannel();
	}
}



void actMetaDOut(word channel)
{
	bool flag=false;
	// Serial.println("entered actMetaDOut");
	// msg initmsg;
	// initmsg.adress = 0;
	// initmsg.code = CODE_INFO;
	// initmsg.opsSize = 3;
	// byte* ops = new byte[initmsg.opsSize];
	// ops[0]=1;
	// ops[1]=(channel>>8)&0xFF;
	// ops[2]=(channel)&0xFF;
	// initmsg.operands = ops;
	// writeMsg(initmsg,0);
	// delete[] ops;
	for(byte i=0;i<metaDOutNum;i++)
	{
		// msg initmsg;
		// initmsg.adress = 0;
		// initmsg.code = CODE_INFO;
		// initmsg.opsSize = 8;
		// byte* ops = new byte[initmsg.opsSize];
		// ops[0]=2;
		// ops[1]=(metaDOuts[i].channel>>8)&0xFF;
		// ops[2]=(metaDOuts[i].channel)&0xFF;
		// ops[3]=i;
		// ops[4]=metaDOuts[i].mode;
		// ops[5]=metaDOuts[i].channelState;
		// ops[6]=(channel>>8)&0xFF;
		// ops[7]=(channel)&0xFF;
		// initmsg.operands = ops;
		// writeMsg(initmsg,0);
		// delete[] ops;
		if(metaDOuts[i]->channel==channel)
		{
			metaDOuts[i]->channelState=true;
			flag=true;

		}
	}
	if(flag)
	{
		checkMetaDOut();
	}
}
byte getActCnlNum()
{
	return actCnlNum;
}
void deactMetaDOut(word channel)
{
	for(byte i=0;i<metaDOutNum;i++)
	{
		if(metaDOuts[i]->channel==channel)
		{
			msg infoMsg;
			infoMsg.adress = 0;
			infoMsg.code = 128;
			infoMsg.opsSize = 1;
			byte* ops=new byte[1];
			infoMsg.operands=ops;
			infoMsg.operands[0] = i;

			writeMsg(infoMsg,0);
			metaDOuts[i]->channelState=false;
			//Serial.println(lents[i].segCnt);
			for(byte j=0;j<metaDOuts[i]->DOutCnt;j++)
			{
				// word c=metaDOuts[i]->dOutArr[j];
				// byte dONum=0;
				// while(dOuts[dONum]->num!=c&&dONum<dOutNum)
				// 	dONum++;
				// if(dONum<dOutNum)
				// {
				// 	digitalWrite(dOuts[dONum]->pin,LOW);
				// }
				digitalWrite(dOuts[metaDOuts[i]->dOutArr[j]]->pin, LOW);
				infoMsg.adress = 0;
				infoMsg.code = 128;
				infoMsg.opsSize = 1;
				infoMsg.operands[0] = dOuts[metaDOuts[i]->dOutArr[j]]->pin;

				writeMsg(infoMsg,0);

			}
			for(byte j=0;j<metaDOuts[i]->DOutRangeCnt;j++)
			{
				for(byte k=0;k<dOutNum;k++)
				{
					if(dOuts[k]->num>=metaDOuts[i]->range[j].startDOut && dOuts[k]->num<=metaDOuts[i]->range[j].endDOut)
					{
						digitalWrite(dOuts[k]->pin,LOW);
					}
				}
			}
			metaDOuts[i]->stepTimout=0;
			metaDOuts[i]->intens=0;
			metaDOuts[i]->direction=1;
		}
	}

}

void checkMetaDOut()
{
	// msg infoMsg;
	// infoMsg.adress = 0;
	// infoMsg.code = CODE_INFO;
	// infoMsg.opsSize = 4;
	// byte* ops1 = new byte[infoMsg.opsSize];
	// ops1[0]=actCnlNum;
	// ops1[1]=0;
	// ops1[2]=0;
	// ops1[3]=0;
	// infoMsg.operands = ops1;
	// writeMsg(infoMsg,0);
	// delete[] ops1;
	for(byte i=0;i<metaDOutNum;i++)
	{
		if(metaDOuts[i]->channelState)
		{
			msg initmsg;
			// initmsg.adress = 0;
			// initmsg.code = CODE_INFO;
			// initmsg.opsSize = 2;
			// byte* ops = new byte[initmsg.opsSize];
			// ops[0]=i;
			// ops[1]=metaDOuts[i].mode;
			// initmsg.operands = ops;
			// writeMsg(initmsg,0);
			// delete[] ops;
			switch(metaDOuts[i]->mode)
			{
				case DOUT_MODE_CONST: lightOnConst(metaDOuts[i]);break;
				case DOUT_MODE_BLINK: lightOnBlinkDO(metaDOuts[i]); break;
				case DOUT_MODE_PWM:lightOnPWM(metaDOuts[i]);break;
				case DOUT_MODE_RANDOM: {
					// msg initmsg;
					// Serial.println("entered actMetaDOut");
					// initmsg.adress = 0;
					// initmsg.code = CODE_INFO;
					// initmsg.opsSize = 3;
					// byte* ops = new byte[initmsg.opsSize];
					// ops[0]=i;
					// ops[1]=0;
					// ops[2]=0;
					// initmsg.operands = ops;
					// writeMsg(initmsg,0);
					// delete[] ops;
					lightOnRandom(metaDOuts[i]);break;}

				default: break;
			}
		}
	}
}

void lightOnConst(metaDOut* tMetaDOut)
{
	//Serial.println("entered lightOnConst");
	if(millis()>tMetaDOut->stepTimout)
	{
	for(byte i=0;i<tMetaDOut->DOutCnt;i++)
	{
	// 	word c=tMetaDOut->dOutArr[i];
	// 	byte dONum=0;
	// 	while(dOuts[dONum]->num!=c &&dONum<dOutNum)
	// 	{
	// 		// Serial.print("num ");
	// 		// Serial.println(segments[segNum].num);
	// 		// Serial.println(c);
	// 		dONum++;
	//
	// 	}
	// 	// Serial.println(dONum);
	// 	if(dONum<dOutNum)
	// 	{
	// 		// Serial.println(dOuts[dONum].pin);
	// 		digitalWrite(dOuts[dONum]->pin,HIGH);
	// 	}
		digitalWrite(dOuts[tMetaDOut->dOutArr[i]]->pin, HIGH);

	}
	for(byte j=0;j<tMetaDOut->DOutRangeCnt;j++)
	{
		for(byte k=0;k<dOutNum;k++)
		{
			if(dOuts[k]->num>=tMetaDOut->range[j].startDOut && dOuts[k]->num<=tMetaDOut->range[j].endDOut)
			{
				digitalWrite(dOuts[k]->pin,HIGH);
			}
		}
	}

	// Serial.print("entered segm ");
		// Serial.println(i);
		// Serial.print("curr pixel ");
		// Serial.println(segments[c].curPixel);
		// Serial.print("pixel cnt ");
		// Serial.println(segments[c].lentSize);

	tMetaDOut->stepTimout=millis()+CHANNEL_TIMEOUT;
	}
	// return tMetaDOut;
}

void lightOnBlinkDO(metaDOut* tMetaDOut)
{
	if(millis()>tMetaDOut->stepTimout)
	{
		for(byte i=0;i<tMetaDOut->DOutCnt;i++)
		{
			// word c=tMetaDOut->dOutArr[i];
			// byte dONum=0;
			// while(dOuts[dONum]->num!=c &&dONum<dOutNum)
			// {
			// // Serial.print("num ");
			// // Serial.println(segments[segNum].num);
			// // Serial.println(c);
			// 	dONum++;
			// }
			// if(dONum<dOutNum)
			// {
			// 	digitalWrite(dOuts[dONum]->pin,tMetaDOut->state);
			// }
			digitalWrite(dOuts[tMetaDOut->dOutArr[i]]->pin, tMetaDOut->state);

		}
		for(byte j=0;j<tMetaDOut->DOutRangeCnt;j++)
		{
			for(byte k=0;k<dOutNum;k++)
			{
				if(dOuts[k]->num>=tMetaDOut->range[j].startDOut && dOuts[k]->num<=tMetaDOut->range[j].endDOut)
				{
					digitalWrite(dOuts[k]->pin,tMetaDOut->state);
				}
			}
		}


	// Serial.print("entered segm ");
		// Serial.println(i);
		// Serial.print("curr pixel ");
		// Serial.println(segments[c].curPixel);
		// Serial.print("pixel cnt ");
		// Serial.println(segments[c].lentSize);

		if(tMetaDOut->state==HIGH)
		{
			tMetaDOut->state=LOW;
		}
		else
		{
			tMetaDOut->state=HIGH;
		}
		tMetaDOut->stepTimout=millis()+STEP_TIMEOUT;
	}
	// return tMetaDOut;
}

void lightOnPWM(metaDOut* tMetaDOut)
{
	if(millis()>tMetaDOut->stepTimout)
	{

			if(tMetaDOut->direction==1)
				tMetaDOut->intens++;
			else
				tMetaDOut->intens--;
			if(tMetaDOut->intens==255)
				tMetaDOut->direction=0;
			if(tMetaDOut->intens==0)
				tMetaDOut->direction=1;
	for(byte i=0;i<tMetaDOut->DOutCnt;i++)
	{
		// word c=tMetaDOut->dOutArr[i];
		// byte dONum=0;
		// while(dOuts[dONum]->num!=c &&dONum<dOutNum)
		// {
		// 	// Serial.print("num ");
		// 	// Serial.println(segments[segNum].num);
		// 	// Serial.println(c);
		// 	dONum++;
		//
		// }
		// if(dONum<dOutNum)
		// {
		// 	if(dOuts[dONum]->pin>12)
		// 		digitalWrite(dOuts[dONum]->pin,HIGH);
		// 	else
		// 		analogWrite(dOuts[dONum]->pin,tMetaDOut->intens);
		if(dOuts[tMetaDOut->dOutArr[i]]->pin>12)
			digitalWrite(dOuts[tMetaDOut->dOutArr[i]]->pin, HIGH);
		else
			analogWrite(dOuts[tMetaDOut->dOutArr[i]]->pin, tMetaDOut->intens);
			// Serial.println(tMetaDOut.intens);
			// Serial.println(dOuts[dONum].pin);
		// }

	}
	for(byte j=0;j<tMetaDOut->DOutRangeCnt;j++)
	{
		for(byte k=0;k<dOutNum;k++)
		{
			if(dOuts[k]->num>=tMetaDOut->range[j].startDOut && dOuts[k]->num<=tMetaDOut->range[j].endDOut)
			{
				if(dOuts[k]->pin>12)
					digitalWrite(dOuts[k]->pin,HIGH);
				else
					analogWrite(dOuts[k]->pin,tMetaDOut->intens);
			}
		}
	}


	// Serial.print("entered segm ");
		// Serial.println(i);
		// Serial.print("curr pixel ");
		// Serial.println(segments[c].curPixel);
		// Serial.print("pixel cnt ");
		// Serial.println(segments[c].lentSize);

	tMetaDOut->stepTimout=millis()+STEP_TIMEOUT_PWM;
	}
	// return tMetaDOut;
}


void lightOnRandom(metaDOut* tMetaDout)
{
	msg initmsg;
	// initmsg.adress = 0;
	// initmsg.code = CODE_INFO;
	// initmsg.opsSize = 5;
	// byte* ops = new byte[initmsg.opsSize];
	// ops[0]=0;
	// ops[1]=0;
	// ops[2]=0;
	// ops[3]=0;
	// ops[4]=tMetaDout.randomizer->maxCnt;
	// initmsg.operands = ops;
	// writeMsg(initmsg,0);
	// delete[] ops;
	if(millis()>tMetaDout->stepTimout)
	{

		for(byte i = 0; i<tMetaDout->randomizer->maxCnt;i++)
		{
			if((checkByteInArr(tMetaDout->randomizer->randomArr[i], tMetaDout->dOutArr, tMetaDout->DOutCnt))|| checkWordInRangeArr(dOuts[tMetaDout->randomizer->randomArr[i]]->num, tMetaDout->range, tMetaDout->DOutRangeCnt))
			{
				digitalWrite(dOuts[tMetaDout->randomizer->randomArr[i]]->pin,LOW);
			}
			// initmsg.adress = 0;
			// initmsg.code = CODE_INFO;
			// initmsg.opsSize = 1;
			// byte* ops = new byte[initmsg.opsSize];
			// ops[0]=tMetaDout.randomizer->randomArr[i];
			// initmsg.operands = ops;
			// writeMsg(initmsg,0);
			// delete[] ops;
			tMetaDout->randomizer->randomArr[i]=random(0,tMetaDout->randomizer->maxCnt);
			if((checkByteInArr(tMetaDout->randomizer->randomArr[i], tMetaDout->dOutArr, tMetaDout->DOutCnt))|| checkWordInRangeArr(dOuts[tMetaDout->randomizer->randomArr[i]]->num, tMetaDout->range, tMetaDout->DOutRangeCnt))
			{
				digitalWrite(dOuts[tMetaDout->randomizer->randomArr[i]]->pin,HIGH);
			}
		}
		tMetaDout->stepTimout = millis() + tMetaDout->randomizer->timeout;
	}
	// return tMetaDout;
}


bool checkWordInArr(word value, word* arr, byte arrSize)
{
	byte i = 0;

	while(i<arrSize&&arr[i]!=value)
	{
		i++;
	}
	if(i<arrSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool checkByteInArr(byte value, byte* arr, byte arrSize)
{
	byte i = 0;

	while(i<arrSize&&arr[i]!=value)
	{
		i++;
	}
	if(i<arrSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool checkWordInRangeArr(word value, DOutRange* arr, byte arrSize)
{
	byte i = 0;
	while(i<arrSize && (!((arr[i].startDOut<=value)&&(arr[i].endDOut>=value))))
	{
		i++;
	}
	if(i<arrSize)
	{
		return true;
	}
	else
	{
		return false;
	}
}


byte findNumInDOutsArr(word value)
{
	byte i=0;
	while(i<dOutNum && dOuts[i]->num != value)
	{
		i++;
	}
	if(i<dOutNum)
	{
		return i;
	}
	else
	{
		return 255;
	}
}

// void actDOut(byte channel)
// {
// 	unsigned long int milli = millis();
// 	for (byte i = 0; i < dOutNum; i++)
// 	{
// 		if (dOutArr[i].channel==channel)
// 		{
// 			dOutArr[i].state = HIGH;
// 			dOutArr[i].channelState = HIGH;
// 			if (dOutArr[i].mode == DOUT_MODE_BLINK)
// 			{
// 				dOutArr[i].stepTimout = milli + 1000L;
// 			}
// 			digitalWrite(dOutArr[i].pin, dOutArr[i].state);
// 		}
// 	}
// }
//
// void deactDOut(byte channel)
// {
// 	for (byte i = 0; i < dOutNum; i++)
// 	{
// 		if (dOutArr[i].channel == channel)
// 		{
// 			dOutArr[i].state = LOW;
// 			dOutArr[i].channelState = LOW;
// 			digitalWrite(dOutArr[i].pin, dOutArr[i].state);
// 			dOutArr[i].stepTimout = 0;
// 		}
// 	}
// }
//
// void checkDOut()
// {
// 	if (actCnlNum > 0)
// 	{
// 		unsigned long milli = millis();
// 		for (byte i = 0; i < dOutNum; i++)
// 		{
// 			if (dOutArr[i].channelState == HIGH)
// 			{
// 				switch (dOutArr[i].mode)
// 				{
// 				case DOUT_MODE_CONST: break;
// 				case DOUT_MODE_BLINK:
// 					if (milli > dOutArr[i].stepTimout)
// 					{
// 						if (dOutArr[i].state == HIGH)
// 							dOutArr[i].state = LOW;
// 						else
// 							dOutArr[i].state = HIGH;
// 						digitalWrite(dOutArr[i].pin, dOutArr[i].state);
// 						dOutArr[i].stepTimout = milli + 1000L;
// 					}
// 					break;
// 				default:
// 					break;
// 				}
// 			}
// 		}
// 	}
// }


void checkChannel()
{
	if(actCnlNum>0)
	{
		unsigned long milli = millis();
		for(byte i=0;i<actCnlNum;i++)
		{
			if(cnlArr[i]->timout<milli)
			{
				byte param[1];
				param[0]=cnlArr[i]->channel;
				deactChannel(param);
			}
		}
		checkMetaDOut();
#if LENTS_USE ==1
		checkLent();
		#endif
	}
}
