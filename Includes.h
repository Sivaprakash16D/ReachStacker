/***************************************************************************************************************************/
/*												PAYLOAD_CON_UNIT															*/
/***************************************************************************************************************************/

#ifndef INCLUDES_HEADER
 #define INCLUDES_HEADER

#include "Memory.h"

#define ON   1
#define OFF  0
#define True 1
#define False 0
#define OutputOn 0
#define OutputOff 1

#define TOP      1
#define BOTTOM   2

// Status of Trip
#define UN_LATCH	0	// NOT LATCHED
#define VAL_LATCH	1	// VALID LATCH - After first lift Bucket rested with Weight
#define UNLOADED	2	// After Unloading, Bucket is rested 

// Digital Inputs
#define IPARKING		_RC2		//IP1 
#define IBODYUP 		_RC1		//IP2 

//Digital Outputs
#define LATCH_OP		_RB5		//OP1
//#define OVERLOAD_OP		_RB4		//OP2

#define ArraySize		150
#define RecordsInABank 	934
#define RecordSize 		70


//LED
#define RED		_RG6			// SOLID LED 1
#define GREEN	_RG7			// SOLID LED 2
#define AMBER	_RG8			// SOLID LED 3
												        //  30/4/2016  //  27-4-16

//#define	RTSCHECK_10_VINC_RTS    		      _RB4//(From Payload-Tripper counter RTS pin changed to RB4 from RB9 - 13-04-2018) //same as prev // (input)		To check the writing or not		
#define	RTSCHECK_10_VINC_RTS    		      _RB9//(From Payload-Tripper counter RTS pin changed to RB9 from RB4 - 08-05-2019) //same as prev // (input)		To check the writing or not		
#define	USBAVAIL_11_LD1_VINC_IO8              _RD10 //(Prev-RB14)  // (input)		To check the usb available or not (input)
#define	RESET_9_VINC_RESET                    _RD0  //(Prev-RB15)  // (Output)    To set the vinculum controller after USB detected
#define TRANSFERCOMPLETE_19_VINC_IO11         _RD11 //             // (input)     To check the data transfer complete or not

//Baud Rate Config
#define Baud1200  	0X0185
#define Baud2400	0X00C2
#define Baud4800	0X0060
#define Baud9600	0X002F
#define Baud19200 	0X0017
#define Baud38400 	0X000B
#define Baud57600	0X0007
#define Baud115200	0X0003

#define Delay_1mS_Cnt      7480 //((Timer1Fcy / 1000000) * 1000)
unsigned long TIMEOUT = 120000;

#define START	Com1StringSend("%",1);
#define END		Com1StringSend("$",1);

#define MAXRECORDS		 	3000 	// Record Storage Limit 	3000
#define MAXRECORDS_WARNING 	5 	// Record Storage Limit 	50
#define MAX_TRIPCOUNT	 	9999 	// Maximum Trip Count		9999
#define SIZE_REC	14

unsigned long RS232_Dump_Time_Cntr = 0;
unsigned long RS232_Dump_Time_Flag = 5;

unsigned long CAN_BroadcastCntr = 0;
unsigned long CAN_BroadcastTime = 1000;
unsigned long CAN_BroadcastFlag = 5;
extern unsigned char C1ER_flag;

/*********** Variables used***************/
unsigned char MemoryCheck;			// To check the Memory to Load Default values
unsigned char Sec_MemoryCheck;
char ReceivedChar;					// UART
char LcdDispBuff[40];				// Used for send the characters to LCD 

unsigned char InterruptFlag = 0;

char ReceivedChar2;
unsigned int ReceiveCounter2 = 0;
char ReceiveBuff2[100];

char ModelName[6] = {"TIPPER"};

// Used in Reading ADCounts
int count=0;		
int ADCOUNTS = 0;
unsigned char Samples; 
unsigned long ADCValue,ADCSampleVal;
unsigned int *ADC16Ptr;

