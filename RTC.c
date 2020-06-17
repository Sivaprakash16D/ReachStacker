#include <p30f6012A.h>
#include <string.h>
#include <stdlib.h>
#include<stdio.h>
//#include "Disp_LCD.h"
//#include "General_functions.h"


#define	RTCSDA				_LATC13
#define RTCSDA_PORT			_RC13
#define	RTCSCL				_LATC14

#define	RTCSDA_Direction	_TRISC13
#define	RTCSCL_Direction	_TRISC14

//----------------------------------------------------------------

#define		WRITE_C		0xde 		//Command for write
#define		READ_C		0xdf		//Command for read
#define		ACK_READ	0xdf		//Command to get acknowledge

//----------------------------------------------------------------

#define HIGH	1
#define LOW		0
#define WRITE	0
#define READ	1
#define ADDRESS_SECOND	0x00
#define ADDRESS_MINUTE	0x01
#define ADDRESS_HOUR	0x02

#define ADDRESS_DATE	0x03
#define ADDRESS_MONTH	0x04
#define ADDRESS_YEAR	0x05
#define ADDRESS_WEEKDAY	0x06

#define ADDRESS_STATUS  0x07
#define RTC_WRITE_EN	0x10
#define ADDRESS_RTCRAM	0x12

#define TOP    1
#define BOTTOM 2

struct struct_rtc
{
	char Hour;
	char Minute;
	char Second;
	char Date;
	char Month;
	int Year;
	char WeekDay;
    char AMPM;	
    int  HoursNMinute;
    int  DateNMonth;
    int  Seconds;
};

struct struct_rtc RTC;
extern char LcdDispBuff[40];
extern unsigned char MenuKey;
extern unsigned char EnterKey;
extern unsigned char UpKey;
extern unsigned char LeftKey;	

void Init_SCL_As_Output(void)
{

	RTCSCL_Direction = 0;			//Set Clock as output
}


void Set_SDA_As_Output(void)
{

	RTCSDA_Direction = 0;			//Set as output
}


/* Sets input mode to SDA line */
void Set_SDA_As_Input(void)
{
	
	RTCSDA_Direction = 1;			//Set as input
}


void IICClockDelay(void)
{
	char	a;
	for(a=0;a<60;a++);				// increase 3 times for 30mhz
}


void StartBit(void)
{
	Set_SDA_As_Output();

	RTCSCL = 1;
	IICClockDelay();

	RTCSDA	= 1;
	IICClockDelay();
	RTCSDA	= 0;
	IICClockDelay();

	RTCSCL = 0;		
	IICClockDelay();
}

/* Get acknowledge from slave */
char SlaveAck(void)
{
	char	a;
	Set_SDA_As_Output();
	IICClockDelay();

	RTCSCL	= 0;
	IICClockDelay();

	RTCSDA	= 1;
	IICClockDelay();

	RTCSCL = 1;
	IICClockDelay();

	Set_SDA_As_Input();

	IICClockDelay();
	a   = RTCSDA_PORT;
	IICClockDelay();

	RTCSCL = 0;
	IICClockDelay();

	return(a);
}


/* No acknowledge from slave */
void SlaveNoAck(void)
{
	Set_SDA_As_Output();
	RTCSDA	= 1;
	IICClockDelay();
	RTCSCL	= 1;
	IICClockDelay();
	RTCSCL = 0;
	IICClockDelay();
}


/* Send One Byte to RTc */
void SendOneByteIIC(unsigned char d)
{
	char	a;
	Set_SDA_As_Output();

	for(a=0;a<8;a++)		//send 8 bits 
	{	
		RTCSCL = 0;			//clear SCL 
		IICClockDelay();		//delay

		if(d & (0x80 >> a))
		{
			RTCSDA = 1;		//set SDA 
		}
		else
		{
			RTCSDA = 0;		//clear SDA
		}

		IICClockDelay();				

		RTCSCL	= 1;			//set SCL
		IICClockDelay();
	}
}


void StopBit(void)
{
	Set_SDA_As_Output();

	RTCSDA	= 0;
	IICClockDelay();
	RTCSCL	= 1;
	IICClockDelay();

	RTCSDA = 1;		
}


