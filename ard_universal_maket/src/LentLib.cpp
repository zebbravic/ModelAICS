//
//
//

#include "LentLib.h"

segment* segments;
lent* lents;
byte lentsNum = 0, segmentsNum = 0;

bool addSegment(byte* params)
{
	segment* tSegments = new segment[segmentsNum + 1];
	bool flag=false;
	for (byte i = 0; i < segmentsNum; i++)
	{
		if(segments[i].pin!=params[0])
		{
			tSegments[i] = segments[i];
		}
		else
		{
			flag=true;
			break;
		}
	}
	if(!flag)
	{
	segment tSegment;

	tSegment.pin=params[0];
	tSegment.lentSize=params[1]+1;
	tSegment.curPixel=params[1];
		tSegment.num=params[2];
		// Serial.print("num ");
		// Serial.println(tSegment.num);
	Adafruit_NeoPixel* tLent =  new Adafruit_NeoPixel(tSegment.lentSize,tSegment.pin);
	tSegment.lent=tLent;
	tSegment.lent->begin();
	tSegment.lent->show();
	delete[] segments;
	segments=tSegments;
	tSegments[segmentsNum]=tSegment;
	segmentsNum++;

	// segments[segmentsNum].lent->setPixelColor(1,0,255,0);
	// segments[segmentsNum].lent->show();
	}
	else
	{
		delete[] tSegments;
	}
	return (!flag);
}

bool addLent(byte* params)
{
	lent* tLents = new lent[lentsNum+1];
	bool flag=false;
	for(byte i=0;i<lentsNum;i++)
	{
		if(lents[i].channel!=params[0])
		{
			tLents[i]=lents[i];
		}
		// else
		// {
		// 	flag=true;
		// 	break;
		// }
	}
	if(!flag)
	{
	lent tLent;
	tLent.channel=params[0];
	tLent.mode=params[1];
	tLent.RED=params[2];
	tLent.GREEN=params[3];
	tLent.BLUE=params[4];
	tLent.segCnt=params[5];
	tLent.channelState=false;
	tLent.state=LOW;
	tLent.nextStep=0;
	tLent.direction=1;
	tLent.curIntens=0;
	// for(byte i=0;i<tLent.segCnt;i++)
	// {
	// 	bool flag=false;
	// 	for(byte j=0;j<segmentsNum;i++)
	// 	{
	// 		if(segments[j].num==params[6+i])
	// 		{
	// 			flag=true;
	// 		}
	// 	}
	// 	if(!flag)
	// 		params[5]--;
	//
	// }
	byte* tSegArr = new byte[params[5]];
	for(byte i=0;i<tLent.segCnt;i++)
	{
			tSegArr[i]=params[6+i];

	}
	tLent.segCnt=params[5];
	tLent.segArr=tSegArr;
	// segments[0].lent->setPixelColor(1,0,0,128);
	// segments[0].lent->show();
	// Serial.println(segments[0].lent->numPixels());
	// Serial.println(segments[0].lent->getPixelColor(1));
	//
	// Serial.println(segments[0].lent->getPixelColor(0));
	// Serial.print("added segms ");
	// Serial.println(tLent.segCnt);
	delete[] lents;
	tLents[lentsNum]=tLent;
	lents=tLents;
	lentsNum++;
	}
	else
	{
		delete[] tLents;
	}
	return (!flag);
}


byte actLent(byte channel)
{
	bool flag=false;
	for(byte i=0;i<lentsNum;i++)
	{
		if(lents[i].channel==channel)
		{
			lents[i].channelState=true;
			lents[i].state=HIGH;
			flag=true;
			//Serial.println("found channel");
		}
	}
	if(flag)
	{
		checkLent();
	}
}

byte deactLent(byte channel)
{
	for(byte i=0;i<lentsNum;i++)
	{
		if(lents[i].channel==channel)
		{
			lents[i].channelState=false;
			//Serial.println(lents[i].segCnt);
			for(byte j=0;j<lents[i].segCnt;j++)
			{
				byte c=lents[i].segArr[j];
				byte segNum=0;
				while(segments[segNum].num!=c)
					segNum++;
				for(byte k=0;k<segments[segNum].lentSize;k++)
				{
					segments[segNum].lent->setPixelColor(k,0,0,0);
					//Serial.println(segments[c].lent->getPixelColor(k));
				}
				//segments[segNum].lent->show();
				segments[segNum].curPixel=segments[segNum].lentSize-1;
				//Serial.println(segments[c].curPixel);
			}
			for(byte j=0;j<segmentsNum;j++)
			{
				for(byte k=0;k<3;k++)
				{
					segments[j].lent->show();
				}
			}
			lents[i].curIntens=0;
			lents[i].direction=1;
			lents[i].nextStep=0;
			lents[i].state=LOW;
		}
	}

}