unsigned char BytesToSend1,BytesSent1;
unsigned char BytesToSend2,BytesSent2;
#define ArraySize		150
#define Rec_ArraySize   601
unsigned char CharRTable1[ArraySize],CharRTable2[ArraySize],PackRcvd1[ArraySize],PackToSend1[ArraySize];
unsigned char PackRcvd2[Rec_ArraySize],PackToSend2[ArraySize];
unsigned char CharIn1,CharIn2,CharOut1,CharOut2,CharRcvd1,CharRcvd2,CharCount1,CharCount2;
unsigned char ID,MSBCSum,LSBCSum,CSum,ByteCount1,ByteCount2,MaxChars1,MaxChars2;

unsigned long TimeInSecs;
unsigned int ReceiveCounter1 = 0;
unsigned long AvgWeight = 0;
//unsigned long Weight_Buff[10] = {0};
unsigned long Weight_Cntr = 0;

/* VOLVO Additional Variables */
void Process(void);
void PressureBar_Calc(void);

unsigned long CurrentPressureBar = 0;	//For Current Pressure Bar
volatile unsigned long ProcessTimer = 0;

#define MAXPEAK	5
unsigned int PeakBar[MAXPEAK];
unsigned int PeakCursor[MAXPEAK];
unsigned int PeakIndex = 0;
unsigned long RPM_Timer = 0;
unsigned long MaxPeakTimer = 0;

unsigned int CurrentPeak = 0;
unsigned int MaxPeak = 0;
unsigned int PreviousBar = 0;
unsigned int ReductionCntr = 0;

unsigned char Power_On_Calib_Enb = 0;
unsigned char MinWt_Repeat = 0;

float Weight_Kg = 0;

unsigned long PreviousWeight = 0;

extern char DiagnoseFlag;
void Pressure_Display(void);
/****************************/
unsigned int Version=0x207;

unsigned char Delayval;	

//unsigned char resdata[20]={1,2,5,10,20,50,100,1,2,4,8,16,32,64,0,1,2,3,4};

struct __attribute__((__packed__))
{
	int RAWADC;
	int TripCount;	
	unsigned long CumulativeWeight;
	long MaxCapacity;
	unsigned long CalCapacity;
	int Zero;
	int Zero1;
	int Span;	
	unsigned char Resol;
	long EnggWt;
	long LatchedWt;
}Calibration;

struct 
{
	unsigned long REFmA1;
	unsigned long REFmA2;
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
	int ChxRef_mA;
	int Offset_mA;

}CurrentCalib,BCurrentCalib;

struct __attribute__((__packed__))
{
	unsigned char Channel1;
	unsigned char Channel2;
	unsigned char Channel3;
	unsigned char Channel4;
	unsigned long MaxPressure;
	unsigned long MinWt;
	unsigned long Band;
	unsigned long OverWeight;
	unsigned long Password;
	unsigned long TruckSerial;
	unsigned long LatchOP_Time;
	unsigned char InputLogic;
	unsigned char OperatingMode;
	unsigned long CapDelay;
	unsigned long CapTime;
	unsigned long Kfactor;
	unsigned int MulFactor_1;
	unsigned int MulFactor_2;
	unsigned int MulFactor_3;
	unsigned int MulFactor_4;
    unsigned int MulFactor_5;
	unsigned int MulFactor_6;
	unsigned int MulFactor_7;
    unsigned char DeviceID;
	unsigned char DestID; 
	unsigned char ComMode;
	unsigned char LatchType;
	unsigned long LatchBarWt;
    unsigned int Graphselect;
//	unsigned long SetPoint1;
//	unsigned long SetPoint2;
//	unsigned long SetPoint3;
//	unsigned char InputLogic;
//	unsigned char Header;
//	unsigned char TruckModel;


}Setup;



struct
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

//Current Record
struct
{
	unsigned char Status_ct;
	unsigned long Weight_ct;
}CurrentTrip;	

struct
{
	unsigned char Rpm;
	unsigned int Peak;
}Prev_Trip;

struct
{
	unsigned long SysON_Mins;
//	unsigned char SysON_Mins;
}RunTime;
unsigned long CurrentMin = 0;
int Pressure_Cursor = 0;
int StartCursor = 0;
int EndCursor = 0;

struct __attribute__((__packed__))
{
	struct __attribute__((__packed__))
	{
		unsigned char Polarity;
		unsigned long Constants;
	}Order[5];

}Polynomial[2];