void PollAck(void)
{
	char	a;
	for(a=0;a<0x40;a++)
	{
		StartBit();			//Give start bit
		SendOneByteIIC(ACK_READ);	//Send the read Acknowledgement command
		if(SlaveAck())			//Get the acknowledge from slave
		{	//acknowledge received
			break;
		}
	}
}


void WriteByteRTC(unsigned char Address, unsigned char Data)
{
//	WriteByteRTC(0x3F,0x06);
	StartBit();						//Give start bit
	SendOneByteIIC(WRITE_C);		//Send the write command
	SlaveAck();						//Get the acknowledge from slave
//	SendOneByteIIC(0x00);			//16 bit address
//	SlaveAck();	
	SendOneByteIIC(Address);		//Send the address
	SlaveAck();						//Get the acknowledge from slave
	SendOneByteIIC(Data);			//Send the Data
	SlaveAck();						//Get the acknowledge from slave
	StopBit();						//Give stop bit
	PollAck();
}


void WriteToRTC_Time()
{
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);		
	WriteByteRTC(ADDRESS_SECOND,RTC.Second); 	// write the Second value	
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);
	WriteByteRTC(ADDRESS_MINUTE,RTC.Minute); 	// write the Minute value	
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);
	WriteByteRTC(ADDRESS_HOUR,RTC.Hour); 		// write the Hour
}


void WriteToRTC_Date()
{
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);
	WriteByteRTC((char)ADDRESS_DATE,(char)(RTC.Date));		// write the Date	
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);		
	WriteByteRTC(ADDRESS_WEEKDAY,RTC.WeekDay); 	// write the Weekday(sunday-staturday)	
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);
	WriteByteRTC((char)ADDRESS_MONTH,(char)(RTC.Month|0x80)); // write the Month	
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);
	WriteByteRTC((char)ADDRESS_YEAR,(char)(RTC.Year)); 		// write the year
}

void WriteToRtcData()
{
 //   Set_SCL_As_Output();
	WriteByteRTC(ADDRESS_STATUS,RTC_WRITE_EN);
	WriteByteRTC(ADDRESS_RTCRAM,'X'); // write the Second value
}

char AsciitoBcd(char *Buff)
{
	return((char)(Buff[1]-48+(((Buff[0]-48)<<4)&0xf0)));
}


/*void SetClock()
{
  //  char * Sbuff;


    Sbuff[0] = Sbuff[1] = '0';
    Sbuff[2] = Sbuff[3] = '0';
	Sbuff[4] = Sbuff[5] = '0';

	WriteByteRTC(0x3F,0x06);
	RTC.Hour  = AsciitoBcd(Sbuff);
	RTC.Hour |= 0x80;				// setting as 24 hr format
	RTC.Minute= AsciitoBcd(Sbuff+2);
	RTC.Second= AsciitoBcd(Sbuff+4);
	WriteToRTC_Time();
}*/


/* Read One Byte (bit by bit) from RTC */
unsigned char ReadOneByteIIC()
{
	char	a;
	unsigned char b;

	b=0;
	Set_SDA_As_Input();

	for(a=0;a<8;a++)
	{
		RTCSCL = 1;
		IICClockDelay();

		if(RTCSDA_PORT)
		{
			b |= (0x80 >> a);
		}

		IICClockDelay();
		RTCSCL	= 0;
		IICClockDelay();
	}
	return(b);
}


/* Read Byte from RTC */
unsigned char ReadByteRTC(unsigned char Address)
{
	unsigned char	a;
									//Write Address
	StartBit();						//Give start bit
	SendOneByteIIC(WRITE_C);		//Send the write command
	SlaveAck();						//Get the acknowledge from slave
//	SendOneByteIIC(0x00);			//16 bit address
//	SlaveAck();	
	SendOneByteIIC(Address);		//Send the address
	SlaveAck();						//Get the acknowledge from slave
	
									//Read a byte
	StartBit();						//Give start bit
	SendOneByteIIC(READ_C);			//Send the read command
	SlaveAck();						//Get the acknowledge from slave
	a=ReadOneByteIIC();				//Read the byte
	SlaveNoAck();
	StopBit();						//Give stop bit
	return (a);
}


