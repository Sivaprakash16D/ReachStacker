//#define CLEAR	0
//#define SET 	1
//#include "LPC15xx.h"
//#include "lpc15xx_gpio.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "Memory.h"
//#include "Defaults.h"
//#include "Includes.h"
#include "p30Fxxxx.h"
//#include "rom_uart_15xx.h"
//#include "uart_15xx.h"
//#include "romapi_15xx.h"

#define TOP 1
#define BOTTOM 2
#define TOP_BOTTOM 3


#define AUTO			0
#define MANUAL			1
//#define LCD_LINE_1 0x00 
//#define LCD_LINE_2 0x01 
//#define LCD_LINE_3 0x02 
//#define LCD_LINE_4 0x04  

#define MULTIPLY_BY10FACTOR 10
#define MaxOption           100
#define None                0
#define PtoChar             1
#define PtoLong             2
#define Combo               3
#define SplFunction         4
#define PtoInt              5
#define ComboStr            6
#define	PtoLong1            7

#define Baud1200  	0X0185
#define Baud2400	0X00C2
#define Baud4800	0X0060
#define Baud9600	0X002F
#define Baud19200 	0X0017
#define Baud38400 	0X000B
#define Baud57600	0X0007
#define Baud115200	0X0003

//#define CAPSENSE_1 4
//#define CAPSENSE_2ABC 13
//#define CAPSENSE_3DEF 9
//#define CAPSENSE_4GHI 5
//#define CAPSENSE_5JKL 8
//#define CAPSENSE_6MNO 10
//#define CAPSENSE_7PQRS 7
//#define CAPSENSE_8TUV 17
//#define CAPSENSE_9WXYZ 12
//#define CAPSENSE_CLEAR 15
//#define CAPSENSE_0 11
//#define CAPSENSE_ENTER 6
//#define CAPSENSE_UP 14
//#define CAPSENSE_LEFT 2
//#define CAPSENSE_DOWN 1
//#define CAPSENSE_RIGHT 3
//#define CAPSENSE_ESC 16
//#define NO_OF_CAPKEY 17

signed char ADCAvgNameDisp = 0;
extern signed long ADCAverageVar;
signed char ADCAverageCounter;

//extern void Write_E2(unsigned long Address,char* Data,int NoofBytes);
//extern void Read_E2(char* Data,unsigned long Address,int NoofBytes);
extern char LcdDispBuff[40];
char NewTypeGetData(char* buff,char EntryMode,char NoDigit);
unsigned char Enter_NxtFlag = 0;
unsigned char Esc_NxtFlag = 0;
unsigned char E2ErrorFlag = 0;

unsigned char U1okFlag;
unsigned char U2okFlag;
unsigned char U3okFlag;
unsigned char U4okFlag;

const unsigned char BLINKSPEED = 30;

char menutmp[8];
unsigned char DispDP=0;
unsigned char DiagADC = 0;
unsigned char CursorIndex;
unsigned char LoadCellSel = 1;
unsigned long lngval;
unsigned long int PowerOnADC = 0;
float WeightWithDecimal = 0.0000,OCwithDecimal = 0.0000;

unsigned char Main_Diag_Entry_Flag = 0;	
//volatile unsigned char clock_1s;   /* Flag activated each second */
//volatile unsigned char Reverse_1s;
//volatile unsigned int tick0 = 0;
//volatile unsigned int tick1 = 0;
//
  
//struct struct_rtc
//{
//	char Hour;
//	char Minute;
//	char Second;
//	char Date;
//	char Month;
//	int Year;
//	char WeekDay;
//	char AMPM;	
//	int  HoursNMinute;
//	int  DateNMonth;
//	int	 Seconds;
//};
//extern struct struct_rtc RTC;

extern char Temp_Hour;
extern char Temp_Minute;
extern char Temp_Date;
extern char Temp_Month;
extern int Temp_Year;

extern unsigned long ADCSampleVal;
unsigned int Current_CH_Axle=0;

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