void Dump_Captured_Values(void);
void Dump_Captured_Values_USB(void);
void Delay(unsigned long int);
void Initialise_Ports();
void PowerOn(void);
void UART_Config();
void Com1Bytesend(char);
void Com1StringSend(char*,char);
void Com2Bytesend(char);
void Com2StringSend(char*,unsigned int);
//void ADC_Init(void);
void ReadADCCounts(unsigned char Channel);

void CalcCRC(unsigned int BytesCount, unsigned char *SPtr);
unsigned char GetLSB(unsigned int Integer);
unsigned char GetMSB(unsigned int Integer);
void ReadSettings();
void CalculatemA();

void Com1Init();
void Com2Init(unsigned long BuadRate);
void CheckPort1();
void ReceiveCheck(void);
void Display_SendData(void);
void Weight_Calc(void);
long RoundOff(long value,char round);
//void PutDp(unsigned char,unsigned long deci,char x,char displaymode);
void PutDp(unsigned char*Tmp,unsigned long deci,char x,char displaymode);
void CalcADC(void);
//void ORDisplay(void);
void PrintReport(void);
void PrintReport_USB(char date_serialNo);

void Reset_Cum_Trip(void);
void PrintResetOption(void);
void PrintOption(void);
void LimitSwitchCheck(void);
void InitialTripCheck(void);
void PowerOnRTC(void);
void Check_Tipping(void);
//void CheckUart2Data(void);
//void PrintAllRec(void);
//void PrintRec(long,long);
void ReadnSend_Rec(unsigned int,unsigned int,unsigned char);

extern void WriteByteRTC(),WriteToRTC_Date(),WriteToRTC_Time();

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
    int	 Seconds;
};

extern struct struct_rtc RTC;
unsigned char RunInput= 0;	//To update the Run time
unsigned int CycCntr;
//void DumpValues(void);
void Disp_mA(void);
void RunTimeUpdate(void);

unsigned char MainDisplay = 0;
//unsigned int OP_Flash_Cntr = 0;

extern void LoadDefault(void);
extern void Delay1_1s(unsigned char);
extern void ReadRTC_DateTime(void);
extern int Calf_mA(int CurCnt,int R1Cnt,int R2Cnt,int R2mA);

//	Latch Delay
//unsigned char TempSec = 0;
//unsigned char SecCntr = 0;	   

// Flash Display
unsigned long Flash_Cnt = 0;
struct
{
	unsigned FlashEnb		:1;
	unsigned Flash_TimeOut	:1; 
	unsigned FlashOver		:1;
}FlashDisp;

//Message Timer
unsigned long MsgTimer = 0;
unsigned long MsgDuration = 0;

// Over Load Output
unsigned char OL_Output_ON = 0;
unsigned long OL_Cnt = 0;


// Menu TimeOut

//unsigned long TimeOut_Cnt = 0;
struct
{
	unsigned TimeOutEnb		:1;
	unsigned TimeOut		:1; 
}TimeOutFlag;


//Input validation
volatile unsigned long CaptureDelayCounter = 0;
volatile unsigned long CaptureTimeCounter = 0;

unsigned long CapDelayTmr = 0;
unsigned long CapTimeTmr = 0;

unsigned long Total_Add = 0;


extern char GetPassword(void);
extern void DoMenu(void);
char GetRTCData(char x,char* buff,char* MinBuff,char* MaxBuff,char* CompBuff,char No_of_Digit);

void RangeOutput(void);
void Init_Interrupt(void);