char BcdtoBinary(char BCDValue)
{
	char Buff[3];

	Buff[2]='\0';
	Buff[0]= (char)(((BCDValue>>4)&0x0f)+48);
	Buff[1]= (char)((BCDValue&0x0f)+48);
	return((char)(atoi(Buff)));
}

char BinaryToBCD(char Value)
{
	char Digi1,Digi2,BCDValue;

	Digi2 = Value / 10;
	Digi1 = Value - Digi2 * 10;
	BCDValue = Digi2 << 4 | Digi1;
	return((char)BCDValue);
}

 
void SetDate(unsigned char Bdate, unsigned char Bmonth,unsigned char Byear)		
{
	WriteByteRTC(0x3F,0x06);
   	RTC.Date	= Bdate;
	RTC.Month	= Bmonth;
	RTC.Year	= Byear;
	WriteToRTC_Date();
}


void SetTime(unsigned char BHour,unsigned char BMin,unsigned char BSec)
{
 	WriteByteRTC(0x3F,0x06);
	RTC.Hour  = BHour;
	RTC.Hour |= 0x80;				// setting as 24 hr format
	RTC.Minute= BMin;
	RTC.Second= BSec;
	WriteToRTC_Time();
}

unsigned char* ReadClock24Hr()
{
   	RTC.Second	= ReadByteRTC(ADDRESS_SECOND);
   	RTC.Minute	= ReadByteRTC((ADDRESS_MINUTE));
   	RTC.Hour	= ReadByteRTC((ADDRESS_HOUR));
   	RTC.Second 	= BcdtoBinary((char)(RTC.Second&0x7f));
   	RTC.Minute	= BcdtoBinary((char)(RTC.Minute&0x7f));
   	RTC.Hour	= BcdtoBinary((char)(RTC.Hour&0x3f));

	RTC.HoursNMinute = RTC.Hour * 256 + RTC.Minute;
	RTC.Seconds = (unsigned int) RTC.Second;		
 	return((unsigned char*)&RTC.Hour);
}

unsigned char* ReadClock12Hr()
{  
   	RTC.Second	= ReadByteRTC(ADDRESS_SECOND);
   	RTC.Minute	= ReadByteRTC((ADDRESS_MINUTE));
  	RTC.Hour	= ReadByteRTC((ADDRESS_HOUR));
   	RTC.Second 	= BcdtoBinary((char)(RTC.Second&0x7f));
   	RTC.Minute	= BcdtoBinary((char)(RTC.Minute&0x7f));
   	RTC.Hour	= BcdtoBinary((char)(RTC.Hour&0x3f));
   
	if(RTC.Hour>=12)
	{
		if(RTC.Hour!=12)
		{	RTC.Hour-=12;
			RTC.AMPM =1; 	// time in pm
		}

		else 
		{
			RTC.AMPM=0;
			if(RTC.Hour==0)
			RTC.Hour=12;	// time in am
		}
	}

  return((unsigned char*)&RTC.Hour);

}
  
 
unsigned char* ReadDate()
{  
	RTC.Date		= ReadByteRTC((ADDRESS_DATE));
	RTC.WeekDay 	= ReadByteRTC((ADDRESS_WEEKDAY));
	RTC.Month		= ReadByteRTC((ADDRESS_MONTH));
	RTC.Year		= ReadByteRTC((ADDRESS_YEAR));
	
	RTC.Date		= BcdtoBinary((char)(RTC.Date&0x3f));
	RTC.WeekDay	= BcdtoBinary((char)(RTC.WeekDay&0x07));
	RTC.Month		= BcdtoBinary((char)(RTC.Month&0x3f));
	RTC.Year 		= BcdtoBinary(RTC.Year)+2000;		// year is 0 to 99 only. add 2000 to get current year
	
	RTC.DateNMonth = RTC.Date * 256 + RTC.Month ;	
  return((unsigned char*)&RTC.Date);
}


