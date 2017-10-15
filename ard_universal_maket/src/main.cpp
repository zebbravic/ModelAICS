#include "LentLib.h"
#include "protoLib.h"

byte addressCur = 4; //адрес контроллера, меняется в зависимости от зашиваемого контроллера.
byte modeCur = MODE_BYTES;
unsigned long nextRequest=0;
unsigned long nextLentDown=0;
void setup()
{
	initCntrl(modeCur,addressCur);
}

void loop()
{
	switch (checkState()) {
		case WORKMODE_BOOT: if(millis()>nextRequest)
		{
			dataRequest();
			nextRequest=millis()+3L*1000L;
		}
		break;
		case WORKMODE_INIT: if(millis()>10000L) {msg mess; mess.adress=5; mess.code=2; writeMsg(mess, 0);}break;
		default: break;
	}
	if(Serial.available()>1)
	{
		msg rcvdMsg = parseBuffer();
		parseAdressMsg(rcvdMsg);
		if(rcvdMsg.opsSize>0)
		{
			delete[] rcvdMsg.operands;
		}
	}
	if(getActCnlNum()==0 && nextLentDown<millis()&&checkState()==WORKMODE_BOOT)
	{
		if(addressCur==1)
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
		nextLentDown=millis()+10000L;
	}
	checkChannel();
	//lightOnLents();
	//delay(500);
  /* add main program code here */

}