//typedef struct tagFBITS {
//        unsigned SendData1      :1;     
//        unsigned SendData2      :1;   
//        unsigned Stx1Received   :1;     
//        unsigned Stx2Received   :1;     
//        unsigned Etx1Received   :1;     
//        unsigned Etx2Received   :1;   
//        unsigned ADDATAREADY  	:1;
//        unsigned ProcessPack1	:1;
//        unsigned ProcessPack2	:1;
//        unsigned Read1HR		:1;
//        unsigned Write1HR		:1;
//        unsigned Read2HR		:1;
//        unsigned Write2HR		:1;
//        unsigned NewSetting		:1;
//        unsigned mACalAvail		:1;
//		unsigned Calibrated 	:1;
//		unsigned CalculateSpeed	:1;
//		unsigned ReadRTC		:1;
//		unsigned DataAvail		:1;
//		unsigned FirstReading 	:1;
//		unsigned FirstSpeed		:1;
//		unsigned UpdateRecord	:1;
//		unsigned AnyChange		:1;
//		unsigned SendWeight		:1;
//		unsigned PowerOnDelay	:1;
//		unsigned WeightUpdated  :1;
//		unsigned GreenSet		:1;
//		unsigned AmberSet		:1;
//		unsigned RedSet			:1;
//		unsigned OLSet			:1;
//		unsigned Force1Coil		:1;
//		unsigned Force2Coil		:1;
//		unsigned InitPort1		:1;
//		unsigned Port1InitOver	:1;
//		unsigned CodeAvail		:1;
//		unsigned AngleCalibrated:1;
//		unsigned ReadStatus		:1;
//		unsigned AllowZero		:1;
//		unsigned Zeroed			:1;
//		unsigned InitPort2		:1;
//		unsigned Port2InitOver	:1;
//		unsigned ParkingSet		:1;
//		unsigned DoAverage		:1;
//		unsigned UpdateTime		:1;
//		unsigned DataQueried	:1;
//		unsigned FirstTime		:1;
//		unsigned FirstTimeRun	:1;
//		unsigned PowerOnCond1	:1;
//		unsigned CheckTime		:1;
//		unsigned FloatRcvd 		:1;
//		unsigned FaultAvail		:1;
//		unsigned UpdateNewWt	:1;
//		unsigned StoreInitial	:1;
//		unsigned InitCAN		:1;
//		unsigned FromCAN		:1;
//		unsigned SendLastRec	:1;
//		unsigned SendSetup		:1;
//} FLAGBITS;
FLAGBITS Flags;

//struct 
//{
//	unsigned UART_1_Start	:1;
//	unsigned UART_2_Start	:1;
//	unsigned UART_1_Ready	:1;
//	unsigned UART_2_Ready	:1;
////	unsigned MenuKey		:1;
////	unsigned EnterKey		:1;
////	unsigned UpKey			:1;
////	unsigned LeftKey		:1;
//	unsigned CommCheck		:1;
//}Bits;

//void ProcessByte2();
//void ProcessQuery2();
void ReadData(int Counter, char *RecordBuffer);
void SendOperator(unsigned char PortID);
void Send40001(unsigned int From, unsigned int Points,  char PortID);


unsigned int Com2TimeOut;
unsigned int Points2,StartAddr2;
//unsigned int StartPos;
//char Sno_Buff[8];
unsigned int cntr;
unsigned long RecordCursor = 0;
void Keyfunction();
void Record_Management(void);
extern void PC_Dump(void);
void MainScreenDisplay();
long MinBar;
unsigned char LatchedFlag = 0;

char PrintBuff[40] = {0};
unsigned char ModeSelectionFlag = 0;	//This is to select the One/All record data from PC through CAN Bus

/*Set up*/
void SETUP_PROCESS();
void DIAGNOSTICS_PROCESS();

struct
{	
	unsigned char	MaximumPressurBar;
	unsigned char 	DeckLampOnTime;
	unsigned char 	Decklamp_Output;
	unsigned char 	DumpModeDisplay;
	unsigned int 	Latch_Delay;
	unsigned int  	LoadFactor_Less20T;
	unsigned int  	LoadFactor_More20T;
	unsigned int  	DumpFactor_Less20T;
	unsigned int  	DumpFactor_More20T;
	unsigned int 	FR_FL_Correction_Factor;
	unsigned int 	RR_RL_Correction_Factor;
	unsigned int  	MimimumWeight;
	unsigned int 	GreenRange;
	unsigned int 	YellowRange;
	unsigned int  	RedRange;
	unsigned int  	OverRange;
	unsigned char 	IO_Status;	
	long Password;

}Setup_MCU;