/*
void DisplayTime()
{
   char LcdBuf[15];

	ReadClock();
	sprintf(LcdBuf,"%02u",RTC.Hour);
  	lcd_goto(5);
   
    lcd_putch(LcdBuf[0]);
	lcd_putch(LcdBuf[1]);
	lcd_puts(":");
    
	sprintf(&LcdBuf[2],"%02u",RTC.Minute);
	lcd_putch(LcdBuf[2]);
	lcd_putch(LcdBuf[3]);
	lcd_puts(":");

    sprintf(&LcdBuf[4],"%02u",RTC.Second);
	lcd_putch(LcdBuf[4]);
	lcd_putch(LcdBuf[5]);

	Delay_ms(1);	
	
}

void TestTime()
{
  unsigned char c = '0';


  lcd_goto(5);
  c = ReadByteRTC(ADDRESS_RTCRAM);
  lcd_putch(c);
  Delay_1s(1);
}

*/

void ReadRTC_DateTime()
{
	ReadDate();
	ReadClock24Hr();	
}	

char GetRTCData(char x,char* buff,char* MinBuff,char* MaxBuff,char* CompBuff)
{
	unsigned char scanbuff[10];
//	unsigned char blink=0;
	unsigned int blink=0;

	unsigned char digit_1=5;
//	unsigned char Validate[6]={0};
    if(x == 3)
        memset(LcdDispBuff+16,' ',16);
	buff[6]='\0';
	if(x==1)
		LcdDispBuff[26]=LcdDispBuff[29]=':';//for time
	else if((x == 2) || (x == 3))
		LcdDispBuff[26]=LcdDispBuff[29]='/';//for date
    if(x != 3)
        LcdDisplay(TOP,LcdDispBuff);
    while(1)
	{
		blink++;
		memcpy(scanbuff,buff,6);

		if(blink>50) 
			scanbuff[digit_1] = '_';

		if(blink>=100)
			blink=0;		
	
		memcpy(LcdDispBuff+24,scanbuff,2);
		memcpy(LcdDispBuff+27,scanbuff+2,2);
		memcpy(LcdDispBuff+30,scanbuff+4,2);
		
		LcdDisplay(BOTTOM,LcdDispBuff+16);

		KeySense();
		if(MenuKey)
		{
			return(0);
		}
		else if(EnterKey)
		{
			if(x==1)
			{
				RTC.Hour=AsciitoBcd(buff);
				RTC.Hour |= 0x80;			// setting as 24 hr format
				RTC.Minute=AsciitoBcd(buff+2);
				RTC.Second=AsciitoBcd(buff+4);
				return(1);
			}
			else if(x==2)
			{
				RTC.Date=AsciitoBcd(buff);
				RTC.Month=AsciitoBcd(buff+2);
				RTC.Year=AsciitoBcd(buff+4);
				return(1);
			}
			else if(x==3)
			{
			//	memcpy(Validate,buff+4,2);
			//	memcpy(Validate+2,buff+2,2);
			//	memcpy(Validate+4,buff,2);
				if(atol(buff) > 0)
					return(1);
			}
			
		}
		else if(UpKey)
		{
			if(CompBuff[digit_1]!='N' && buff[digit_1-1]<CompBuff[digit_1])
			{
				if(buff[digit_1]=='9')
				{
					if(buff[digit_1-1]=='0')
						buff[digit_1]=MinBuff[digit_1];
					else
						buff[digit_1]='0';
				}
				else
					buff[digit_1]+=1;
			}
			else
			{
				if(buff[digit_1]>=MaxBuff[digit_1])
					buff[digit_1]='0';
				else
					buff[digit_1]+= 1;
			}
			if(CompBuff[digit_1]=='N')
			{
			 	if(buff[digit_1+1] >MaxBuff[digit_1+1] && buff[digit_1]>=MaxBuff[digit_1])
				buff[digit_1+1]=MaxBuff[digit_1+1];

				if(buff[digit_1+1] <MinBuff[digit_1+1] && buff[digit_1]=='0')
				buff[digit_1+1]=MinBuff[digit_1+1];
			}
			blink=0;
		}
		else if(LeftKey)
		{
			if(digit_1==0)
				digit_1=5;
			else 
				digit_1--;

			blink=0;
		}
//		if(TimeOutFlag.TimeOut)
//			return(0);
	}
	return(0);
}

