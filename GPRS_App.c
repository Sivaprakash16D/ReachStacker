//#include "Defaults.h"
//#include "Includes.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "Memory.h"
extern void Write_E2(const unsigned long Address,char* Data,const int NoofBytes);
extern void Read_E2(char* Data,const unsigned long Address,const int NoofBytes);
#define NO_OF_CH            6
extern struct
{
	long MinWt;
    unsigned int No_of_Samples;      
	unsigned long LatchDelay;
	long LatchedWt;    
	unsigned int AngleConfig;
    struct
    {
        unsigned char Hour;
        unsigned char Minute;
        unsigned char Second;    
    }Perioric_Timer;
	unsigned char Mobile_Operator;
	unsigned long OverWeight;    
    unsigned char LCED[NO_OF_CH];
	unsigned long Red_Glow;
	unsigned long Amber_Flash;
	unsigned long Amber_Glow;
    unsigned char Input1Logic;
    unsigned long Corr_Factor;  
    unsigned char PLMS_Status;
    unsigned char ComMode;
    unsigned char OperationLogic;
    unsigned char AngleEnDis;  
    unsigned char Input2Logic;    
    unsigned long CAN_Broadcast_Time;
    unsigned long RS232_Dump_Time;
}Application_Setting;
extern unsigned char ucount;
#define ReceiveBuff_size 200
extern unsigned char ReceiveBuff[ReceiveBuff_size];
extern unsigned char MenuKey;
extern unsigned char EnterKey;
extern unsigned char UpKey;
extern unsigned char LeftKey;	
extern unsigned char MemoryCheck;			// To check the Memory to Load Default values
extern unsigned char Sec_MemoryCheck;
extern struct 
{
    long Zero;
    long Zero1;
    long Span;
	long CalCapacity;    
    long RawADC;
    float CurWeight;	
    long Corr_Factor;	
}Loadcell[NO_OF_CH];

extern struct
{
	char Date;
	char Month;
	int Year;
	char Hour;
	char Minute;
	char Second;
	long SumWeight;
    long RawADC[NO_OF_CH];
    long CurWeight[NO_OF_CH];
	unsigned int Average_Cnt;
	unsigned int Trip_Cnt;
    unsigned char Input[2];
    unsigned char PLMS_Status;
	long Average_Weight;
}Periodic_Report,Latch_Report;

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

extern struct struct_rtc RTC;

extern struct
{
	int RAWADC;
	long CurWeight;	
	long LatchedWt;    
	int TripCount;	
	unsigned long CumulativeWeight;   
    char Polarity; 
}AllChannel;

extern unsigned int Average_Count;
// Digital Inputs
#define ILATCH		_RC2		//IP1 
#define IRUN		_RC1		//IP2 
extern unsigned char MinWt_Check_Counter;
extern struct
{
	char Date;
	char Month;
	int Year;
	char Hour;
	char Minute;
	char Second;
	long Weight;
	unsigned char OLStatus;
	unsigned char TempFlag;
	unsigned char Polarity;
}TripRecord;

extern struct
{
	unsigned long DeviceID;	 					
	unsigned long Password;
	unsigned long BuadRate;
    unsigned long CAN_BuadRate;
}System_setting;

extern struct
{
    unsigned OCERRFlag1		:1;
    unsigned OCERRFlag2		:1;
    unsigned OCERRFlag3		:1;
    unsigned OCERRFlag4		:1;
    unsigned OCERRFlag5		:1;
    unsigned OCERRFlag6		:1;
    unsigned ORERRFlag1		:1;
    unsigned ORERRFlag2		:1;
    unsigned ORERRFlag3		:1;
    unsigned ORERRFlag4		:1;
    unsigned ORERRFlag5		:1;    
    unsigned ORERRFlag6		:1;    
    unsigned URERRFlag1		:1;
    unsigned URERRFlag2		:1;
    unsigned URERRFlag3		:1;
    unsigned URERRFlag4		:1;
    unsigned URERRFlag5		:1;
    unsigned URERRFlag6		:1;
    unsigned LatchedWt		:1;
    unsigned Disp_Latch_Wt	:1;    
    unsigned ERRFlag    	:1;    
    unsigned Com2Check    	:1;  
    unsigned ILATCH_on		:1;
    unsigned ILATCH_val		:1;
    unsigned InputCheck		:1;
    unsigned RecOverWrite	:1;    
}Bits;

