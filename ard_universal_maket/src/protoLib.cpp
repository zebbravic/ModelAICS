//
//
//

#include "protoLib.h"

word dOutNum=0, metaDOutNum=0, actCnlNum=0, state=WORKMODE_BOOT;
DOut* dOuts;
metaDOut* metaDOuts;
ActCnl* cnlArr;
unsigned long curSyncTimeout=0;
byte syncSize, syncTimeout,curSessSize=0;
byte mode, address;




void writeMsg(msg message, byte port)
{
	if(port==0)
	{
		Serial.write(message.adress);
		Serial.write(message.code);
		//Serial.write(message.opsSize);

		for (int i = 0; i < message.opsSize; i++)
		{
			Serial.write(message.operands[i]);
		}
	}
	else
	{
		Serial3.write(message.adress);
		Serial3.write(message.code);
		//Serial.write(message.opsSize);

		for (int i = 0; i < message.opsSize; i++)
		{
			Serial3.write(message.operands[i]);
		}
	}
}

void initCntrl(byte tmode, byte taddress)
{
	if(taddress==1)
	{
		segment* seg;
		seg=new segment[29];
  seg[0].lent=new Adafruit_NeoPixel(3,2);
  seg[1].lent=new Adafruit_NeoPixel(3,3);
  seg[2].lent=new Adafruit_NeoPixel(11,4);
  seg[3].lent=new Adafruit_NeoPixel(23,5);
  seg[4].lent=new Adafruit_NeoPixel(21,6);
  seg[5].lent=new Adafruit_NeoPixel(3,7);
  seg[6].lent=new Adafruit_NeoPixel(2,8);
  seg[7].lent=new Adafruit_NeoPixel(19,9);
  seg[8].lent=new Adafruit_NeoPixel(8,10);
  seg[9].lent=new Adafruit_NeoPixel(14,11);
  seg[10].lent=new Adafruit_NeoPixel(10,12);
  seg[11].lent=new Adafruit_NeoPixel(10,16);
  seg[12].lent=new Adafruit_NeoPixel(8,17);
  seg[13].lent=new Adafruit_NeoPixel(66,18);
  seg[14].lent=new Adafruit_NeoPixel(5,19);
  seg[15].lent=new Adafruit_NeoPixel(7,20);
  seg[16].lent=new Adafruit_NeoPixel(57,21);
  seg[17].lent=new Adafruit_NeoPixel(33,22);
  seg[18].lent=new Adafruit_NeoPixel(17,23);
  seg[19].lent=new Adafruit_NeoPixel(47,24);
  seg[20].lent=new Adafruit_NeoPixel(19,25);
  seg[21].lent=new Adafruit_NeoPixel(11,26);
  seg[22].lent=new Adafruit_NeoPixel(16,27);
  seg[23].lent=new Adafruit_NeoPixel(3,28);
  seg[24].lent=new Adafruit_NeoPixel(30,29);
	seg[25].lent=new Adafruit_NeoPixel(12,30);
  seg[26].lent=new Adafruit_NeoPixel(12,31);
  seg[27].lent=new Adafruit_NeoPixel(1,32);
  seg[28].lent=new Adafruit_NeoPixel(50,33);
  for(byte j=0;j<3;j++)
  {
  for(byte i=0;i<29;i++)
  {
        seg[i].lent->begin();
        seg[i].lent->show();
  }
  }
	for(byte i=0;i<29;i++)
  {
        delete seg[i].lent;
  }
	delete[] seg;
	}
	else
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
	msg initmsg;
	mode=tmode;
	address=taddress;
	initmsg.adress = 0+mode;
	initmsg.code = CODE_REBOOTED + mode;
	initmsg.opsSize = 1;
	byte* ops = new byte[initmsg.opsSize];
	ops[0] = address + mode;
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
	initmsg.adress = 0+mode;
	initmsg.code = CODE_NEED_DATA + mode;
	initmsg.opsSize = 1;
	byte* ops = new byte[initmsg.opsSize];
	ops[0] = address + mode;
	initmsg.operands = ops;
	writeMsg(initmsg,0);
	delete[] ops;
}
msg parseBuffer()
{
	//Serial.println("Entered parseBuffer");
	msg buffMessage;
	// Serial.println(Serial.available());
	if (Serial.available() > 1 )
	{
		buffMessage.adress = Serial.read()-mode;
		buffMessage.code = Serial.read()-mode;
		switch (buffMessage.code)
		{
		case CODE_REBOOTED: buffMessage.opsSize = 1;
			break;
		case CODE_NEED_DATA: buffMessage.opsSize = 1;
			break;
		 case CODE_INIT_SYNC: buffMessage.opsSize = 2;
		 	break;
		case CODE_SYNC: buffMessage.opsSize=1;
			break;
		case CODE_END_SYNC: buffMessage.opsSize=1;
			break;
		case CODE_BLOCK_ERR: buffMessage.opsSize=1;
			break;
		case CODE_INIT_DOUT: buffMessage.opsSize = 3;
			break;
		case CODE_INIT_SEGM: buffMessage.opsSize=4;
			break;
		case CODE_ACT_CNL: buffMessage.opsSize = 2;
			break;
		case CODE_DEACT_CNL: buffMessage.opsSize = 2;
			break;
		case CODE_DEACT_ALL: buffMessage.opsSize = 0;
			break;
		case CODE_DEACT_LAST: buffMessage.opsSize = 0;
			break;
		case CODE_INIT_LENT: buffMessage.opsSize = 7;
			break;
		case CODE_INIT_METADOUT:
			buffMessage.opsSize=4;
			break;
		default:
			buffMessage.opsSize = 0;
			break;
		}
		// Serial.println();
		// Serial.println(buffMessage.opsSize);
		if(buffMessage.opsSize>0)
		{
			while (Serial.available()<buffMessage.opsSize)
			{
				true;
			}
		byte i = 0;
		byte* ops = new byte[buffMessage.opsSize];
		buffMessage.operands = ops;
		while (i != buffMessage.opsSize)
		{
			buffMessage.operands[i] = Serial.read() - mode;
			// Serial.println(buffMessage.operands[i]);
			i++;
		}
		if(buffMessage.code==CODE_INIT_LENT)
		{
			byte* opsLent = new byte[buffMessage.opsSize+buffMessage.operands[6]];
			while(Serial.available()<buffMessage.operands[6])
				{
					true;
				}
				byte j=0;
				while(j!=buffMessage.opsSize*2)
				{
					opsLent[j]=ops[j];
					j++;
				}
				buffMessage.opsSize+=buffMessage.operands[6];
				while(j!=buffMessage.opsSize*2)
				{
					opsLent[j]=Serial.read()-mode;
					j++;
				}
				delete[] buffMessage.operands;
				buffMessage.operands=opsLent;

			}
			if(buffMessage.code==CODE_INIT_METADOUT)
			{
				byte* opsMeta = new byte[buffMessage.opsSize+buffMessage.operands[3]];
				while(Serial.available()<buffMessage.operands[3])
				{
					true;
				}
				byte j=0;
				while(j!=buffMessage.opsSize*2)
				{
					opsMeta[j]=ops[j];
					j++;
				}
				buffMessage.opsSize+=buffMessage.operands[3];
				while(j!=buffMessage.opsSize*2)
				{
					opsMeta[j]=Serial.read()-mode;
					j++;
				}
				delete[] buffMessage.operands;
				buffMessage.operands=opsMeta;

			}
		}
		return buffMessage;
	}
}