void Set_Date()
{
    unsigned char DateFormatMax[7]={"311299"};
    unsigned char DateFormatMin[7]={"010100"};
    unsigned char DateFormatComp[7]={"N3N1N0"};
    char LocalBuff1[7];
	char LocalBuff2[7];
	char LocalBuff3[7];
	char LocalBuff4[7];	

	ReadRTC_DateTime();
	memcpy(LcdDispBuff+16,"Date:           ",16);
	sprintf(LocalBuff1,"%02u%02u%02u",RTC.Date,RTC.Month,(RTC.Year % 100));
	memcpy(LocalBuff2,DateFormatMin,6);
	memcpy(LocalBuff3,DateFormatMax,6);
	memcpy(LocalBuff4,DateFormatComp,6);
//    sprintf(LcdDispBuff,"%6ld",(unsigned long)290817);
//    RTC.Date=AsciitoBcd(LcdDispBuff);
//    RTC.Month=AsciitoBcd(LcdDispBuff+2);
//    RTC.Year=AsciitoBcd(LcdDispBuff+4);
	if(GetRTCData(2,LocalBuff1,LocalBuff2,LocalBuff3,LocalBuff4))
	{
		WriteToRTC_Date();
	}
	memcpy(LcdDispBuff+16,"                ",16);
	return;
}

void Set_Time()
{
    unsigned char TimeFormat24hrMax[7]={"235959"};
    unsigned char TimeFormat24hrMin[7]={"000000"};
    unsigned char TimeFormat24hrComp[7]={"N2N0N0"};    
	char LocalBuff1[7];
	char LocalBuff2[7];
	char LocalBuff3[7];
	char LocalBuff4[7];

	ReadRTC_DateTime();
	memcpy(LcdDispBuff+16,"Time:           ",16);
	sprintf(LocalBuff1,"%02u%02u%02u",RTC.Hour,RTC.Minute,RTC.Second);	
	memcpy(LocalBuff2,TimeFormat24hrMin,6);
	memcpy(LocalBuff3,TimeFormat24hrMax,6);
	memcpy(LocalBuff4,TimeFormat24hrComp,6);
//    sprintf(LcdDispBuff,"%06ld",(unsigned long)94600);    
//    RTC.Hour=AsciitoBcd(LcdDispBuff);
//    RTC.Hour |= 0x80;			// setting as 24 hr format
//    RTC.Minute=AsciitoBcd(LcdDispBuff+2);
//    RTC.Second=AsciitoBcd(LcdDispBuff+4);
   	if(GetRTCData(1,LocalBuff1,LocalBuff2,LocalBuff3,LocalBuff4))
	{
		WriteToRTC_Time();
	}
	memcpy(LcdDispBuff+16,"                ",16);
	return;

}

void RTC_Init()
{
	char TempYear[2]={'0','1'};
    LcdDisplay(BOTTOM,(char *)"  Are You Sure? ");
    while(1)
	{
		KeySense();
		if(MenuKey)
		{
			break;
		}
		else if(EnterKey)
		{
//			memcpy(LcdDispBuff+16,"  Initializing  ",16);
			LcdDisplay(BOTTOM,(char *)"  Initializing  ");

			RTC.Date=1;
			RTC.Month=1;
			//RTC.Year=2012;
			RTC.Year = AsciitoBcd(TempYear);
			WriteToRTC_Date(); Delay_ms(1000);
		
			RTC.Hour=1;
			RTC.Hour |= 0x80;			// setting as 24 hr format
			RTC.Minute=1;
			RTC.Second=1;
			WriteToRTC_Time(); Delay_ms(1000);
			break;
		}
//		if(TimeOutFlag.TimeOut)
//			return;
	}
	memset(LcdDispBuff+16,' ',16);
	return;
}