extern struct
{
	unsigned long REFvolt1;
	unsigned long REFvolt2;
	int REF1Cnt;
	int REF2Cnt;
	int Ch1Ref1;
	int Ch1Ref2;
	int Ch2Ref1;
	int Ch2Ref2;
	int Ch3Ref1;
	int Ch3Ref2;
	int Ch4Ref1;
	int Ch4Ref2;
	int Ch5Ref1;
	int Ch5Ref2;
	int Ch6Ref1;
	int Ch6Ref2;
	int Offset_Volt;
}VoltageCalib,BVoltageCalib;

extern unsigned int DecimalPoint;
extern long MaxCapacity;
extern unsigned char Resolution;

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5

extern struct
{
	unsigned TimeOutEnb		:1;
	unsigned TimeOut		:1; 
}TimeOutFlag;

extern unsigned char Unit;
long cmbminmax[8]={0,5,8,13,14,18,6,2};	
char codedpt[7]={0,1,2,3,4,5,6};
unsigned char cmbdata[20]={1,2,5,10,20,50,100,1,2,4,8,16,32,64,0,1,2,3,4};
long BaudRateBuff[7]={1200,2400,9600,19200,38400,57600,115200};
long CAN_BaudRateBuff[3]={250,500,1000};
//               0   1   2 3  4  5    6     7    8   9  10 11 12 13  14    15   16  17   18    19   20 21 22  23
long minmax[14]={0,99000,9,1,99,100,9999,99999,999,4095,45,3,500,10};//,255,50000,9,30,65534,99999,200,999,2000,999999,80,10,90,4095};
unsigned int Sflfun[31]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};//,17,18,19,20,21,22,23,24,25,26,27,28,29};//changed from int to char for memory reducing
unsigned char resdata[7]={1,2,5,10,20,50,100};
char GetCombo(char x);
char GetData(char* buff,char x,char mode,char flag,long minval,long maxval,char dept);
//void WeightCalibration(char Selected_Parameter);
char PasswordEntry = 0;
struct DataRegis
{
	union
	{
		float          fData;
		unsigned long  ulData;
		long           lData;
		unsigned long *pulData;
		long          *plData;
		unsigned int   uiData;
		int            iData;
		unsigned int   *puiData;
		int            *piData;
		unsigned char  ucData;
		signed char    scData;
		unsigned char *pucData;
		signed char   *pscData;
	}Val;
};

struct __MENUDESCR
{
   
	unsigned int Next;  
	unsigned int Enter;   
	unsigned int Esc;
	unsigned int E2Index;
	char Name[17];

	union
	{
	  unsigned char Value;
	  struct
	  {
		 unsigned  char WriteE2:1;    //e2 prom writing wanted
		 unsigned  char Validation:1; // validation reqd
		 unsigned  char Blink:1;	  // blink reqd while getting the values
		 unsigned  char SeeMin:1;	  // see min value for validation
		 unsigned  char SeeMax:1;	  // see max value for validation
		 unsigned  char Numeric:1;	  // Input from user thro the keypads
		 unsigned  char Sign:1;		  // sign to get as input
		 unsigned  char isPassword:1; // if blink flag then blinking
	  }flags;
	}mnuFlags;
	unsigned char mnuType;

	struct DataRegis *Value;
	
	long *Min;
	long *Max;

	unsigned int mnuCurIndex; // current index
	unsigned char CurScanDigiVal;
	unsigned char NumDigit:4;//to go till how much digit for scanning
	unsigned char CurDigi:4;//used for cur index
	unsigned char ShowHead:4;//alignment 0,1,2,3
	unsigned char ShowValue:4;//alignment 0,1,2,3
	unsigned char *NumDecimal;
	unsigned char ErrNo;
};
//extern unsigned int DecimalPoint;
//extern unsigned int Average_Count;

#define Array_Counter  400

#define	Loaddefault           0
#define	TakeAngleCalBackup    1
#define	RestoreAngCalBackup   2
#define InitGenSettings       3
#define InitVoltValue         4
#define TakeVoltBackup        5
#define RestoreVoltBackup     6
#define InitAppSettings       7
#define	RestoreLoadCalBackup  8
#define	TakeLoadCalBackup     9