struct
{
	unsigned int Channel_1RAWADC_mA;
	unsigned int Channel_2RAWADC_mA;
	unsigned int Channel_3RAWADC_mA;
	unsigned int Channel_4RAWADC_mA;	
	unsigned int Channel_1RAW_Count;
	unsigned int Channel_2RAW_Count;	
	unsigned int Channel_3RAW_Count;	
	unsigned int Channel_4RAW_Count;	
	unsigned int CurrentWeight;
	unsigned char IO_Status;
	unsigned int DistanceTravelled;
	unsigned int Speed;
	unsigned int Hour_Minitue;
	unsigned int Seconds;
	unsigned int Date_Month;
	unsigned int Year;
	unsigned int NoOfRevelution;	
	unsigned char ErrorStatus;	
	unsigned char LastStatus;
	unsigned int TripCount;
	unsigned int OvrLoadTrip;
	unsigned long CumulativeWeight;
	unsigned int Channel_1Pressure_Bar;
	unsigned int Channel_2Pressure_Bar;
	unsigned int Channel_3Pressure_Bar;
	unsigned int Channel_4Pressure_Bar;
}Mod_Dia_StoreData;

struct
{   	
	unsigned char Resolution;
	long Capacity;
	long DecimalPoint;
	unsigned char Unit;
	long CalCapacity;
	long int  CalZero;
	long int  CalZero1;
	long int  CalSpan;
	long int  InsAdc;
	long int  CurWt;
	unsigned char ChannelCheckCounter;
//	unsigned char WtPolarity;
	unsigned char CalMode;
	unsigned int CLMsb;
	unsigned int CLLsb;
	unsigned int BucketWeight;	
	unsigned int K1Factor;
	unsigned int K2Factor;
//	long OverCapacity;
//	long CalConstant;
//	long MinofConstant;
	long MaxofConstant;
	unsigned int DeclampOnTime;
//	char CalSecond;
//	char CalMinute;
	char CalHour;
	char CalDate;
	char CalMonth;
	char CalYear;
	long int PeakWt;
}Diagnostics;

unsigned char curindex;
unsigned char MenuProtocol_Flag = 0;
//unsigned char SerialNo[16];
//unsigned char ModelNo[16];
//unsigned char CustomerID[21];
unsigned char ReceiveDataBuff_Setup[75];
unsigned char ReceiveDataBuff_Setup_1[75];
unsigned char I_OStatusLeval[8];
unsigned char ReceiveReady;
void BytesToBits_SET(unsigned int);

void USB_status(void);




////////////////////////////////////////////////////////////////////////////////////////////////////////

struct tmr 
{
	unsigned StartTimer     :1;     
	unsigned TimerOver      :1;
	unsigned int TimerValue;
};

struct tmr WtTimer;
struct tmr WaitTimer,SettleTimer,BlinkTimer,GreenTimer;
struct tmr AmberTimer,RedTimer,HUTimer;
struct tmr ZeroTimer,NutrlTimer,AltTimer;
struct tmr SpeedLTimer,SpeedGTimer;
struct tmr NewRecTimer;
struct tmr CanFTimer;
struct tmr CanCTimer;

unsigned int SecondsCounter = 1000;

unsigned char RxCANPGN = 0;
unsigned char RxCompleteFlag = 0;
unsigned int BytesCount;

//unsigned char PackRcvd2[Rec_ArraySize];
unsigned int cntr,cntr1,StartPos,StartFrom;

unsigned char SlNo[8],ModelNo[8],Customer[20];
int MaxPressure,DLOnTime,DLStatus,DMDisplay,LatchDelay;
int LFactor1,LFactor2,DFactor1,DFactor2,FrontFactor,RearFactor;
int MinWeight,GreenRange,YellowRange,RedRange,OverRange;

extern struct
{
	unsigned Priority : 3;
	unsigned Reserved : 1;
	unsigned DataPage : 1;
	unsigned PF		  :	8;
	unsigned PS		  : 8;
	unsigned SAddress : 8;
	unsigned SRR	  : 1;
	unsigned RxIDE	  : 1;
	unsigned RXRTR	  : 1;
	unsigned RxRB1	  : 1;
	unsigned RxRB0	  : 1;
	unsigned DLC	  : 4;
}Extract_CANId,Tx_CANId;

extern unsigned long Received_PGN;
extern unsigned char C1R0_flag;
extern unsigned char C1R1_flag;
extern char FrameSequenceNumber;
unsigned char MCU_Version[2] = {0};