void PowerOnRTC(void)
{
    
	char TempYear[2]={'0','1'};
	char i =0;//CheckCounter = 0;
//       LedDisplay("  6   ",0,0);
//    Delay1_1s(2);
	for(i=0;i<=6;i++)
	{
		ReadRTC_DateTime();
        
		if((RTC.Date == 0) || (RTC.Month == 0))
		{
		//	CheckCounter++;
			if(i >= 6)
			{
				LcdDisplay(TOP,   "   RTC Error    ");
				LcdDisplay(BOTTOM,"Press ENTER/ESC ");	
//                LedDisplay("RTCErr",0,0);
				while(1)
				{
                    KeySense();
                    if((EnterKey)||(MenuKey))
                    {
//						IBMKeyValue=' ';
                        if(MenuKey)
                        {
//							LcdDisplay(TOP,   "   Warning!!!   ");
//							LcdDisplay(BOTTOM,"Set Date & Time ");				
//							Delay_1sec(15);
                            RTC.Date=1;
                            RTC.Month=1;
                            RTC.Year = AsciitoBcd(TempYear);
                            WriteToRTC_Date();
                            IICClockDelay();

                            RTC.Hour=1;
                            RTC.Hour |= 0x80;			// setting as 24 hr format
                            RTC.Minute=1;
                            RTC.Second=1;
                            WriteToRTC_Time(); 
                            IICClockDelay();
                        }
                        else if(EnterKey)
                        {
                        	LcdDisplay(TOP,   "   RTC - Date   ");
                        //    LedDisplay(" Date ",0,0);
                            Delay1_1s(1);
                            //FlagSet = 3;
                            Set_Date();
                            ///FlagSet = 0;
                            LcdDisplay(TOP,   "   RTC - Time   ");
                            //LedDisplay(" Time ",0,0);
                            Delay1_1s(1);
                            //FlagSet = 3;
                            Set_Time();	
                            //FlagSet = 0;							
                        }
                        break;
                    }
				}
			}
		}
		else
        {
//          sprintf(LcdDispBuff,"%02u",i);
//          LedDisplay(LcdDispBuff,0,0); Delay1_1s(1);
//          sprintf(LcdDispBuff,"%02u%02u%02u",RTC.Date,RTC.Month,(RTC.Year%100));
//          LedDisplay(LcdDispBuff,0,0); Delay1_1s(1);
//          sprintf(LcdDispBuff,"%02u%02u%02u",RTC.Hour,RTC.Minute,RTC.Second);
//          LedDisplay(LcdDispBuff,0,0); Delay1_1s(1);
            break;
        }
	}
//   LedDisplay("  7   ",0,0);
//    Delay1_1s(2);	
}

void RTC_Check()
{
	char display = 0;
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			memset(LcdDispBuff+16,' ',32);
			break;
		}
		else if(UpKey)
		{
			if(display++ > 0)
				display = 0;
		}
		ReadRTC_DateTime();
		if(display == 0)
			sprintf(LcdDispBuff+16," Date:%02u/%02u/%04u",RTC.Date,RTC.Month,RTC.Year);
		else
			sprintf(LcdDispBuff+16," Time: %02u:%02u:%02u ",RTC.Hour,RTC.Minute,RTC.Second);

		LcdDisplay(BOTTOM,LcdDispBuff+16);
//		if(TimeOutFlag.TimeOut)
//			return;
	}	
}