struct UsedVar
{
	unsigned char Signal;
	unsigned char ModemStatus;
	unsigned char Active_BankNo;
	unsigned char SendStatus;
	unsigned short Received_Bank[3];
	unsigned short Send_Bank[3];
}GPRS;

extern volatile unsigned char ReceivedFlag;
extern unsigned long TimerCntr1;
extern char LcdDispBuff[40];
#define TOP      1
#define BOTTOM   2
void Rx_ClearVar(void);

void GPRS_Status_Disp(void)
{
    static unsigned int Disp_counter = 0;
    Disp_counter++;
    if(Disp_counter < 100)
        sprintf((char*)&LcdDispBuff,"R1: %4u S1:%4u",GPRS.Received_Bank[0],GPRS.Send_Bank[0]);
    else if(Disp_counter < 200)    
        sprintf((char*)&LcdDispBuff,"R2: %4u S2:%4u",GPRS.Received_Bank[1],GPRS.Send_Bank[1]);
    else if(Disp_counter < 300)    
        sprintf((char*)&LcdDispBuff,"R3: %4u S3:%4u",GPRS.Received_Bank[2],GPRS.Send_Bank[2]);
    if(Disp_counter > 300)   
        Disp_counter = 0;
    if(GPRS.ModemStatus == 0)
        memcpy((char*)&LcdDispBuff+16,"SIMChk",6);
    else if(GPRS.ModemStatus == 1)
        memcpy((char*)&LcdDispBuff+16,"PINChk",6);
    else if(GPRS.ModemStatus == 2)
        memcpy((char*)&LcdDispBuff+16,"SIMREG",6);
    else if(GPRS.ModemStatus == 3)
        memcpy((char*)&LcdDispBuff+16,"APNSET",6);
    else if(GPRS.ModemStatus == 4)
        memcpy((char*)&LcdDispBuff+16,"INIOK ",6);
    else
        memcpy((char*)&LcdDispBuff+16,"      ",6);
    sprintf((char*)&LcdDispBuff+23,"L:%2uF%01u:%2u",GPRS.Signal,GPRS.Active_BankNo,GPRS.SendStatus);
    LcdDisplay(TOP,LcdDispBuff);
    LcdDisplay(BOTTOM,LcdDispBuff+16);
}

void GPRS_Status_Read(unsigned char Mode)
{
//  unsigned int Rx_CheckSum_Value = 0;
//	unsigned int Tx_CheckSum_Value = 0;
//  unsigned char SentBuff[11] = {0};
	char CheckSumBuff[5] = {0};
  
//	[BUF2OK*L015*S00*F200*R10000*S10000*R20001*S20001*R30450*S30450*66]
//	[BUF3OK*L014*S04*F902*R10002*S10002*R20000*S20000*R30010*S30016*38]
//	ReceiveBuff,"BUF2OK",6)
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+8,3);
	GPRS.Signal = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+13,2);
	GPRS.ModemStatus = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+17,1);
	GPRS.Active_BankNo = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+18,2);
	GPRS.SendStatus = atoi(CheckSumBuff);

	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+23,4);
	GPRS.Received_Bank[0] = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+30,4);
	GPRS.Send_Bank[0] = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+37,4);
	GPRS.Received_Bank[1] = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+44,4);
	GPRS.Send_Bank[1] = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+51,4);
	GPRS.Received_Bank[2] = atoi(CheckSumBuff);
	
	memset(CheckSumBuff,' ',5);
	memcpy(CheckSumBuff,ReceiveBuff+58,4);
	GPRS.Send_Bank[2] = atoi(CheckSumBuff);
}

void GPRS_Display(void)
{
	unsigned char ResetAck_Flag = 0;
	unsigned char RequestBuff[11] = {0};
	unsigned int CheckSum_Value = 0;
    unsigned char COMFail_count = 1;
	ucount = 0;  	
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			ClearLcdDisplay(3);
			break;
		}
        if(COMFail_count == 0)
            GPRS_Status_Disp();
        else
        {
            if(COMFail_count < 15)
                TwoLineDisplay("                "," Please Wait... ",0);  
            else
                TwoLineDisplay("                ","   CHECK COMM   ",0);
        }
        if(ResetAck_Flag == 0)
        {
            memcpy(RequestBuff,"[STATUSLL]",10);
            ucount = 0;
            CheckSum_Value = CalcLRC(RequestBuff+1,6);
            sprintf((char*)&RequestBuff+7,"%02u",CheckSum_Value);
            RequestBuff[9] = ']';
            Rx_ClearVar();
            ReceivedFlag = 0;            
            Com2StringSend(RequestBuff,10);
            ResetAck_Flag = 1;
            TimerCntr1 = 1;
        }
		if(TimerCntr1 > (unsigned long)(1000*3))		
		{
			TimerCntr1 = 0;
			ResetAck_Flag = 0;
            COMFail_count++;
            if(COMFail_count > 15)
               COMFail_count = 15;
		}

		if( (ReceivedFlag == 1) && (ucount >= 65) )
		{
            COMFail_count = 0;
            if(!strncmp((char *)ReceiveBuff,"STATUS",6)) //STATUS
            {
                TimerCntr1 = 0;
                ResetAck_Flag = 0;
                GPRS_Status_Read(1);
            }        
            else
            {
                TimerCntr1 = 0;
                ResetAck_Flag = 0;                
            }
		}
	}    
}