void checkLent()
{
	for(byte i=0;i<lentsNum;i++)
	{
		if(lents[i].channelState)
		{
			switch(lents[i].mode)
			{
				case LENTMODE_ONE_FLOAT: lents[i]=lightOnOneFlow(lents[i]);break;
				case LENTMODE_FILL: lents[i]=lightOnFill(lents[i]); break;
				case LENTMODE_BLINK: lents[i]=lightOnBlink(lents[i]);break;
				case LENTMODE_WAVE: lents[i]=lightOnWave(lents[i]);break;

				default: break;
			}
		}
	}
	for(byte i=0;i<segmentsNum;i++)
	{
		segments[i].lent->show();
	}
}


lent lightOnOneFlow(lent tLent)
{
	if(millis()>tLent.nextStep)
	{
	for(byte i=0;i<tLent.segCnt;i++)
	{
		byte c=tLent.segArr[i];
		byte segNum=0;

		while(segments[segNum].num!=c &&segNum<segmentsNum)
		{
			segNum++;

		}
		if(segNum<segmentsNum)
		{
			// for(byte j=0;j*4<segments[segNum].lentSize;j++)
			// {
			// 	if(j*4+segments[segNum].curPixel<segments[segNum].lentSize)
			// 	{
			// 		segments[segNum].lent->setPixelColor(j*4+segments[segNum].curPixel,0,0,0);
			// 	}
			// }
			segments[segNum].curPixel++;
			if(segments[segNum].curPixel>=3)
			{
				segments[segNum].curPixel=0;
			}
		for(byte j=0;j*4<segments[segNum].lentSize;j++)
		{
			if(j*4+segments[segNum].curPixel<segments[segNum].lentSize)
			{
				segments[segNum].lent->setPixelColor(j*4+segments[segNum].curPixel,tLent.RED,tLent.GREEN,tLent.BLUE);
			}
			if((j*4+segments[segNum].curPixel-1<segments[segNum].lentSize)&&(j*4+segments[segNum].curPixel-1>-1))
			{
				segments[segNum].lent->setPixelColor(j*4+segments[segNum].curPixel-1,tLent.RED*0.6,tLent.GREEN*0.6,tLent.BLUE*0.6);
			}
			if((j*4+segments[segNum].curPixel-2<segments[segNum].lentSize)&&(j*4+segments[segNum].curPixel-2>-1))
			{
				segments[segNum].lent->setPixelColor(j*4+segments[segNum].curPixel-2,tLent.RED*0.3,tLent.GREEN*0.3,tLent.BLUE*0.3);
			}
			if((j*4+segments[segNum].curPixel-3<segments[segNum].lentSize)&&(j*4+segments[segNum].curPixel-3>-1))
			{
				segments[segNum].lent->setPixelColor(j*4+segments[segNum].curPixel-3,0,0,0);
			}
		}//segments[c].lent->setPixelColor(segments[c].curPixel,127,tLent.GREEN,tLent.BLUE);
		// segments[segNum].lent->show();
	}
	}
	// for(byte i=0;i<segmentsNum;i++)
	// {
	// 	segments[i].lent->show();
	// }
	tLent.nextStep=millis()+STEP_TIMEOUT_FLOWPIXEL;
	}
	return tLent;
	// if(millis()>tLent.nextStep)
	// {
	// for(byte i=0;i<tLent.segCnt;i++)
	// {
	// 	byte c=tLent.segArr[i];
	// 	byte segNum=0;
	//
	// 	while(segments[segNum].num!=c &&segNum<segmentsNum)
	// 	{
	// 		// Serial.print("num ");
	// 		// Serial.println(segments[segNum].num);
	// 		// Serial.println(c);
	// 		segNum++;
	//
	// 	}
	// 	// Serial.println(segNum);
	// 	if(segNum<segmentsNum)
	// 	{
	// 	segments[segNum].lent->setPixelColor(segments[segNum].curPixel,0,0,0);
	// 	segments[segNum].curPixel++;
	// 	if(segments[segNum].curPixel>=segments[segNum].lentSize)
	// 	{
	// 		segments[segNum].curPixel=0;
	// 	}
	//
	// 	segments[segNum].lent->setPixelColor(segments[segNum].curPixel,tLent.RED,tLent.GREEN,tLent.BLUE);
	// 	//segments[c].lent->setPixelColor(segments[c].curPixel,127,tLent.GREEN,tLent.BLUE);
	// 	segments[segNum].lent->show();
	// }// Serial.print("entered segm ");
	// 	// Serial.println(i);
	// 	// Serial.print("curr pixel ");
	// 	// Serial.println(segments[c].curPixel);
	// 	// Serial.print("pixel cnt ");
	// 	// Serial.println(segments[c].lentSize);
	// }
	// tLent.nextStep=millis()+STEP_TIMEOUT_PWM;
	// }
	// return tLent;
}