unsigned char Get_Hr_min_Sec(unsigned char *Hour,unsigned char Hour_Enb,unsigned char *Min,unsigned char Min_Enb,unsigned char *Sec,unsigned char Sec_Enb)
{
    unsigned char TimeFormat24hrMax[7]={"235959"};
    unsigned char TimeFormat24hrMin[7]={"000000"};
    unsigned char TimeFormat24hrComp[7]={"N2N0N0"};    
	char LocalBuff1[7] = {0};
	char LocalBuff2[7] = {0};
    unsigned char scanbuff[10];
    unsigned char digit_1 = 5,blink = 0,count = 1;
    LcdDispBuff[16]='(';
    if(Sec_Enb == 0)
        digit_1 = 3;
    if(Hour_Enb)
    {
        sprintf(LocalBuff1,"%02u",*Hour);	
    	LcdDispBuff[26]=':';//for time
        memcpy(LcdDispBuff+(16+count),"H:",2);
        count+=2;
    }
    if(Min_Enb)
    {
        sprintf(LocalBuff1+2,"%02u",*Min);	
        LcdDispBuff[16+count] = 'M';
        count+=1;
        if(Sec_Enb)
        {   
            LcdDispBuff[29]=':';//for time
            LcdDispBuff[16+count]=':';
            count+=1;
        }
    }
    if(Sec_Enb)
    {
        sprintf(LocalBuff1+4,"%02u",*Sec);	
        memcpy(LcdDispBuff+(16+count),"S",1);
        count+=1;
    }
    LcdDispBuff[16+count]=')';
    while(1)
	{
		blink++;
		memcpy(scanbuff,LocalBuff1,6);

		if(blink>50) 
			scanbuff[digit_1] = '_';

		if(blink>=100)
			blink=0;		
	
		memcpy(LcdDispBuff+24,scanbuff,2);
		memcpy(LcdDispBuff+27,scanbuff+2,2);
		memcpy(LcdDispBuff+30,scanbuff+4,2);
		
		LcdDisplay(BOTTOM,LcdDispBuff+16);

		KeySense();
		if(MenuKey)
		{
            memcpy(LcdDispBuff+16,"                ",16);            
			return(0);
		}
		else if(EnterKey)
		{
            LocalBuff2[2]='\0';
            if(Hour_Enb)
            {
                memcpy(LocalBuff2,LocalBuff1,2);
                *Hour = atoi(LocalBuff2);                    
            }
            if(Min_Enb)
            {
                memcpy(LocalBuff2,LocalBuff1+2,2);
                *Min = atoi(LocalBuff2);                    
            }                    
            if(Sec_Enb)
            {
                memcpy(LocalBuff2,LocalBuff1+4,2);
                *Sec = atoi(LocalBuff2);
            }
            memcpy(LcdDispBuff+16,"                ",16);
            return(1);
		}
		else if(UpKey)
		{
			if(TimeFormat24hrComp[digit_1]!='N' && LocalBuff1[digit_1-1]<TimeFormat24hrComp[digit_1])
			{
				if(LocalBuff1[digit_1]=='9')
				{
					if(LocalBuff1[digit_1-1]=='0')
						LocalBuff1[digit_1]=TimeFormat24hrMin[digit_1];
					else
						LocalBuff1[digit_1]='0';
				}
				else
					LocalBuff1[digit_1]+=1;
			}
			else
			{
				if(LocalBuff1[digit_1]>=TimeFormat24hrMax[digit_1])
					LocalBuff1[digit_1]='0';
				else
					LocalBuff1[digit_1]+= 1;
			}
			if(TimeFormat24hrComp[digit_1]=='N')
			{
			 	if(LocalBuff1[digit_1+1] >TimeFormat24hrMax[digit_1+1] && LocalBuff1[digit_1]>=TimeFormat24hrMax[digit_1])
				LocalBuff1[digit_1+1]=TimeFormat24hrMax[digit_1+1];

				if(LocalBuff1[digit_1+1] <TimeFormat24hrMin[digit_1+1] && LocalBuff1[digit_1]=='0')
				LocalBuff1[digit_1+1]=TimeFormat24hrMin[digit_1+1];
			}
			blink=0;
		}
		else if(LeftKey)
		{
            if(((digit_1==0) && (Min_Enb) && (Hour_Enb) && (Sec_Enb == 0)))
                digit_1=3;
            else if(((digit_1==0) && (Hour_Enb)) || 
                    ((digit_1==2) && (Min_Enb) && (Hour_Enb == 0)) ||
                        ((digit_1==4) && (Sec_Enb) && (Min_Enb == 0) && (Hour_Enb == 0)))
                digit_1=5;
            else 
				digit_1--;

			blink=0;
		}
//		if(TimeOutFlag.TimeOut)
//			return(0);
	}
    

	return;

}