void parseAdressMsg(msg message)
{
	if (message.adress == address||message.adress==255)
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
	switch (rcvMsg.code)
	{
	case CODE_INIT_DOUT:
		if(state==WORKMODE_INIT){ if(addDOut(rcvMsg.operands)) sync();}
		break;
	case CODE_INIT_SEGM:
		if(state==WORKMODE_INIT){ if(addSegment(rcvMsg.operands)) sync();}
		break;
	case CODE_INIT_LENT:
		if(state==WORKMODE_INIT){ if(addLent(rcvMsg.operands)) sync();}
		break;
	case CODE_INIT_SYNC:
		if(state==WORKMODE_BOOT) beginInit(rcvMsg.operands);
		break;
	case CODE_INIT_METADOUT:
		if(state==WORKMODE_INIT) { if(addMetaDOut(rcvMsg.operands)) sync();}
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
	state=WORKMODE_INIT;
	syncSize=params[0];
	syncTimeout=params[1];
	// Serial.println("entered init");
}

void endInit(byte* params)
{
	if(params[0]==getSegLent()+dOutNum+metaDOutNum)
	{
		msg initmsg;
		initmsg.adress = 0+mode;
		initmsg.code = CODE_END_SYNC+mode;
		initmsg.opsSize = 2;
		byte* ops = new byte[initmsg.opsSize];
		ops[1] = params[0]+mode;
		ops[0]=address+mode;
		initmsg.operands = ops;
		writeMsg(initmsg,0);
		delete[] ops;
		state=WORKMODE_NORMAL;
		}
	else
	{
		sendSyncErr();
	}
}

void sync()
{
	if(syncSize>0)
	{
		curSessSize++;
		if(curSessSize==syncSize)
		{
			msg initmsg;
			initmsg.adress = 0+mode;
			initmsg.code = CODE_SYNC+mode;
			initmsg.opsSize = 1;
			byte* ops = new byte[initmsg.opsSize];
			ops[0]=address+mode;
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
	initmsg.adress = 0+mode;
	initmsg.code = CODE_BLOCK_ERR+mode;
	initmsg.opsSize = 1;
	byte* ops = new byte[initmsg.opsSize];
	ops[0]=address+mode;
	initmsg.operands = ops;
	writeMsg(initmsg,0);
	delete[] ops;
}


bool addDOut(byte* params)
{
	DOut* tDouts = new DOut[dOutNum + 1];
	bool flag=false;
	word num=word(params[1],params[2]);
	for (byte i = 0; i < dOutNum; i++)
	{
		tDouts[i] = dOuts[i];
		if(dOuts[i].num==num)
		{
			flag=true;
			break;
		}
	}
	if(!flag)
	{
		DOut tDOut;
		tDOut.pin = params[0];
		tDOut.num = num;
		tDouts[dOutNum] = tDOut;
		delete[] dOuts;
		dOutNum++;
		dOuts = tDouts;
		pinMode(tDOut.pin, OUTPUT);
	}
	else
	{
		delete[] tDouts;
	}
	return (!flag);
}


bool addMetaDOut(byte* params)
{
	metaDOut* tMetaDOuts = new metaDOut[metaDOutNum + 1];
	bool flag=false;
	for (byte i = 0; i < metaDOutNum; i++)
	{
		tMetaDOuts[i] = metaDOuts[i];
		// if(metaDOuts.num==params[1])
		// {
		// 	flag=true;
		// 	break;
		// }
	}
	if(!flag)
	{
		metaDOut tMetaDOut;
		tMetaDOut.channel=word(params[0],params[1]);
		tMetaDOut.mode=params[2];
		tMetaDOut.DOutCnt=params[3];
		tMetaDOut.channelState=false;
		tMetaDOut.stepTimout=0;
		tMetaDOut.direction=1;
		tMetaDOut.intens=0;
		// Serial.println(params[0]);
		// Serial.println(params[1]);
		// Serial.println(params[2]);

		word* tDoutArr = new word[params[3]];
		for(byte i=0;i<tMetaDOut.DOutCnt;i++)
		{
				tDoutArr[i]=word(params[4+i*2],params[5+i*2]);

		}
		tMetaDOut.DOutCnt=params[3];
		tMetaDOut.dOutArr=tDoutArr;
		tMetaDOuts[metaDOutNum] = tMetaDOut;
		delete[] metaDOuts;
		metaDOutNum++;
		metaDOuts = tMetaDOuts;
	}

	else
	{
		delete[] tMetaDOuts;
	}
	return (!flag);
}



void actChannel(byte* params)
{
	word channel=word(params[0],params[1]);
	//Serial.println(channel);
	actMetaDOut(channel);
	unsigned long int milli=millis();
	actLent(channel);
	bool flag=false;
	ActCnl* tCnlArr = new ActCnl[actCnlNum + 1];
	for (byte i = 0; i < actCnlNum; i++)
	{
		if(cnlArr[i].channel!=channel)
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

		ActCnl tCnl;
		tCnl.channel = channel;
		tCnl.timout = milli + CHANNEL_TIMEOUT;
		// tCnl.timout = milli + 30L * 1000L;
		// Serial.println(milli);
		// Serial.println(tCnl.timout);
		tCnlArr[actCnlNum] = tCnl;
		delete[] cnlArr;
		cnlArr = tCnlArr;
		actCnlNum++;
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
		deactLent(channel);
		//Serial.println(channel);
		bool flag=false;
		ActCnl* tCnlArr = new ActCnl[actCnlNum - 1];
		for (byte i = 0; i < actCnlNum; i++)
		{
			if (cnlArr[i].channel==channel)
				flag=true;
			if(flag==true)
			{
				if(cnlArr[i].channel!=channel)
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
		byte params[2]={highByte(cnlArr[actCnlNum-1].channel),lowByte(cnlArr[actCnlNum-1].channel)};
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
	for(byte i=0;i<metaDOutNum;i++)
	{
		if(metaDOuts[i].channel==channel)
		{
			metaDOuts[i].channelState=true;
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
		if(metaDOuts[i].channel==channel)
		{
			metaDOuts[i].channelState=false;
			//Serial.println(lents[i].segCnt);
			for(byte j=0;j<metaDOuts[i].DOutCnt;j++)
			{
				byte c=metaDOuts[i].dOutArr[j];
				byte dONum=0;
				while(dOuts[dONum].num!=c&&dONum<dOutNum)
					dONum++;
				if(dONum<dOutNum)
				{
					digitalWrite(dOuts[dONum].pin,LOW);
				}
			}
			metaDOuts[i].stepTimout=0;
			metaDOuts[i].intens=0;
			metaDOuts[i].direction=1;
		}
	}

}

void checkMetaDOut()
{
	// Serial.println("entered checkMetaDOut");
	for(byte i=0;i<metaDOutNum;i++)
	{
		if(metaDOuts[i].channelState)
		{
			switch(metaDOuts[i].mode)
			{
				case DOUT_MODE_CONST: metaDOuts[i]=lightOnConst(metaDOuts[i]);break;
				case DOUT_MODE_BLINK: metaDOuts[i]=lightOnBlinkDO(metaDOuts[i]); break;
				case DOUT_MODE_PWM: metaDOuts[i]=lightOnPWM(metaDOuts[i]);break;

				default: break;
			}
		}
	}
}

metaDOut lightOnConst(metaDOut tMetaDOut)
{
	//Serial.println("entered lightOnConst");
	if(millis()>tMetaDOut.stepTimout)
	{
	for(byte i=0;i<tMetaDOut.DOutCnt;i++)
	{
		byte c=tMetaDOut.dOutArr[i];
		byte dONum=0;
		while(dOuts[dONum].num!=c &&dONum<dOutNum)
		{
			// Serial.print("num ");
			// Serial.println(segments[segNum].num);
			// Serial.println(c);
			dONum++;

		}
		// Serial.println(dONum);
		if(dONum<dOutNum)
		{
			// Serial.println(dOuts[dONum].pin);
			digitalWrite(dOuts[dONum].pin,HIGH);
		}

	}// Serial.print("entered segm ");
		// Serial.println(i);
		// Serial.print("curr pixel ");
		// Serial.println(segments[c].curPixel);
		// Serial.print("pixel cnt ");
		// Serial.println(segments[c].lentSize);

	tMetaDOut.stepTimout=millis()+CHANNEL_TIMEOUT;
	}
	return tMetaDOut;
}

metaDOut lightOnBlinkDO(metaDOut tMetaDOut)
{
	if(millis()>tMetaDOut.stepTimout)
	{
	for(byte i=0;i<tMetaDOut.DOutCnt;i++)
	{
		byte c=tMetaDOut.dOutArr[i];
		byte dONum=0;
		while(dOuts[dONum].num!=c &&dONum<dOutNum)
		{
			// Serial.print("num ");
			// Serial.println(segments[segNum].num);
			// Serial.println(c);
			dONum++;

		}
		if(dONum<dOutNum)
		{
			digitalWrite(dOuts[dONum].pin,tMetaDOut.state);

		}

	}// Serial.print("entered segm ");
		// Serial.println(i);
		// Serial.print("curr pixel ");
		// Serial.println(segments[c].curPixel);
		// Serial.print("pixel cnt ");
		// Serial.println(segments[c].lentSize);

	if(tMetaDOut.state==HIGH)
	{
		tMetaDOut.state=LOW;
	}
	else
	{
		tMetaDOut.state=HIGH;
	}
	tMetaDOut.stepTimout=millis()+STEP_TIMEOUT;
	}
	return tMetaDOut;
}

metaDOut lightOnPWM(metaDOut tMetaDOut)
{
	if(millis()>tMetaDOut.stepTimout)
	{

			if(tMetaDOut.direction==1)
				tMetaDOut.intens++;
			else
				tMetaDOut.intens--;
			if(tMetaDOut.intens==255)
				tMetaDOut.direction=0;
			if(tMetaDOut.intens==0)
				tMetaDOut.direction=1;
	for(byte i=0;i<tMetaDOut.DOutCnt;i++)
	{
		byte c=tMetaDOut.dOutArr[i];
		byte dONum=0;
		while(dOuts[dONum].num!=c &&dONum<dOutNum)
		{
			// Serial.print("num ");
			// Serial.println(segments[segNum].num);
			// Serial.println(c);
			dONum++;

		}
		if(dONum<dOutNum)
		{
			if(dOuts[dONum].pin>12)
				digitalWrite(dOuts[dONum].pin,HIGH);
			else
				analogWrite(dOuts[dONum].pin,tMetaDOut.intens);
			// Serial.println(tMetaDOut.intens);
			// Serial.println(dOuts[dONum].pin);
		}

	}// Serial.print("entered segm ");
		// Serial.println(i);
		// Serial.print("curr pixel ");
		// Serial.println(segments[c].curPixel);
		// Serial.print("pixel cnt ");
		// Serial.println(segments[c].lentSize);

	tMetaDOut.stepTimout=millis()+STEP_TIMEOUT_PWM;
	}
	return tMetaDOut;
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
			if(cnlArr[i].timout<milli)
			{
				byte param[1];
				param[0]=cnlArr[i].channel;
				deactChannel(param);
			}
		}
		checkMetaDOut();
		checkLent();
	}
}
