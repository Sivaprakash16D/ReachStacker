/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
//#ifndef XC_HEADER_TEMPLATE_H
//#define	XC_HEADER_TEMPLATE_H

#include "p30fxxxx.h"
//#include "Defaults.h"
//#include "Includes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"

#define RS	1
#define AO	0

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5

#define TRUE	1
#define FALSE	0

#define Delay_1mS_Cnt      7480 //((Timer1Fcy / 1000000) * 1000)

#define Axle1     0
#define Axle2     1
#define Axle_NOs  2

#define loadcells   0
#define Voltage     1
#define loadcells_Voltage   2

#define MaxAdcCount	4050
#define MinAdcCount	  20

//#define MaxOption       71
#define	None 			0
#define	PtoChar			1
#define	PtoLong			2
#define	Combo			3
#define	SplFunction		4
#define	PtoInt			5  
#define ComboStr		6
#define	PtoLong1		7

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

#define	def_CalZero 	0
#define	def_CalSpan     1
#define	def_CalCapacity	2

#define INIT	0
#define LOAD 	1
#define DUMP 	2
#define EMPTY	3
#define RETURN 	4
#define HAULING 5

#define EWP_A_Latch  LATCbits.LATC4

#define Input_Direction 	1
#define Output_Direction	0
#define Output_Enable       1
#define Output_Disable 		0

#define TOP 1
#define BOTTOM 2
#define TOP_BOTTOM 3

#define Inverse_Logic_ON  1
#define Inverse_Logic_OFF 0

//struct
//{
//	long CalZero;
//	long CalZero1;
//	long CalSpan;
//	long InsAdc;
//	long CalCapacity;
//	long CurWeight;	
//	long MaxCapacity;
//	unsigned char DecimalPoint;
//	unsigned char Resolution;
//	unsigned char Polarity;
//}Weight;

#define Baud1200  	0X0185
#define Baud2400	0X00C2
#define Baud4800	0X0060
#define Baud9600	0X002F
#define Baud19200 	0X0017
#define Baud38400 	0X000B
#define Baud57600	0X0007
#define Baud115200	0X0003

void UART_Config(void);

extern long MaxCapacity;
extern unsigned char Resolution;

extern unsigned int DecimalPoint;
//#define NO_OF_CH            6
//long EnggWt = 0;
//struct
//{
//	int PowerOnTare;
//	long Password;
//	unsigned char BuadRate;
//	long MinWt;
//	unsigned long LatchDelay;
//	unsigned long CaptureTime;
//	unsigned long Red_Glow;
//	unsigned long Amber_Flash;
//	unsigned long Amber_Glow;
//	unsigned int DeviceID;
//	unsigned char ComMode;	//com-m
//	unsigned char AutoZero;
//	unsigned char TareED;
//    unsigned char LCED[NO_OF_CH];
//    unsigned char InputLogic;
//	unsigned char PLMS_Status;
//	unsigned char Minus_multiply_selection;
//	unsigned long OverWeight;    
//	unsigned long StableToll;    
//	unsigned long StableCntr;    
//	unsigned long TareDelay;    
//	unsigned long BucketWt;    
//	long LatchedWt;    
//    unsigned long Corr_Factor;    
//    unsigned int Periodic_Timer;
//	unsigned char Mobile_Operator;
//	unsigned int AngleConfig;
//    unsigned int No_of_Samples; 
//}Setup;



//extern unsigned char resdata[20];
extern unsigned char curindex;
//char menutmp[8]={0};
/* Contains Two Axles
 * Axle_1 - Loadcell 1 + Loadcell 2  
 * Axle_2 - Loadcell 3 + Loadcell 4 */
/*struct
{
    int Zero;
    int Zero1;
    int Span;
    int CH1_Difference;
    int CH2_Difference;
	int RawADC;
	long CalCapacity;
	long CurWeight;	
    long Corr_Factor;	//Correction Factor

}Axle[Axle_NOs];*/

/*  Total Weight Calculation Summation of all four load cells    */

#define MINWTDISP	0		
#define DUMPLIMIT	10		

#define OP1_ON   IO_RA7_OP1_SetLow();
#define OP1_OFF  IO_RA7_OP1_SetHigh();

#define OP2_ON  IO_RC3_OP2_SetLow();
#define OP2_OFF  IO_RC3_OP2_SetHigh();

extern unsigned char resdata[7];
extern char LcdDispBuff[40];

//struct
//{
//	int RAWADC;
//	long CurWeight;	
//	long LatchedWt;    
//	int TripCount;	
//	unsigned long CumulativeWeight;   
//    char Polarity; 
//}AllChannel;



struct UsedVariables
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

//struct 
//{
//    long Zero;
//    long Zero1;
//    long Span;
//	long CalCapacity;    
//    long RawADC;
//    float CurWeight;	
//    long Corr_Factor;	
//}Loadcell[NO_OF_CH];

struct
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

// Digital Inputs
//#define ILATCH		_RC2		//IP1 
//#define IRUN		_RC1		//IP2 