extern union{
	struct {
            unsigned Neutral 		:1;
            unsigned Float			:1;
            unsigned Parking		:1;
            unsigned Green			:1;
            unsigned Amber			:1;
            unsigned Red			:1;
            unsigned Buzzer			:1;
            unsigned Pressure		:1;
            unsigned Empty			:1;
            unsigned Loaded			:1;
            unsigned OverLoaded		:1;
            unsigned Alternator		:1;
            unsigned OLRelay		:1;
    }IOBits;
	unsigned int IOStatus;
}IO,LogicalStatus,ForcedStatus;

unsigned long TimerCntr1 = 0;
volatile unsigned int ReceiveStartFlag;
volatile unsigned char ReceivedFlag;
unsigned char ucount = 0;
#define ReceiveBuff_size 200
unsigned char ReceiveBuff[ReceiveBuff_size];
extern unsigned char MenuKey;
extern unsigned char EnterKey;
extern unsigned char UpKey;
extern unsigned char LeftKey;	

struct
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
#define NO_OF_CH            6
struct
{
	unsigned long DeviceID;	 					
	unsigned long Password;
	unsigned long BuadRate;
    unsigned long CAN_BuadRate;
}System_setting;

struct
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

struct
{
	int RAWADC;
	long CurWeight;	
	long LatchedWt;    
	int TripCount;	
	unsigned long CumulativeWeight;   
    char Polarity; 
}AllChannel;

unsigned int DecimalPoint;
long MaxCapacity;
unsigned char Resolution;
struct 
{
    long Zero;
    long Zero1;
    long Span;
	long CalCapacity;    
    long RawADC;
    float CurWeight;	
    long Corr_Factor;	
}Loadcell[NO_OF_CH];

struct
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

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5
extern void Rx_ClearVar(void);
unsigned int Average_Count = 0;
unsigned char MinWt_Check_Counter = 0;
unsigned char Unit = 0;
unsigned long Latching_Time = 0;
unsigned long Periodic_Time = 0;
void Delay_ms(unsigned char Delay);
char Send_Protocol = 0;
extern unsigned Header1_Name[20];
#define ADC_Average_Count 10
extern unsigned char BotDisp;
extern unsigned char Display_Deg;
extern struct UsedVariables
{
	unsigned char Periodic_Send_Flag;
	unsigned char Periodic_Store_Flag;
	unsigned char Latched_Send_Flag;
	unsigned char Periodic_Check;
	unsigned char HealthStatus;
	
	unsigned long Periodic_Send_Cntr;
	unsigned long Periodic_Store_Cntr;
	
	unsigned short Collection_Disp_Dly_Cntr;
	
	unsigned char LD_Event_PreStatus;
	unsigned char SetWeightStatus;
	unsigned char Alert;
	unsigned char AckFlag;
	
	unsigned char GPRS_Status_Flag;
	unsigned char GPRS_COMErr_Flag;	
}Flag;
extern unsigned long TimerCntr2;
extern long GetAdc(char Channel);
extern void Write_E2(const unsigned long Address,char* Data,const int NoofBytes);
extern void Read_E2(char* Data,const unsigned long Address,const int NoofBytes);
void TwoLineDisplay(char * FirstLineData,char * SecondLineData,unsigned char DelayTime);

#define INIT	0
#define LOAD 	1
#define DUMP 	2
#define EMPTY	3
#define RETURN 	4
#define HAULING 5

#define ParkingBreak    1
#define BodyUp          2
unsigned char changeLoadFlag = 0;
#define COM_GPRS_MODULE     1
#define COM_PRINTER         2
#define COM_DUMP            3

#define Enable              1
#define Disable             2

extern float Average_Wt;
void namechange_via_RS232(void);


unsigned char Input1sts_Bit = 0,Input1sts_Bit1 = 0,Input1sts_Bit2 = 0, PowerOn_Bit1 = 0,PowerOn_Bit2 = 0;
unsigned char Input2sts_Bit = 0,Input2sts_Bit1 = 0,Input2sts_Bit2 = 0, PowerOn_Bit3 = 0, PowerOn_Bit4 = 0;
extern void SMS_Alert(char status);
#endif

extern void Interrupt_Check(void);
void PowerOn_SetBit();

extern unsigned char RT_Temp_Date ;
extern unsigned long RT_Count,RT_Count1 ;