const struct __MENUDESCR MenuData[MaxOption] =
{
{15,15,0,0, "*     MENU     *",0x00,0,0,0,0,0,0,0,0,0,0,0,0},//0

{2,2,0,0, "* VOLTAGE CALIB*",0x00,0,0,0,0,1,0,0,0,0,0,0,0},//1
{3,3,1,EE_REFvolt1, "*  REFERENCE 1 *",0x23,PtoLong,(struct DataRegis*)&VoltageCalib.REFvolt1,(long *)&minmax[0],(long *)&minmax[7],2,0,5,0,0,0,(unsigned char *)&codedpt[3],0},//2
{4,4,1,EE_REFvolt2, "*  REFERENCE 2 *",0x23,PtoLong,(struct DataRegis*)&VoltageCalib.REFvolt2,&minmax[3],&minmax[7],3,0,5,0,0,0,(unsigned char *)&codedpt[3],0},//3
{5,5,1,0,"* VOLT BACKUP? *",0x20,SplFunction,(struct DataRegis*)&Sflfun[1],0,0,4,0,0,0,0,0,0,0},//4
{2,2,1,0,"*  CALIB END   *",0x00,0,0,0,0,5,0,0,0,0,0,0,0},//5

{63,7,0,0, "    LOAD CAL    ",0x00,0,0,0,0,6,0,0,0,0,0,0,0},//6
{9,9,6,EDecimalPoint," DECIMAL POINT  ",0x23,PtoInt,(struct DataRegis*)&DecimalPoint,&minmax[0],&minmax[11],7,0,1,0,0,0,(unsigned char *)&codedpt[0],0},//7
{61,61,6,EMaxCapacity, "  MAX-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&MaxCapacity,&minmax[3],&minmax[1],8,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//8
{8,8,6,EResolution,  "   RESOLUTION   ",0x21,Combo,(struct DataRegis*)&Resolution,&cmbminmax[0],&cmbminmax[6],9,0,0,0,0,0,(unsigned char *)&codedpt[0],0},//9
{11,44,6,0,"    CAL ZERO    ",0x00,0,0,0,0,10,0,0,0,0,0,0,0},//10
{12,48,6,0,"    CAL SPAN    ",0x00,0,0,0,0,11,0,0,0,0,0,0,0},//11
{13,54,6,0,"  CAL-CAPACITY  ",0x00,0,0,0,0,12,0,0,0,0,0,0,0},//12
{14,14,6,0,"  BACKUP CALIB? ",0x20,SplFunction,(struct DataRegis*)&Sflfun[1],0,0,13,0,0,0,0,0,0,0},//13
{7,15,6,0,"   CALIB END    ",0x00,0,0,0,0,14,0,0,0,0,0,0,0},//14

{74,16,0,0,"GENERAL SETTINGS",0x00,0,0,0,0,15,0,0,0,0,0,0,0},//15
{25,25,15,EDeviceID,"    DEVICE ID   ",0x23,PtoInt,(struct DataRegis*)&System_setting.DeviceID,&minmax[0],&minmax[4],16,0,2,0,0,0,(unsigned char *)&codedpt[0],0},//16
{62,62,74,E_MinWt,"     MIN.WT     ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.MinWt,&minmax[0],&MaxCapacity,17,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//17
{57,57,74,E_LatchDelay,"  LATCH DELAY   ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.LatchDelay,&minmax[0],&minmax[8],18,0,3,0,0,0,(unsigned char *)&codedpt[1],0},//18
{56,56,74,EAngleConfig,"  ANGLE CONFIG  ",0x23,PtoInt,(struct DataRegis*)&Application_Setting.AngleConfig,&minmax[0],&minmax[2],19,0,1,0,0,0,(unsigned char *)&codedpt[0],0},//16
{21,21,74,0,"   CH. CONFIG   ",0x20,SplFunction,(struct DataRegis*)&Sflfun[8],0,0,20,0,0,0,0,0,0,0},//20
{22,22,74,E_Red_Glow,"    RED GLOW    ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.Red_Glow,(long *)&Application_Setting.Amber_Flash,(long*)&MaxCapacity,21,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//21
{23,23,74,E_Amber_Flash,"   AMBER FLASH  ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.Amber_Flash,(long *)&Application_Setting.Amber_Glow,(long*)&Application_Setting.Red_Glow,22,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//22
{24,24,74,E_Amber_Glow,"   AMBER GLOW   ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.Amber_Glow,&minmax[0],(long*)&Application_Setting.Amber_Flash,23,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//23
{28,28,74,0,"  INPUT LOGIC   ",0x20,SplFunction,(struct DataRegis*)&Sflfun[3],0,0,24,0,0,0,0,0,0,0},//24
{26,26,15,EPassword,"    PASSWORD    ",0x23,PtoLong,(struct DataRegis*)&System_setting.Password,&minmax[0],&minmax[8],25,0,3,0,0,0,(unsigned char *)&codedpt[0],0},//25
{27,27,15,0,"  RTC-SET DATE  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[9],0,0,26,0,0,0,0,0,0,0},//26
{29,29,15,0,"  RTC-SET TIME  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[10],0,0,27,0,0,0,0,0,0,0},//27
//{29,29,15,EAutoZero,"*  AUTO ZERO   *",0x23,PtoInt,(struct DataRegis*)&Setup.AutoZero,&minmax[0],&minmax[12],16,0,1,0,0,0,&codedpt[0],0},//28
//{30,30,15,EPowerOnTare,"*POWER ON TARE *",0x23,PtoLong,(struct DataRegis*)&Setup.PowerOnTare,&minmax[0],&minmax[5],21,0,3,0,0,0,&codedpt[0],0},//29
{79,79,74,ECorr_Factor,"    C-FACTOR    ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.Corr_Factor,&minmax[0],&minmax[7],28,0,4,0,0,0,(unsigned char *)&codedpt[3],0},//28
{80,80,15,EBuadRate,  " RS232-BAUDRATE ",0x21,Combo,(struct DataRegis*)&System_setting.BuadRate,&cmbminmax[0],&cmbminmax[6],29,0,0,0,0,0,(unsigned char *)&BaudRateBuff[0],0},//29
{16,16,15,0,"  SETUP - END   ",0x00,0,0,0,0,30,0,0,0,0,0,0,0},//30

{15,32,0,0,"   DIAGNOSTICS  ",0x00,0,0,0,0,31,0,0,0,0,0,0,0},//31
{33,33,31,0,"     RAW ADC    ",0x20,SplFunction,(struct DataRegis*)&Sflfun[0],0,0,32,0,0,0,0,0,0,0},//32
{34,34,31,0,"     KEYPAD     ",0x20,SplFunction,(struct DataRegis*)&Sflfun[6],0,0,33,0,0,0,0,0,0,0},//33
{35,35,31,0,"     DISPLAY    ",0x20,SplFunction,(struct DataRegis*)&Sflfun[14],0,0,34,0,0,0,0,0,0,0},//34
{36,36,31,0,"   DIGITAL I/P  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[7],0,0,35,0,0,0,0,0,0,0},//35
{37,37,31,0,"   DIGITAL O/P  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[13],0,0,36,0,0,0,0,0,0,0},//36
{38,38,31,0,"      COMM      ",0x20,SplFunction,(struct DataRegis*)&Sflfun[5],0,0,37,0,0,0,0,0,0,0},//37
{39,39,31,0,"    RTC-CHECK   ",0x20,SplFunction,(struct DataRegis*)&Sflfun[15],0,0,38,0,0,0,0,0,0,0},//38 
{40,40,31,0,"    RTC-INIT.   ",0x20,SplFunction,(struct DataRegis*)&Sflfun[11],0,0,39,0,0,0,0,0,0,0},//39
{42,42,31,0,"    RESTORE ?   ",0x20,SplFunction,(struct DataRegis*)&Sflfun[23],0,0,40,0,0,0,0,0,0,0},//40 for changed 41-42
{42,42,31,0,"  LOAD DEFAULT  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[12],0,0,41,0,0,0,0,0,0,0},//41
{75,75,31,0," INIT GEN SETT. ",0x20,SplFunction,(struct DataRegis*)&Sflfun[4],0,0,42,0,0,0,0,0,0,0},//42
{32,6,31,0,"DIAGNOSTICS END ",0x00,0,0,0,0,43,0,0,0,0,0,0,0},//43

{45,45,10,ELC1Zero1,"  CH1-CAL ZERO  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[0].Zero1,&minmax[3],&minmax[9],44,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//44
{46,46,10,ELC2Zero1,"  CH2-CAL ZERO  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[1].Zero1,&minmax[3],&minmax[9],45,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//45
{47,47,10,ELC3Zero1,"  CH3-CAL ZERO  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[2].Zero1,&minmax[3],&minmax[9],46,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//46
{67,67,10,ELC4Zero1,"  CH4-CAL ZERO  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[3].Zero1,&minmax[3],&minmax[9],47,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//47

{49,49,11,ELC1Span,"  CH1-CAL SPAN  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[0].Span,&Loadcell[0].Zero,&minmax[9],48,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//48
{50,50,11,ELC2Span,"  CH2-CAL SPAN  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[1].Span,&Loadcell[1].Zero,&minmax[9],49,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//49
{51,51,11,ELC3Span,"  CH3-CAL SPAN  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[2].Span,&Loadcell[2].Zero,&minmax[9],50,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//50
{68,68,11,ELC4Span,"  CH4-CAL SPAN  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[3].Span,&Loadcell[3].Zero,&minmax[9],51,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//51

{53,53,12,ELC3CalCapacity,"  CH3-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[2].CalCapacity,&minmax[3],&MaxCapacity,52,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//52
{69,69,12,ELC4CalCapacity,"  CH4-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[3].CalCapacity,&minmax[3],&MaxCapacity,53,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//53
{55,55,12,ELC1CalCapacity,"  CH1-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[0].CalCapacity,&minmax[0],&minmax[10],54,0,2,0,0,0,(unsigned char *)&codedpt[0],0},//54
{52,52,12,ELC2CalCapacity,"  CH2-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[1].CalCapacity,&minmax[0],&minmax[10],55,0,2,0,0,0,(unsigned char *)&codedpt[0],0},//55

{66,66,74,0," PERIODIC TIMER ",0x20,SplFunction,(struct DataRegis*)&Sflfun[27],0,0,56,0,0,0,0,0,0,0},//56

{19,19,74,E_LatchedWt,"  LATCH WEIGHT  ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.LatchedWt,&Application_Setting.MinWt,&MaxCapacity,57,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//58

{59,59,40,0,"      LOAD      ",0x20,SplFunction,(struct DataRegis*)&Sflfun[2],0,0,58,0,0,0,0,0,0,0},//58
{58,58,40,0,"     VOLTAGE    ",0x20,SplFunction,(struct DataRegis*)&Sflfun[2],0,0,59,0,0,0,0,0,0,0},//59

{78,78,31,0," GPRS REC RESET ",0x20,SplFunction,(struct DataRegis*)&Sflfun[17],0,0,60,0,0,0,0,0,0,0},//60
{10,10,6,EUnit,  "      UNIT      ",0x21,Combo,(struct DataRegis*)&Unit,&minmax[0],&minmax[3],61,0,0,0,0,0,(unsigned char *)&codedpt[0],0},//61

{18,18,74,ENo_of_Samples," NO.OF SAMPLES  ",0x23,PtoInt,(struct DataRegis*)&Application_Setting.No_of_Samples,&minmax[13],&minmax[12],62,0,3,0,0,0,(unsigned char *)&codedpt[0],0},//68
{15,10,0,0,"  ANGLE CALIB   ",0x00,0,0,0,0,63,0,0,0,0,0,0,0},//63
{20,20,74,EE_OverWeight,"    OVER LOAD   ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.OverWeight,(long*)&Application_Setting.MinWt,(long*)&MaxCapacity,64,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//70
{57,57,15,EE_BucketWt,"   BUCKET WT    ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.Amber_Flash,(long*)&Application_Setting.MinWt,(long*)&MaxCapacity,65,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//71
{79,79,74,0, " MOBILE OPERATOR",0x20,SplFunction,(struct DataRegis*)&Sflfun[16],0,0,66,0,0,0,0,0,0,0},//66
//{30,30,15,EE_BucketWt,"* OP. TARE E/D *",0x23,PtoLong,(struct DataRegis*)&Setup.BucketWt,(long*)&Setup.MinWt,(long*)&MaxCapacity,72,0,5,0,0,0,&DecimalPoint,0},//72

{71,71,10,ELC5Zero1,"  CH5-CAL ZERO  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[4].Zero1,&minmax[3],&minmax[9],67,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//67
{72,72,11,ELC5Span,"  CH5-CAL SPAN  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[4].Span,&Loadcell[4].Zero,&minmax[9],68,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//68
{73,73,12,ELC5CalCapacity,"  CH5-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[4].CalCapacity,&minmax[3],&MaxCapacity,69,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//69

{60,60,31,0,"  VOLTAGE INIT  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[18],0,0,70,0,0,0,0,0,0,0},//70

{44,44,10,ELC6Zero1,"  CH6-CAL ZERO  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[5].Zero1,&minmax[3],&minmax[9],71,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//67
{48,48,11,ELC6Span,"  CH6-CAL SPAN  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[5].Span,&Loadcell[5].Zero,&minmax[9],72,0,4,0,0,0,(unsigned char *)&codedpt[0],0},//68
{54,54,12,ELC6CalCapacity,"  CH6-CAPACITY  ",0x23,PtoLong,(struct DataRegis*)&Loadcell[5].CalCapacity,&minmax[3],&MaxCapacity,73,0,5,0,0,0,(unsigned char *)&DecimalPoint,0},//69

{31,56,0,0," APP. SETTINGS  ",0x00,0,0,0,0,74,0,0,0,0,0,0,0},//74 chnaged to 17-56
{60,60,31,0," INIT APP SETT. ",0x20,SplFunction,(struct DataRegis*)&Sflfun[22],0,0,75,0,0,0,0,0,0,0},//75 for changed 76-60
{77,77,31,0," LAST TARE DATE ",0x20,SplFunction,(struct DataRegis*)&Sflfun[21],0,0,76,0,0,0,0,0,0,0},//76
{70,70,31,0," LAST CAL DATE  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[20],0,0,77,0,0,0,0,0,0,0},//77
{85,85,31,0,"  GPRS DISPLAY  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[25],0,0,78,0,0,0,0,0,0,0},//78
{84,84,74,0,"    COM MODE    ",0x20,SplFunction,(struct DataRegis*)&Sflfun[26],0,0,79,0,0,0,0,0,0,0},//79

{30,30,15,ECAN_BuadRate,"  CAN-BIT RATE  ",0x21,Combo,(struct DataRegis*)&System_setting.CAN_BuadRate,&cmbminmax[0],&cmbminmax[7],80,0,0,0,0,0,(unsigned char *)&CAN_BaudRateBuff[0],0},//80
{82,82,74,ECAN_Broadcast_Time," CAN DUMP TIME  ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.CAN_Broadcast_Time,&minmax[0],&minmax[6],81,0,4,0,0,0,(unsigned char *)&codedpt[1],0},//81
{84,84,74,EERS232_Dump_Time," HMI DUMP TIME  ",0x23,PtoLong,(struct DataRegis*)&Application_Setting.RS232_Dump_Time,&minmax[0],&minmax[6],82,0,4,0,0,0,(unsigned char *)&codedpt[1],0},//82
{56,74,74,0," APP. SETUP-END ",0x00,0,0,0,0,83,0,0,0,0,0,0,0},//83
{83,83,74,0," MOBILE NUMBER  ",0x20,SplFunction,(struct DataRegis*)&Sflfun[28],0,0,84,0,0,0,0,0,0,0},//84
{86,86,74,0," View SMS Count ",0x20,SplFunction,(struct DataRegis*)&Sflfun[29],0,0,85,0,0,0,0,0,0,0},//84
{43,43,74,0,"View GPRS LIMIT ",0x20,SplFunction,(struct DataRegis*)&Sflfun[30],0,0,86,0,0,0,0,0,0,0},//84
};

#define loadcells   0
#define Voltage     1
#define loadcells_Voltage   2
#define Angle       3


extern unsigned char CapKeyValueBuff; 
//extern unsigned char CapKeyValue;
extern unsigned char CapSenseRead;
extern unsigned char CapSenseReadClear;
extern void KeySense(void);
char digit = 4;	
unsigned char blink=0;

extern void LcdDisplay	(char Mode,char *Buff);
extern void BigDelay(unsigned long DelayValue);
//extern void Delay(unsigned int DelayValue);
//extern void ClearLcdDisplay(void);
extern void LocalEuWeigh(void);

//char GetCombo(unsigned char x);
char GetComboString(char x);
void StoreCalib(unsigned char);
void ShowADC(char Val);
void TestWeight(void);
void CheckE2Memory(void);
void DisplayCheck(void);
void PortCheck(void);
char DiagnoseFlag = 0;
char Getvalues_mA(char Ref,long mA_val);
char GetCombo(char x);
extern unsigned char CounterOverflowFlg;
extern unsigned char MinWt_Check_Counter;
void Digital_OP(void);
void DIPCheck(void);
void KeyPadTest(void);
void PrinterCheck(void);
//void KeyPadTest1(void);
void LoadDefault(void);
char Get_Password(void);

void Graphic_Loading(char No_of_Bytes);
void RTC_Values_Init(void);
void RTC_Values(void);
void SetClock(char Time_SMSTime);
//unsigned char SetDate(char* TopDisp,unsigned char *source,char Date_Entry);
void StoreBackUp(void);							  // For Calib BackUp
void RestoreBackup(void);						  // For Calib Restore
void Set_Baud(char ComPort);
void Digital_Output_Off(char LED_Num);
void Digital_Output_On(char LED_Num);

#define LPC_USART0_BASE           0x40040000
#define LPC_USART1_BASE           0x40044000
#define LPC_USART2_BASE           0x400C0000
#define LPC_SYSCTL_BASE           0x40074000

#define LPC_SYSCTL                ((LPC_SYSCTL_T           *) LPC_SYSCTL_BASE)
#define LPC_USART0                ((LPC_USART_T            *) LPC_USART0_BASE)
#define LPC_USART1                ((LPC_USART_T            *) LPC_USART1_BASE)
#define LPC_USART2                ((LPC_USART_T            *) LPC_USART2_BASE)

unsigned char Keypad_Entry_Flag;
extern unsigned char WeightLoadErr;
extern unsigned char Config_Flag;

//extern int UART0_SendChar (unsigned char ch);
//extern int UART1_SendChar (unsigned char ch);
extern int UART2_SendChar (unsigned char ch);
extern int UART3_SendChar (unsigned char ch);

//extern void UART1_Init (unsigned int BaudRate);

extern void Digital_Output_On(char LED_Num);
extern void Digital_Output_Off(char LED_Num);

//extern void LedDisplay(char* Buff,unsigned char Mode,unsigned char DpPos);
 void Put_DecimalPoint_Display(unsigned char *SourceBuff,unsigned char Length,unsigned char DpPos);

int VehicleNumeric_Entry(unsigned int SourceNumBuff,unsigned char Length,unsigned char DpPos,unsigned int MinVal,unsigned int MaxVal);
void EditSumSingleLine(void); 			// To Edit Code Summary 1 & 2. and Single Line Printbil
//extern void Delay_1sec(unsigned long count);
extern void Clear234LCD(void); 
extern void Clear34LCD(void); 
void DOPCheck(void);
extern unsigned long GunnyWeight;
extern char AsciitoBcd(char *Buff);
extern void WriteToRTC_Date(void);
extern void WriteToRTC_Time(void);
unsigned char TimeFormat24hrMax[7]={"235959"};
unsigned char TimeFormat24hrMin[7]={"000000"};
unsigned char TimeFormat24hrComp[7]={"N2N0N0"};

unsigned char DateFormatMax[7]={"311299"};
unsigned char DateFormatMin[7]={"010100"};
unsigned char DateFormatComp[7]={"N3N1N0"};
void LoadDefault(void);
char GetRTCData(char x,char* buff,char* MinBuff,char* MaxBuff,char* CompBuff,char No_of_Digit);
extern void Brightness_Adj(unsigned long Address,char* Data);
void LoadDefault(void);
void DoMenu(void);
void Init_Setup(void);

unsigned long Shift1_StartMin = 0;
unsigned long Shift2_StartMin = 0;
unsigned long Shift3_StartMin = 0;

unsigned long Shift1_EndMin = 0;
unsigned long Shift2_EndMin = 0;
unsigned long Shift3_EndMin = 0;

//unsigned long CurrentMin;
#define MAXSHIFT 3
struct ShiftStartTime  				//Shift start time
{
	unsigned char hour;
	unsigned char minutes;
}Shift[MAXSHIFT];

struct ShiftEndTime  				//Shift start time
{
	unsigned char hour;
	unsigned char minutes;
}EndShift[MAXSHIFT];
unsigned char CurrentShift 	= 0;
#define SIZE_CHAR		1
int shiftstartendtime=0;
#define CLEAR		0
#define SET 		1
#define COM_1       0
#define COM_2       1
//extern struct
//{
//	unsigned TimeOutEnb		:1;
//	unsigned TimeOut		:1; 
//}TimeOutFlag;
extern unsigned long TimeOut_Cnt;
extern unsigned long TimeOut_Cnt1;
char GetPassword(void);
extern unsigned char LED_Brightness;
#define InputSel  0
#define Minussel  1

#define ParkingBreak    0
#define BodyUp          1
extern unsigned long TimerCntr1 ;
//extern unsigned long TimeOut_Cnt;
extern unsigned char MenuKey;
extern unsigned char EnterKey;
extern unsigned char UpKey;
extern unsigned char LeftKey;		
extern unsigned char curindex;
unsigned int CH1ADC,CH2ADC,CH3ADC,CH4ADC,TotalADC,CH5ADC,CH6ADC;
extern unsigned long ADCValue,ADCSampleVal;
#define ReceiveBuff_size 200
extern unsigned char ReceiveBuff[ReceiveBuff_size];
extern unsigned char ucount;
extern volatile unsigned char ReceivedFlag;
extern char PowerOnKeyPress;
extern unsigned char FlagSet;	
unsigned char InputName[3][6] = {" None "," PB-BU"," IP2  "};
void InputConfig(void);
extern int No_of_Samplesarray[Array_Counter];
void GPRS_Reset_Function(unsigned char mode);
extern char Header1_Name[20];
void ChannelConfiguration(void);
void Restore_Calib_Backup(char loadcells_or_Voltage);
void Voltage_calib_read(void);
void Calibration_Backup(char loadcells_or_Voltage);
void done_Display(void);
void Confirmation_Function(char selectedparameter);
void InputLogic_Multiply_Select(char InputSel_Minussel);
void ShowAD(char val);
void DiagnosticCounts(void);
extern void Write_E2(const unsigned long Address,char* Data,const int NoofBytes);
extern void Read_E2(char* Data,const unsigned long Address,const int NoofBytes);
extern void LastCalibRecord(void);
extern void LasttareRecord(void);
extern struct 
{
	char Date;
	char Month;
	int Year;
	char Hour;
	char Minute;
	char Second;    
}LastCalib,LastTare;
void Init_GenSetting(void);
void Init_AppSetting(void);
unsigned char OptionChooseFunction(char * DataOrigin,unsigned char AvailData,unsigned char InitData);
#define COM_GPRS_MODULE     1
#define COM_PRINTER         2
#define COM_DUMP            3

#define Enable  1
#define Disable 2

/************** DEFINES *********************/
/* INPUTS */

//#define LatchInput				1
//#define secondInput				2
//
//#define ILATCH              DIP_Check(LatchInput)
//#define ISecondIP			DIP_Check(secondInput)


void Get_Phone_No(void);
unsigned char MobileNumBuff[17] = {0};
unsigned char MobileNumRdy = 0;
unsigned char Protocol_Buff[130] = {0};
char LCD_Buff1[17] = {0};
char LCD_Buff2[17] = {0};
unsigned char TempMobEntryBuff[20] = {0};
extern void Interrupt_Check(void);
extern unsigned int CalcLRC(char* ChecksumBuff,unsigned int ByteCount);

extern unsigned char RT_Temp_Date ;
extern unsigned long RT_Count ;

void View_SMS_Count(void);
void View_GPRS_LIMIT(void);