#define MAXRECORDS 	3000 	// Record Storage Limit 
#define SIZE_REC	14
#define MAX_TRIPCOUNT	 	9999 	// Maximum Trip Count		9999
//char ScanBuff[6]; 
//char digit;	
void UART_SendChar(unsigned char byte);
unsigned char DisplayFlag = 0;

void PowerOn_Read(void);
void Calibration_Backup(char loadcells_or_Voltage);
void Restore_Calib_Backup(char loadcells_or_Voltage);
void LoadDefault(void);

extern void Delay1sec(unsigned char CounterValue);
extern void UART_Init(void);
extern void UART_SendString(char *U_Char,char U_Length);
extern long InternalADC_GetVal(unsigned char CH_No);

void CalcADC(void);
long Calc_Individual_Weight(unsigned char LC_No,long TotalADCCount);
long Calc_Axle_Weight(unsigned char Axle_No,long TotalADCCount);
long RoundOffPos(long value,long round);
void Read_EEPROM_Data(void);

//unsigned char rescmb[7]={1,2,5,10,20,50,100};        //Resolution Combo
//unsigned char E2ErrorFlag = 0;
extern unsigned char CursorIndex;

#define Array_Counter  400
int No_of_Samplesarray[Array_Counter]={0};

int Enb_Volt;
int Total_Volt;
long Ch1Volt,Ch2Volt,Ch3Volt,Ch4Volt,Ch5Volt,Ch6Volt;
unsigned char DummyVar = 3;
void Init_Setup(void);
void PutDp(unsigned char*Tmp,unsigned long deci,char x,char displaymode);
long RoundOff(long value,char round);
void ClearLcdDisplay(char Mode);
void Get_calZero(void);
void Get_calSpan(void);
void Get_calCapacity(void);
void Select_calib_parameters(char zero_or_span_or_Capacity);
void ChannelConfiguration(void);
//Capture Time declaration section start
void Do_CaptureWeight(void);
unsigned char forsendingdata=0;
//#define MAX_CAP_CNT	150
//Iint Cap_Count_1[MAX_CAP_CNT];
//Iint Cap_Count_2[MAX_CAP_CNT];
//Iint Cap_Count_3[MAX_CAP_CNT];
//Iint Cap_Count_4[MAX_CAP_CNT];
//Ilong Cap_Wt[MAX_CAP_CNT];
//int Cap_Cursor;
//int Wt_Avg_Cntr;
//unsigned long CaptureTime;
//unsigned char OverCapPercent = 0;
//volatile unsigned long CaptureTimeCounter = 0;
//long RAM_EnggWt=0;
//Capture Time declaration section end
long StoredWeight = 0;

//struct
//{
//	char Date;
//	char Month;
//	int Year;
//	char Hour;
//	char Minute;
//	char Second;
//	long Weight;
//	unsigned char OLStatus;
//	unsigned char TempFlag;
//	unsigned char Polarity;
//}TripRecord;


//unsigned char WtCur = 0;

void Reset_Cum_Trip();
extern void Confirmation_Function(char selectedparameter);

#define Weight_Display  0
#define Count_Display   1
#define InputSel  0
#define Minussel  1
void WeightDisplay(char Weight_or_Count);
extern char PasswordEntry;
void InputLogic_Multiply_Select(char InputSel_Minussel);
//long AZADCCnt[4] = 0;
void Autozero(char Cur_Loadcell);
void Voltage_Calib_init(void);
extern char DiagnoseFlag;
void Print_CRLF(char No_of_print);
void PowerOnRTC();
void Power_On_KeypressFunction();
void Weight_Calc(void);
extern void Keyfunction();
extern void MainScreenDisplay();
void StoreRecord(char Send_data);
void RangeOutput();
void PayloadProcess_BodyUp();
char GetPassword();
extern void DoMenu(void);
void TareFunction(void);
void PrintResetOption();
void DiagnosticCounts();
void PortCheck();
void Driver_DIPCheck();
void Set_Time();
void RTC_Init();
void Driver_DOPCheck();
void DisplayCheck();
void RTC_Check();
void Get_calzero_calspan(char calzero_calspan);
void assigning_values(void);
//char PrintBuff[50] = {0};
//unsigned int CumTrips=0;
//unsigned long CurDate = 0;
//unsigned long From = 0,End = 0,Cum_Wt = 0;
//unsigned int OLTrips = 0,TripNo = 0; 
unsigned int TripNo = 0; 
void Print_parameters(void);
unsigned int Volt_to_count_Conv(unsigned int Entered_Volt,unsigned int Ref2Count,unsigned int Ref1Count);
unsigned int Difference_Volt =0,Difference_Count=0,final_count =0;
void done_Display(void);
void Voltage_calib_read(void);
void voltage_Calib_write(void);
extern void CANDataFlow(void);
void Append_IO_Status(char *Buffer);

#define Buad_1200     1
#define Buad_2400     2
#define Buad_9600     3
#define Buad_19200    4
#define Buad_57600    5
#define Buad_115200   6
#define ADC_Average_Count 10