lent lightOnFill(lent tLent)
{
	if(millis()>tLent.nextStep)
	{
		for(byte i=0;i<tLent.segCnt;i++)
		{
			byte c=tLent.segArr[i];
			byte segNum=0;
			while(segments[segNum].num!=c)
				segNum++;
			for(byte j=0;j<segments[segNum].lentSize;j++)
			{
				segments[segNum].lent->setPixelColor(j,tLent.RED,tLent.GREEN,tLent.BLUE);
			}
			// segments[segNum].lent->show();
		}
		// for(byte i=0;i<segmentsNum;i++)
		// {
		// 	segments[i].lent->show();
		// }
		tLent.nextStep=millis()+CHANNEL_TIMEOUT;
	}
	return tLent;
}

lent lightOnBlink(lent tLent)
{
	if(millis()>tLent.nextStep)
	{
		for(byte i=0;i<tLent.segCnt;i++)
		{
			byte c=tLent.segArr[i];
			byte segNum=0;

			while(segments[segNum].num!=c &&segNum<segmentsNum)
			{
				segNum++;

			}
			if(segNum<segmentsNum)
			{
			for(byte j=0;j<segments[segNum].lentSize;j++)
			{
				if(tLent.state==HIGH)
				{
					segments[segNum].lent->setPixelColor(j,tLent.RED,tLent.GREEN,tLent.BLUE);
				}
				if(tLent.state==LOW)
				{
					segments[segNum].lent->setPixelColor(j,0,0,0);
				}
				// Serial.println(segments[segNum].lent->getPixelColor(j));
			}
		}

			// segments[segNum].lent->show();
		}
		// for(byte i=0;i<segmentsNum;i++)
		// {
		// 	segments[i].lent->show();
		// }
		tLent.nextStep=millis()+STEP_TIMEOUT;
		if(tLent.state==HIGH)
		{

			tLent.state=LOW;
			// Serial.println("changed state to LOW");
		}
		else
		{
			tLent.state=HIGH;
			// Serial.println("changed state to HIGH");
		}

	}
	return tLent;
}
lent lightOnWave(lent tLent)
{
	if(millis()>tLent.nextStep)
	{
		if(tLent.direction==1)
			tLent.curIntens++;
		else
			tLent.curIntens--;
		if(tLent.curIntens==128)
			tLent.direction=0;
		if(tLent.curIntens==0)
			tLent.direction=1;
		byte tRed, tGreen, tBlue;
		// Serial.println(tLent.curIntens);
		tRed=(tLent.RED*tLent.curIntens)/255;
		tGreen=(tLent.GREEN*tLent.curIntens)/255;
		tBlue=(tLent.BLUE*tLent.curIntens)/255;
		// Serial.println(tRed);
		// Serial.println(tBlue);
		// Serial.println(tGreen);
		for(byte i=0;i<tLent.segCnt;i++)
		{

			byte c=tLent.segArr[i];
			byte segNum=0;
			while(segments[segNum].num!=c)
				segNum++;
			for(byte j=0;j<segments[segNum].lentSize;j++)
			{
					segments[segNum].lent->setPixelColor(j,tRed,tGreen,tBlue);

			}
			// segments[segNum].lent->show();
		}
		// for(byte i=0;i<segmentsNum;i++)
		// {
		// 	segments[i].lent->show();
		// }
		tLent.nextStep=millis()+STEP_TIMEOUT_PWM;
		//tLent.state=!tLent.state;
	}
	return tLent;
}

byte getSegLent()
{
	return segmentsNum+lentsNum;
}