void Rx_ClearVar(void)
{
	memset(ReceiveBuff,' ',ReceiveBuff_size);
	ucount = 0;
	ReceivedFlag = 0;
}

void GPRS_Reset_Function(unsigned char mode)
{
	unsigned char ResetMode = 0;
	unsigned char ResetAck_Flag = 0;
	unsigned char ResetBuff[11] = {0};
	unsigned int CheckSum_Value = 0;
	ucount = 0;  	
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			ClearLcdDisplay(3);
			break;
		}
		else if(UpKey)
		{
			if(ResetMode++ > 3)
				ResetMode = 0;
		}
		else if(EnterKey)
		{
			if(ResetMode == 0)
				memcpy(ResetBuff,"[RSTALLLL]",10);
			else if(ResetMode == 1)
				memcpy(ResetBuff,"[RST001LL]",10);
			else if(ResetMode == 2)
				memcpy(ResetBuff,"[RST002LL]",10);
			else if(ResetMode == 3)
				memcpy(ResetBuff,"[RST003LL]",10);
			ucount = 0;
			CheckSum_Value = CalcLRC(ResetBuff+1,6);
			sprintf((char*)&ResetBuff+7,"%02u",CheckSum_Value);
			ResetBuff[9] = ']';
			Rx_ClearVar();
            ReceivedFlag = 0;            
			Com2StringSend(ResetBuff,10);
			ResetAck_Flag = 2;
			TimerCntr1 = 1;
		}
		if(TimerCntr1 > (unsigned long)(1000*3))		// For Timeout after 3  seconds
		{
			TimerCntr1 = 0;
			ResetAck_Flag = 3;
		}
		if(ResetAck_Flag == 0)
		{
			if(ResetMode == 0)
				memcpy(LcdDispBuff+16,"      All       ",16);
			else if(ResetMode == 1)
				memcpy(LcdDispBuff+16,"     BANK-1     ",16);
			else if(ResetMode == 2)
				memcpy(LcdDispBuff+16,"     BANK-2     ",16);
			else if(ResetMode == 3)
				memcpy(LcdDispBuff+16,"     BANK-3     ",16);
		}
		else if(ResetAck_Flag == 1)
		{
			LcdDisplay(BOTTOM,"  Reset Success ");
			TimerCntr1 = 0;
			ResetAck_Flag = 0;
			Rx_ClearVar();
			Delay1_1s(2);
		}
		else if(ResetAck_Flag == 2)
		{
			memcpy(LcdDispBuff+16,"  Resetting...  ",16);
		}
		else if(ResetAck_Flag == 3)
		{
			LcdDisplay(BOTTOM,"  Reset Fail    ");
			ResetAck_Flag = 0;
			Rx_ClearVar();
			Delay1_1s(2);
		}
		if( (ReceivedFlag == 1) && (ucount == 8) )
		{
			if(ResetMode == 0)
			{
				if(!strncmp((char *)ReceiveBuff,"RSTAOK",6))
					ResetAck_Flag = 1;
			}
			else if(ResetMode == 1)
			{
				if(!strncmp((char *)ReceiveBuff,"RST1OK",6))
					ResetAck_Flag = 1;
			}
			else if(ResetMode == 2)
			{
				if(!strncmp((char *)ReceiveBuff,"RST2OK",6))
					ResetAck_Flag = 1;
			}
			else if(ResetMode == 3)
			{
				if(!strncmp((char *)ReceiveBuff,"RST3OK",6))
					ResetAck_Flag = 1;
			}
			if(ResetAck_Flag != 1)
				ResetAck_Flag = 3;
		}

		LcdDisplay(BOTTOM,LcdDispBuff+16);
	}
}