long Calf_Volt(int CurCnt,int R1Cnt,int R2Cnt,int R2Volt);
void Get_calzero_calspan_Individual(char calzero_calspan);
//unsigned long StableToll = 0;
unsigned long IndexCntr = 0;
unsigned long ChkStableWt = 0;
//unsigned long ChkStableWt_fortare = 0;
unsigned long StableWtDelayCnt = 0;
//unsigned long StableWtDelayCnt_checkstablewtattare=0;
unsigned char StableWtFlg = 0;
#define MAX_BUFFER	10
//long StableWt[MAX_BUFFER];
long LatchedWts[MAX_BUFFER];
unsigned char WtBuffOverflowFlg = 0;
unsigned char CounterOverflowFlg = 0;
unsigned char Wtstored = 0;
#define NO_OF_CH            6
//unsigned int AdcBuff[NO_OF_CH][ADC_Average_Count];
//unsigned int AdcCounter[NO_OF_CH];
long RetAdc;

long BandAverage(long AdcVal,char Channel);
//unsigned char Dump_Counter = 0;
void ProtocolDump(char BankNumber,char PowerON);
void PC_Dump(void);
unsigned char Display_Deg = 0;
//unsigned long TimerCntr1=0;
void GPRS_Reset_Function(unsigned char mode);
void Operator_Selector(void);
float Average_Wt = 0;
unsigned long Previous_Average_Wt = 0;//it should be a non-zero value 
float Previous_CurWeight = 0;

unsigned char Onetime_average_Counter = 0;
unsigned int CalcLRC(char* ChecksumBuff,unsigned int ByteCount);                //	Calculating LRC for Received protocol	//	Here Received bytes and no.of received bytes are passed
/*struct UsedVar
{
	unsigned char Signal;
	unsigned char ModemStatus;
	unsigned char Active_BankNo;
	unsigned char SendStatus;
	unsigned short Received_Bank[3];
	unsigned short Send_Bank[3];
	
}GPRS;*/
unsigned long TimerCntr2 = 0;
void Periodic_Report_Assign(void);
void Latched_Report_Assign(void);
extern void Record_Management(void);
//unsigned int i = 0;
unsigned char BotDisp = 2;
void Wt_Array_Reset(void);
unsigned char LoadingFlag = 0;

char Header1_Name[20] = {0};
void namechange_via_RS232(void);

extern unsigned long ADCValue,ADCSampleVal;
extern unsigned char Samples; 
extern unsigned long ADCValue,ADCSampleVal;
extern unsigned int *ADC16Ptr;
extern volatile unsigned char ReceivedFlag;
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
#define IPARKING		_RC2		//IP1 
#define IBODYUP 		_RC1		//IP2 
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
extern char PrintBuff[40];
extern unsigned long Latching_Time;
extern unsigned long Periodic_Time;
extern void Rx_ClearVar(void);
extern void Write_E2(const unsigned long Address,char* Data,const int NoofBytes);
extern void Read_E2(char* Data,const unsigned long Address,const int NoofBytes);

//extern unsigned long CAN_BroadcastCntr;
//extern unsigned long CAN_BroadcastTime;
//extern unsigned long CAN_BroadcastFlag;
//extern void Tx_CANReport_Generate(char Req_Data);
//extern void Delay1_1s(unsigned char);

struct 
{
	char Date;
	char Month;
	int Year;
	char Hour;
	char Minute;
	char Second;    
}LastCalib,LastTare;
extern unsigned char Unit;
void PayloadProcess_ParkingBreak(void);
extern unsigned char changeLoadFlag;
#define COM_GPRS_MODULE     1
#define COM_PRINTER         2
#define COM_DUMP            3

#define REDLED_OP1      _RG6    //RED
#define GREENLED_OP2    _RG7
#define AMBERLED_OP3    _RG8
#define OUTPUT1         _RB5
unsigned char OP_Flash_Cntr = 0;    

#define Enable              1
#define Disable             2
#define ParkingBreak    1
#define BodyUp          2
void Status_Write(unsigned char Status);
unsigned char BodyUpFlag = 0;
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
}RecordStore;
extern unsigned long RS232_Dump_Time_Flag;
extern void Rec_Count_read(void);

//FLAGBITS Flags;
//unsigned char Byte1,Byte2,Byte3,Byte4,Byte5,Byte6,Byte7,Byte8;
// Testing lines used for display
//if you terminate below lines testing display will be displayed.
//if Enabled testing display will not come.
//#define Execute
//#define Execute_report
#define Display_testing
//#endif	/* XC_HEADER_TEMPLATE_H */
//unsigned char Dump_Prot_Buff[150] = {0};


extern unsigned char MobileNumBuff[17] ;
extern unsigned char MobileNumRdy ;
extern unsigned long TimeOut_Cnt3;

void Interrupt_Check(void);

unsigned char RT_Temp_Date = 0;
unsigned long RT_Count = 0,RT_Count1 = 0;