void Operator_Selector()
{
    unsigned int CheckSum_Value = 0;
	unsigned char MOperator=0;
    unsigned char SentBuff[11] = {0};
	unsigned int blink = 0;
    unsigned char SetAck_Flag = 0;
    ucount = 0;  
    MOperator= Application_Setting.Mobile_Operator;
	memset(LcdDispBuff+16,' ',16); 
    TimerCntr1 = 0;
	if(MOperator > 13)
			MOperator = 0;	            
	while(1)
	{
		blink++;
		if(blink<=150) //90
		{
			if(MOperator==0)               
					memcpy(LcdDispBuff+16,"         AIRTEL ",16);            
			else if(MOperator == 1)            
					memcpy(LcdDispBuff+16,"    TATA DOCOMO ",16);            
			else if(MOperator == 2)
					memcpy(LcdDispBuff+16,"           IDEA ",16);
			else if(MOperator == 3)
					memcpy(LcdDispBuff+16,"         AIRCEL ",16);
			else if(MOperator == 4)
					memcpy(LcdDispBuff+16,"           BSNL ",16);
			else if(MOperator == 5)
					memcpy(LcdDispBuff+16,"       STARHUB1 ",16);
			else if(MOperator == 6)
					memcpy(LcdDispBuff+16,"  SINGTEL(PRE1) ",16);
			else if(MOperator == 7)
					memcpy(LcdDispBuff+16,"       STARHUB2 ",16);
			else if(MOperator == 8)
					memcpy(LcdDispBuff+16,"  SINGTEL(PRE2) ",16);
			else if(MOperator == 9)
					memcpy(LcdDispBuff+16,"   SINGTEL(POS) ",16);
			else if(MOperator == 10)
					memcpy(LcdDispBuff+16,"        M1(POS) ",16);
			else if(MOperator == 11)
					memcpy(LcdDispBuff+16,"        M1(PRE) ",16);
			else if(MOperator == 12)
					memcpy(LcdDispBuff+16,"       VODAFONE ",16);
			else if(MOperator == 13)
					memcpy(LcdDispBuff+16,"            JIO ",16);
		}	
		else 
		{
			memset(LcdDispBuff+16,' ',16);
		}
		
		if(blink>250)
			blink=0;
	
		KeySense();
		if(EnterKey)
		{
			sprintf((char*)&SentBuff,"[SET%03uLL]",MOperator);
			CheckSum_Value = CalcLRC(SentBuff+1,6);
			sprintf((char*)&SentBuff+7,"%02u",CheckSum_Value);
			SentBuff[9] = ']';
			SetAck_Flag = 1;
			TimerCntr1 = 1;
			Rx_ClearVar();
			Com2StringSend(SentBuff,10);
		}
		else if(MenuKey)
		{
			ClearLcdDisplay(3);
            break;
		}
		else if(UpKey)
		{
            MOperator++;
			if(MOperator > 13)
				MOperator = 0;
			blink=0;
		}

		if(TimerCntr1 > (unsigned long)(1000*3))		// For Timeout after 3  seconds
		{
			TimerCntr1 = 0;
			SetAck_Flag = 3;
		}
		
		if((ReceivedFlag == 1) && (ucount == 8))
		{
            if(!strncmp((char *)ReceiveBuff,"SET OK",6))
			{
                SetAck_Flag = 2;
            }
			else
            {
				SetAck_Flag = 3;
            }
		}

		if(SetAck_Flag == 1)
            memcpy(LcdDispBuff+16," Please wait... ",16);
		else if(SetAck_Flag == 2)
		{
			Application_Setting.Mobile_Operator = MOperator;
			Write_E2(EES_Mobile_Operator,(char *)&Application_Setting.Mobile_Operator,sizeof(Application_Setting.Mobile_Operator));
			LcdDisplay(BOTTOM,(char *)"   Set Success  ");
//            sprintf(LcdDispBuff+16,"%d:%d:%d",ReceivedFlag,ucount,SetAck_Flag);
//            LcdDisplay(TOP,ReceiveBuff);            
            TimerCntr1 = 0;
			SetAck_Flag = 0;
			Rx_ClearVar();
			Delay1_1s(5);
			ClearLcdDisplay(3);
            break;
		}
		else if(SetAck_Flag == 3)
		{
			LcdDisplay(BOTTOM,(char *)"   Set Failed   ");
			SetAck_Flag = 0;
			Rx_ClearVar();
			Delay1_1s(2);
		}
        LcdDisplay(BOTTOM,LcdDispBuff+16);		
	}
}
