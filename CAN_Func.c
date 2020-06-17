#include "p30fxxxx.h"
#include "Defaults.h"
// data declared for rs232 interface used. interface by either rs232 or canbus 

// to update in e2prom call the required functions available in payloadcu.c
//	update in PackRcvd2 array with starting position as 7.
// 	this routine to be called after all the frames are received. reply to be in 8 bytes

// MSB:LSB from SCADA will be LSB:MSB in the 16 bite register received


#define FCY 		30000000             		// 30 MHz
//#define BITRATE 	250000			 			// 0.250Mbps
//#define NTQ 		20							// Number of Tq cycles which will make the  30000000
unsigned long BITRATE = 1000000;
unsigned long NTQ = 15;
unsigned long BRP_VAL = 20;
/*
#define FCY 		30000000             		// 30 MHz
#define BITRATE 	1000000			 			// 1Mbps
#define NTQ 		15							// Number of Tq cycles which will make the  30000000
*/

#define TOP 1
#define BOTTOM 2

//CAN Bit Timing 
//#define BRP_VAL		((FCY/(2*NTQ*BITRATE))-1)  //Formulae used for C1CFG1bits.BRP // For 250Kbps

#define CAN1 _LATG12 					// STB-SELECT1 line TJA1040
#define CAN2 _LATG14					// STB-SELECT2 line TJA1040

#define ArraySize		150
#define RecordsInABank 	934
#define RecordSize 		70
#define Rec_ArraySize   601
unsigned char CTempVal = 0;
unsigned char RxDateReceived = 0;
unsigned char RxCANPGN1 = 0;
extern unsigned char MCU_Version[2];
extern unsigned int Version;
extern unsigned char RxCANPGN;
extern unsigned int BytesCount;
extern unsigned char PackRcvd2[Rec_ArraySize];
unsigned char ByteArr[8];
char RxMoveData(unsigned char ExSequence);

//---------------------------------------------------------------------
unsigned int OutData0[4] = {0x3030, 0x3030, 0x3030, 0x3030};             
unsigned int OutData1[2] = {0x0000, 0x0000};
unsigned int OutData2[4] = {0x0000, 0x0000, 0x0000, 0x0000};            
unsigned int OutData3[2] = {0x0000, 0x0000};

// Intilializing the receive registers to be 0

unsigned int InData0[4] = {0, 0, 0, 0};
unsigned int InData1[4] = {0, 0, 0, 0};
unsigned int InData2[4] = {0, 0, 0, 0};
unsigned int InData3[4] = {0, 0, 0, 0};

unsigned char TXStartFlag = 0;

unsigned char C1T0_flag = 0;
unsigned char C1T1_flag = 0;
unsigned char C1R0_flag = 0;
unsigned char C1R1_flag = 0;
unsigned char C1ER_flag = 0;

unsigned char C2T0_flag = 0;
unsigned char C2T1_flag = 0;
unsigned char C2R0_flag = 0;
unsigned char C2R1_flag = 0;
unsigned char C2ER_flag = 0;

/* ERROR FLAGS */
unsigned char  C1IVR_flag = 0;
unsigned char  C1EWARN_flag = 0;
unsigned char  C1RXWAR_flag = 0;
unsigned char  C1TXWAR_flag = 0;
unsigned char  C1RXEP_flag = 0;
unsigned char  C1TXEP_flag = 0;
unsigned char  C1TXBO_flag = 0;

unsigned char  C2IVR_flag = 0;
unsigned char  C2EWARN_flag = 0;
unsigned char  C2RXWAR_flag = 0;
unsigned char  C2TXWAR_flag = 0;
unsigned char  C2RXEP_flag = 0;
unsigned char  C2TXEP_flag = 0;
unsigned char  C2TXBO_flag = 0;

unsigned char Byte1,Byte2,Byte3,Byte4,Byte5,Byte6,Byte7,Byte8;
unsigned long Received_PGN = 0;
unsigned char Tx_IDFlag = 0;
char FrameSequenceNumber = 0;

extern FLAGBITS Flags;

unsigned char CurrFrame,ExpFrame,RMaxFrames;

struct
{
	unsigned Priority : 3;
	unsigned Reserved : 1;
	unsigned DataPage : 1;
	unsigned PF		  :	8;
	unsigned PS		  : 8;
	unsigned SAddress : 8;
}CANIdentifier_29,CANRecIdentifier_29;
struct
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

extern struct tmr 
{
	unsigned StartTimer     :1;     
	unsigned TimerOver      :1;
	unsigned int TimerValue;
};
extern struct tmr CanFTimer;
extern struct tmr CanCTimer;

extern struct
{
	unsigned long DeviceID;	 					
	unsigned long Password;
	unsigned long BuadRate;
    unsigned long CAN_BuadRate;
}System_setting;

void ClearRx_CAN_ID(void);
void SendCANdata(char SequenceNo, unsigned char Byte1, unsigned int Byte23, unsigned int Byte45, unsigned int Byte67);
void Can_Init(void);
void CANDataFlow(void);
extern void CAN_Send(void);
extern void CAN_ID(void);

unsigned char LSBByte(unsigned int Buff);
unsigned char MSBByte(unsigned int Buff);
unsigned int From2Bytes(char LSBByte, char MSBByte);

/* CAN-USB related Variaables */
extern void FormRecord(unsigned char uartNo,unsigned char Excel_or_other);
extern unsigned char selectformat;
int AvailTrip = 100;
int TripRecordad = 0;
int RecordsRead = 0;
int RecordsAvailable = 51;
int RecordOffset = 0;
int m_ReadCounter = 0;
int startingPoint = 0;
#define NoOfRecords    3
#define MAXRECORDS    3000

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5

void Tx_LatestRec_Details(char Mode);
void Tx_LatchedWt_Fun(void);
void Tx_CumulativeWt_Fun(void);
void Tx_TripNumber(void);
unsigned int TripCount;
#define Tx_TripCount            0
#define Tx_LatchedWt            1
#define Tx_CurrentWt            2
#define Tx_CumulativeWt         3
#define Tx_Individual_Ch_Wt     4
#define Tx_Individual_Axle_Wt   5
#define EEPROM_STORING_ADD  5000

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

extern struct
{
	int RAWADC;
	long CurWeight;	
	long LatchedWt;    
	int TripCount;	
	unsigned long CumulativeWeight;   
    char Polarity; 
}AllChannel;

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
extern unsigned int DecimalPoint;
extern unsigned char Unit;
extern char LcdDispBuff[40];
void Tx_CANReport_Generate(char Req_Data);
void Can_Init(void)
{
//	 Change to Configuration Mode from Normal Operation Mode
	 C1CTRLbits.REQOP = 4;
	 C2CTRLbits.REQOP = 4;
	 while(C1CTRLbits.OPMODE != 4);//Wait for CAN1 mode change from Configuration Mode to Normal mode 
	 while(C2CTRLbits.OPMODE != 4);//Wait for CAN2 mode change from Configuration Mode to Normal mode
	//--------------------------------------------------------------------------------------------------------------------
						//Initialization of CAN1 Module and Enabling of CAN1 Interrupts
	//--------------------------------------------------------------------------------------------------------------------
    
	 CAN1 = 0;
	 C1CTRLbits.CANCKS = 0;			// Select the CAN Master Clock . It is equal to Fcy here. 
	 								// equal to Fcy.(Fcy=30Mhz)
	 C1CFG1bits.SJW = 0;			//Synchronized jump width time is 1 x TQ when SJW is equal to 00
	
     if(System_setting.CAN_BuadRate == 1)
     {
         BITRATE = 250000;
         NTQ = 20;
         C1CFG1bits.BRP = 2;		//((FCY/(2*NTQ*BITRATE))-1) 	
         C1CFG2 = 0x02bf;                              // Each bit time is 20Tq	//250K	with CANCKS=0 & SJW = 0;	Previous Version used
//         sprintf(LcdDispBuff,"%3lu:%2lu: 1 ",(BITRATE/1000),System_setting.CAN_BuadRate);   
//        LcdDisplay(BOTTOM,LcdDispBuff);
//        Delay1_1s(12);
     }
     else if(System_setting.CAN_BuadRate == 2)
     {
         BITRATE = 500000;
         NTQ = 10;
         C1CFG1bits.BRP = 2;		//((FCY/(2*NTQ*BITRATE))-1) 	
         C1CFG2 = 0x0086;
//         sprintf(LcdDispBuff,"%3lu:%2lu: 2 ",(BITRATE/1000),System_setting.CAN_BuadRate);   
//        LcdDisplay(BOTTOM,LcdDispBuff);
//        Delay1_1s(12);
     }
     else if(System_setting.CAN_BuadRate == 3)
     {
         BITRATE = 1000000;
         NTQ = 15;
         C1CFG1bits.BRP = 0;		//((FCY/(2*NTQ*BITRATE))-1) 	
         C1CFG2 = 0x01bb;
//         sprintf(LcdDispBuff,"%3lu:%2lu: 3 ",(BITRATE/1000),System_setting.CAN_BuadRate);   
//        LcdDisplay(BOTTOM,LcdDispBuff);
//        Delay1_1s(12);
     }

//     if(System_setting.CAN_BuadRate == 1)
//         C1CFG2 = 0x02bf;                              // Each bit time is 20Tq	//250K	with CANCKS=0 & SJW = 0;	Previous Version used
//     else if(System_setting.CAN_BuadRate == 2)
//         C1CFG2 = 0x0086;
//     else if(System_setting.CAN_BuadRate == 3)
//         C1CFG2 = 0x01bb;
//	 C1CFG2 = 0x04B8;                              // Each bit time is Tq	//250K	with CANCKS=1 & SJW = 1;
//	 C1CFG2 = 0x02bf;                              // Each bit time is 20Tq	//250K	with CANCKS=0 & SJW = 0;	Previous Version used
//	 C1CFG2 = 0x01bb;                              // Each bit time is 15Tq	//1Mbps	with CANCKS=0 & SJW = 3;
//	 C1CFG2 = 0x00b9;

	//Interrupt Section of CAN1 Peripheral
	 C1INTF = 0;					//Reset all The CAN Interrupts 
	 IFS1bits.C1IF = 0;  			//Reset the Interrupt Flag status register
	 C1INTE = 0x00FF;               //Enable all CAN interrupt sources
	 IEC1bits.C1IE = 1;				//Enable the CAN1 Interrupt 
	
	//-------------------------------------------------------------------------------------------------------------------
						//Initialization of CAN2 Module and Enabling of CAN2 Interrupts
	//-------------------------------------------------------------------------------------------------------------------
	CAN2 = 0;
	C2CTRLbits.CANCKS = 0;			// Select the CAN Module Frequency Here CAN Master Clk is 
									// equal to Fcy.(Fcy=30Mhz)
	C2CFG1bits.SJW=0;			   	//Synchronized jump width time is 1 x TQ when SJW is equal to 00
	 
	C2CFG1bits.BRP = BRP_VAL;		//((FCY/(2*NTQ*BITRATE))-1)  2 or 3

//	C2CFG1 = 0x0002;
//	C2CFG2 = 0x0086;
	C2CFG2 = 0x02bf;                // SEG1PH=6Tq, SEG2PH=3Tq, PRSEG=5Tq Previous Version used
//	C2CFG2 = 0x01bb;
//	C2CFG2 = 0x00b9;
                            
//	Interrupt Section of CAN2 Peripheral	
	 C2INTF = 0;					//Reset all The CAN Interrupts 
	 IFS2bits.C2IF = 0;  			//Reset the Interrupt Flag status register
	 C2INTE = 0x00FF;               //Enable all interrupt sources
	 IEC2bits.C2IE = 1;				//Enable the CAN2 Interrupt

	//-----------------------------------------------------------------------------------------------------------------------
						// Configure Receive registers, Filters and Masks
	//-----------------------------------------------------------------------------------------------------------------------
	
	//  We are initializing the Receive Buffer 0 and Receive Buffer 1 for CAN1 and CAN2 
	//	RXF0 and RXF1 filters with the RXM0 mask are associated with RXB0.
	//	The filters RXF2, RXF3, RXF4 and RXF5 and the mask RXM1 are associated with RXB1.
	//	the number of the filter that enabled the message reception is indicated in the CiRXnCON register via the FILHIT bits
	 C1RX0CON = C1RX1CON = C2RX0CON = C2RX1CON = 0x0000; 	// Receive Buffer1 and 0 Status
	 														//and Control Register for CAN1 and CAN2

	//Initializing of Acceptance Filter n Standard Identifier for CAN1
    // ?18F200FF? -   ?00011000111100100000000011111111?
    //              00011000111100 100000000011 111111?
	C1RXF0SID=0x18F1;   // 0001100011110001
	C1RXF0EIDH=0x0803;  // 100000000011
	C1RXF0EIDL=0xFC00;  // 111111?0000000000
	
	C1RXF1SID=0x18F1;
	C1RXF1EIDH=0x0803;
	C1RXF1EIDL=0xFC00;

	C1RXM0SID=0xFFFF;
	C1RXM0EIDH=0xFC03;
	C1RXM0EIDL=0xFFFF;

	C1RXF2SID=0xFFFF;
	C1RXF2EIDH=0xFFFF;
	C1RXF2EIDL=0xFFFF;

	C1RXF3SID=0xFFFF;
	C1RXF3EIDH=0xFFFF;
	C1RXF3EIDL=0xFFFF;

	C1RXF4SID=0xFFFF;
	C1RXF4EIDH=0xFFFF;
	C1RXF4EIDL=0xFFFF;

	C1RXF5SID=0xFFFF;	//0001100011110001
	C1RXF5EIDH=0xFFFF;  
	C1RXF5EIDL=0xFFFF;	//0000110000000000

	C1RXM1SID=0xFFFF;
	C1RXM1EIDH=0xFFFF;
	C1RXM1EIDL=0xFFFF;

	//Initializing of Acceptance Filter n Standard Identifier for CAN2
	C2RXF0SID=0xFFFF;
	C2RXF0EIDH=0xFFFF;
	C2RXF0EIDL=0xFFFF;
	
	C2RXF1SID=0xFFFF;
	C2RXF1EIDH=0xFFFF;
	C2RXF1EIDL=0xFFFF;

	C2RXM0SID=0xFFFF;
	C2RXM0EIDH=0xFC03;
	C2RXM0EIDL=0xFFFF;
	
	C2RXF2SID=0xFFFF;
	C2RXF2EIDH=0xFFFF;
	C2RXF2EIDL=0xFFFF;

	C2RXF3SID=0xFFFF;
	C2RXF3EIDH=0xFFFF;
	C2RXF3EIDL=0xFFFF;
	
	C2RXF4SID=0xFFFF;
	C2RXF4EIDH=0xFFFF;
	C2RXF4EIDL=0xFFFF;

	C2RXF5SID=0xFFFF;
	C2RXF5EIDH=0xFFFF;
	C2RXF5EIDL=0xFFFF;

	C2RXM1SID=0xFFFF;
	C2RXM1EIDH=0xFFFF;
	C2RXM1EIDL=0xFFFF;

	 //-----------------------------------------------------------------------------------------------------------------------
							// Configure Transmit Registers Buffer 0 and Transmit Buffer 1
	//-----------------------------------------------------------------------------------------------------------------------
	
	 C1TX0CON = 0x0003;		// highest interrmediate priority
	 C1TX0SID = 0x4089;     // SID - 0x222  // extended identifier 
	 C1TX0EID = 0x0000;     // EID 
	 C1TX0DLC = 0x01C0;		//Select the Data word Length for CAN1 Transmit Buffer0 which is 8 byte  
	
	 C1TX1CON = 0x0003;		// highest interrmediate priority
	 C1TX1SID = 0x4089;     // SID - 0x222  // extended identifier 
	 C1TX1EID = 0x0000;     // EID 
	 C1TX1DLC = 0x01C0;		//Select the Data word Length for CAN1 Transmit Buffer0 which is 8 byte  

	 C1TX2CON = 0x0003;		// highest interrmediate priority
	 C1TX2SID = 0x4089;     // SID - 0x222  // extended identifier 
	 C1TX2EID = 0x0000;     // EID 
	 C1TX2DLC = 0x01C0;		//Select the Data word Length for CAN1 Transmit Buffer0 which is 8 byte  


	 C2TX0CON = 0x0003;		// highest interrmediate priority
	 C2TX0SID = 0x4089;     // SID - 0x222  // extended identifier 
	 C2TX0EID = 0x0000;     // EID 
	 C2TX0DLC = 0x01C0;		//Select the Data word Length for CAN1 Transmit Buffer0 which is 8 byte  
	
	 C2TX1CON = 0x0003;		// highest interrmediate priority
	 C2TX1SID = 0x4089;     // SID - 0x222  // extended identifier 
	 C2TX1EID = 0x0000;     // EID 
	 C2TX1DLC = 0x01C0;		//Select the Data word Length for CAN1 Transmit Buffer0 which is 8 byte  

	 C2TX2CON = 0x0003;		// highest interrmediate priority
	 C2TX2SID = 0x4089;     // SID - 0x222  // extended identifier 
	 C2TX2EID = 0x0000;     // EID 
	 C2TX2DLC = 0x01C0;		//Select the Data word Length for CAN1 Transmit Buffer0 which is 8 byte  
	 
	 // Data Field 1,Data Field 2, Data Field 3, Data Field 4 // 8 bytes selected by DLC 
	
	 C1TX0B1 = 0xFFFF;
	 C1TX0B2 = 0xFFFF;
	 C1TX0B3 = 0xFFFF;
	 C1TX0B4 = 0xFFFF;

	 C2TX0B1 = 0xFFFF;
	 C2TX0B2 = 0xFFFF;
	 C2TX0B3 = 0xFFFF;
	 C2TX0B4 = 0xFFFF;

//	 Change to Loopback Operation Mode from Configuration Mode
	 C1CTRLbits.REQOP = 0;
	 C2CTRLbits.REQOP = 0;

	 while(C1CTRLbits.OPMODE != 0);//Wait for CAN1 mode change from Configuration Mode to Normal mode 
	 while(C2CTRLbits.OPMODE != 0);//Wait for CAN2 mode change from Configuration Mode to Normal mode

	 //Enable transmission
// 	C1TX0CONbits.TXREQ = 1; 
// 	C1TX1CONbits.TXREQ = 1;	 
//	C2TX0CONbits.TXREQ = 1; 
// 	C2TX1CONbits.TXREQ = 1;

	 C1T0_flag = 1;
	 C2T0_flag = 1;
     CAN1 = 1;
     CAN2 = 1;
}
//--------------------------------------------------------------------------------------------------------------------------
											//Interrupt Section for CAN1
//--------------------------------------------------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _C1Interrupt(void)
{
    IFS1bits.C1IF = 0;         //Clear interrupt flag
    if(C1INTFbits.TX0IF)
    {
       C1T0_flag = 1;
       C1INTFbits.TX0IF = 0;  //If the Interrupt is due to Transmit0 of CAN1 Clear the Interrupt	
      C1TX0CONbits.TXREQ = 0;
    }  
    else if(C1INTFbits.TX1IF)
    {
      C1T1_flag = 1;  
      C1INTFbits.TX1IF = 0;   //If the Interrupt is due to Transmit1 of CAN1 Clear the Interrupt 
      C1TX0CONbits.TXREQ = 0;
    }  

    if(C1INTFbits.RX0IF)
    {
      C1R0_flag = 1;
      C1INTFbits.RX0IF = 0; 	//If the Interrupt is due to Receive0 of CAN1 Clear the Interrupt
      if(C1RX0CONbits.RXFUL)
      {
          InerruptCheck_CAN_ID();
      }
      C1RX0CONbits.RXFUL = 0;
    }
    else if(C1INTFbits.RX1IF)
    {
      C1R1_flag = 1;
      C1INTFbits.RX1IF = 0;  	//If the Interrupt is due to Receive1 of CAN1 Clear the Interrupt
      if(C1RX1CONbits.RXFUL) {
          InerruptCheck_CAN_ID();
      }
      C1RX1CONbits.RXFUL = 0;  // clear indication of buffer free
    }

	if(C1INTFbits.TXWAR) {
		C1INTFbits.TXWAR = 0; C1TXWAR_flag = 1;
	}
	if(C1INTFbits.RXWAR) {
		C1INTFbits.RXWAR = 0; C1RXWAR_flag = 1;
	}
	if(C1INTFbits.TXEP) {
		C1INTFbits.TXEP = 0; C1TXEP_flag = 1;
	}
	if(C1INTFbits.RXEP) {
		C1INTFbits.RXEP = 0; C1RXEP_flag = 1;
	}
	if(C1INTFbits.TXBO) {
		C1INTFbits.TXBO = 0; C1TXBO_flag = 1;
	}
}
//--------------------------------------------------------------------------------------------------------------------------
											//Interrupt Section for CAN2
//--------------------------------------------------------------------------------------------------------------------------

void __attribute__((interrupt, no_auto_psv)) _C2Interrupt(void)
{
    IFS2bits.C2IF = 0;         //Clear interrupt flag
	if(C2INTFbits.TX0IF)
	{
		C2T0_flag = 1;
		C2INTFbits.TX0IF = 0;  //If the Interrupt is due to Transmit0 of CAN1 Clear the Interrupt	
		C2TX0CONbits.TXREQ = 0;
	}  
	else if(C2INTFbits.TX1IF)
	{
		C2T1_flag = 1;  
		C2INTFbits.TX1IF = 0;   //If the Interrupt is due to Transmit1 of CAN1 Clear the Interrupt 
		C2TX0CONbits.TXREQ = 0;
	}
    if(C2INTFbits.RX0IF)
	{
		C2R0_flag = 1;
		C2INTFbits.RX0IF = 0; 	//If the Interrupt is due to Receive0 of CAN1 Clear the Interrupt
		if(C2RX0CONbits.RXFUL)
		{
//            InerruptCheck_CAN2_ID();
		}
		C2RX0CONbits.RXFUL = 0;
	}
	else if(C2INTFbits.RX1IF)
	{
		C2R1_flag = 1;
		C2INTFbits.RX1IF = 0;  	//If the Interrupt is due to Receive1 of CAN1 Clear the Interrupt
//		InerruptCheck_CAN2_ID();
		C2RX1CONbits.RXFUL = 0;  // clear indication of buffer free
	}
	if(C2INTFbits.TXWAR) {
		C2INTFbits.TXWAR = 0; C2TXWAR_flag = 1;
	}
	if(C2INTFbits.RXWAR) {
		C2INTFbits.RXWAR = 0; C2RXWAR_flag = 1;
	}
	if(C2INTFbits.TXEP) {
		C2INTFbits.TXEP = 0; C2TXEP_flag = 1;
	}
	if(C2INTFbits.RXEP) {
		C2INTFbits.RXEP = 0; C2RXEP_flag = 1;
	}
	if(C2INTFbits.TXBO) {
		C2INTFbits.TXBO = 0; C2TXBO_flag = 1;
	}
}
void CAN_ID(void)
{
	CANIdentifier_29.Priority = Tx_CANId.Priority;			//6 constant
	CANIdentifier_29.Reserved = Tx_CANId.Reserved;			//0 constant
	CANIdentifier_29.DataPage = Tx_CANId.DataPage;			//0 constant
	CANIdentifier_29.PF = Tx_CANId.PF;						//241 PDU format
	CANIdentifier_29.PS = Tx_CANId.PS;						// PDU Specific
	CANIdentifier_29.SAddress = Tx_CANId.SAddress;			//255 Broad cast
    
	C1TX0SIDbits.SID10_6 = (CANIdentifier_29.Priority << 2) | (CANIdentifier_29.Reserved << 1)| CANIdentifier_29.DataPage;
	C1TX0SIDbits.SID5_0 = (CANIdentifier_29.PF >> 2);
	C1TX0EIDbits.EID17_14 = ((CANIdentifier_29.PF << 6) >> 4) | (CANIdentifier_29.PS >> 6);
	C1TX0EIDbits.EID13_6 = (CANIdentifier_29.PS << 2)| (CANIdentifier_29.SAddress >> 6);
	C1TX0DLCbits.EID5_0 = (CANIdentifier_29.SAddress << 2 ) >> 2;

	C1TX0SIDbits.TXIDE = 1;	// enable extended ID
	C1TX0SIDbits.SRR = 0;	// Not used in EX ID
	C1TX0DLCbits.DLC = 8;	// data length 8
	C1TX0DLCbits.TXRTR = 0;	// normal message ( no RTR)
}
void CAN_Send()
{
	if (Flags.InitCAN) return;
	if (C1ER_flag == 1) { return;}
    CAN1 = 0;Nop();
    CAN2 = 0;Nop();
	if( (C1T0_flag == 1) && (C1TX0CONbits.TXREQ == 0) )
	{			
		C1T0_flag = 0; 

		C1TX0B1 = OutData0[0];Nop();
	  	C1TX0B2 = OutData0[1];Nop();
	 	C1TX0B3 = OutData0[2];Nop();
	 	C1TX0B4 = OutData0[3];Nop();

		C1TX0CONbits.TXREQ = 1;
        C1TX0CONbits.TXABT = 0;
		C1TX0CONbits.TXLARB = 0;
		C1TX0CONbits.TXERR = 0;Nop();
		C1TX0CONbits.TXPRI = Tx_CANId.Priority;Nop();
	}
	CanCTimer.StartTimer=0;CanCTimer.TimerValue=2;CanCTimer.TimerOver=0;CanCTimer.StartTimer=1;

    while(C1TX0CONbits.TXREQ)
	{
		Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
		if(C1INTFbits.ERRIF) {
			C1INTFbits.ERRIF = 0;C1ER_flag = 1;
		}
		if (CanCTimer.TimerOver) {C1ER_flag=1;}
		if(C1ER_flag == 1) {
			//C1ER_flag=0;Flags.InitCAN=1;
			CanFTimer.StartTimer=0;CanFTimer.TimerValue = 5;CanFTimer.StartTimer=1;
            CAN1 = 1;Nop();
            CAN2 = 1;Nop();	
			return;
		}
	}	
    CAN1 = 1;Nop();
    CAN2 = 1;Nop();	
}
unsigned int extractInt(unsigned int orig16BitWord, unsigned from, unsigned to) 
{
  unsigned mask = ( (1<<(to-from+1))-1) << from;
  return (orig16BitWord & mask) >> from;
}
void StoreRcvdData(void)
{
    if(C1R0_flag == 1)
	{
		if(C1RX0CONbits.RXRTRRO)
		{
	//			RTR_flag = 1;		
		}
		else
		{
            InData0[0] = C1RX0B1; 
            InData0[1] = C1RX0B2;  	//Move the recieve data from Buffers to InData
            InData0[2] = C1RX0B3;	
            InData0[3] = C1RX0B4;
		}
	}
	if(C1R1_flag == 1)
	{
		if(C1RX1CONbits.RXRTRRO)
		{
	//				RTR_flag = 1;			
		}
		else
		{
			InData0[0] = C1RX1B1;   //Move the  data received to Indata Registers
			InData0[0] = C1RX1B2;
			InData0[0] = C1RX1B3;
			InData0[0] = C1RX1B4;
		}
	}
	return;
}
/****************************************************************
Function name 	: void InerruptCheck_CAN_ID(void);
Returns 		: None
Arguments		: void
Date created 	: 25_July_2017
Description 	: Used to Extract Received CAN parameters in interrupt
****************************************************************/
void InerruptCheck_CAN_ID(void)
{
	unsigned long TempVar = 0;
	unsigned int SourceAddress = 0;
    
	if(C1R0_flag == 1)
	{
		Extract_CANId.Priority = extractInt(C1RX0SID,10,12);
		Extract_CANId.Reserved = extractInt(C1RX0SID,9,9);
		Extract_CANId.DataPage = extractInt(C1RX0SID,8,8);
		Extract_CANId.PF = extractInt(C1RX0SID,2,7);
		Extract_CANId.SRR = extractInt(C1RX0SID,1,1);
		Extract_CANId.RxIDE = extractInt(C1RX0SID,0,0);
	
		Extract_CANId.PF = (Extract_CANId.PF << 2);
		Extract_CANId.PF = (Extract_CANId.PF | extractInt(C1RX0EID,10,11) );
		Extract_CANId.PS = extractInt(C1RX0EID,2,9);
		Extract_CANId.SAddress = extractInt(C1RX0EID,0,1);
	
		Extract_CANId.SAddress = (Extract_CANId.SAddress << 6);
		Extract_CANId.SAddress = (Extract_CANId.SAddress | extractInt(C1RX0DLC,10,15) );
		Extract_CANId.RXRTR = extractInt(C1RX0DLC,9,9);	//C1RX0CONbits.RXRTRRO
		Extract_CANId.RxRB1 = extractInt(C1RX0DLC,8,8);
		Extract_CANId.RxRB0 = extractInt(C1RX0DLC,4,4);
		Extract_CANId.DLC = extractInt(C1RX0DLC,0,3);

		TempVar = Extract_CANId.Reserved;
		Received_PGN = (TempVar << 17);
		TempVar = Extract_CANId.DataPage;
		Received_PGN = (Received_PGN | (TempVar << 16) );
		TempVar = Extract_CANId.PF;		//1byte(char) assigned to 2bytes(int) then left shift 8.
		Received_PGN = (Received_PGN | (TempVar << 8) );
		Received_PGN = (Received_PGN | Extract_CANId.PS );
	}

	if(C1R1_flag == 1)
	{
		Extract_CANId.Priority = extractInt(C1RX1SID,10,12);
		Extract_CANId.Reserved = extractInt(C1RX1SID,9,9);
		Extract_CANId.DataPage = extractInt(C1RX1SID,8,8);
		Extract_CANId.PF = extractInt(C1RX1SID,2,7);
		Extract_CANId.SRR = extractInt(C1RX1SID,1,1);
		Extract_CANId.RxIDE = extractInt(C1RX1SID,0,0);
	
		Extract_CANId.PF = (Extract_CANId.PF << 2);
		Extract_CANId.PF = (Extract_CANId.PF | extractInt(C1RX1EID,10,11) );
		Extract_CANId.PS = extractInt(C1RX1EID,2,9);
		Extract_CANId.SAddress = extractInt(C1RX1EID,0,1);
	
		Extract_CANId.SAddress = (Extract_CANId.SAddress << 6);
		Extract_CANId.SAddress = (Extract_CANId.SAddress | extractInt(C1RX1DLC,10,15) );
		Extract_CANId.RXRTR = extractInt(C1RX1DLC,9,9);	//C1RX0CONbits.RXRTRRO
		Extract_CANId.RxRB1 = extractInt(C1RX1DLC,8,8);
		Extract_CANId.RxRB0 = extractInt(C1RX1DLC,4,4);
		Extract_CANId.DLC = extractInt(C1RX1DLC,0,3);

		TempVar = Extract_CANId.Reserved;
		Received_PGN = (TempVar << 17);
		TempVar = Extract_CANId.DataPage;
		Received_PGN = (Received_PGN | (TempVar << 16) );
		TempVar = Extract_CANId.PF;		//1byte(char) assigned to 2bytes(int) then left shift 8.
		Received_PGN = (Received_PGN | (TempVar << 8) );
		Received_PGN = (Received_PGN | Extract_CANId.PS );
	}
    if(Extract_CANId.SAddress == 255)
    {
        if(Received_PGN == (unsigned long)61952)      // ID : 18F200FF
        {
            RxCANPGN = 1;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        else if(Received_PGN == (unsigned long)61953)	// ID : 18F201FF
        {
            RxCANPGN = 2;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        else if(Received_PGN == (unsigned long)61954)	// ID : 18F202FF
        {
            RxCANPGN = 3;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        else if(Received_PGN == (unsigned long)61955)	// ID : 18F203FF
        {
            RxCANPGN = 4;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        else if(Received_PGN == (unsigned long)61956)	// ID : 18F204FF    
        {
            RxCANPGN = 5;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        else if(Received_PGN == (unsigned long)61957)	// ID : 18F205FF    
        {
            RxCANPGN = 6;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        else if(Received_PGN == (unsigned long)61958)	// ID : 18F206FF    
        {
            RxCANPGN = 7;
            Tx_IDFlag = 1;
            StoreRcvdData();
        }
        C1R0_flag = 0;
        C1R1_flag = 0;
        Received_PGN = 0;
        if(Tx_IDFlag == 1)
        {
            Tx_CANId.Priority = Extract_CANId.Priority;
            Tx_CANId.Reserved = Extract_CANId.Reserved;
            Tx_CANId.DataPage = Extract_CANId.DataPage;
            Tx_CANId.PF = Extract_CANId.PF;
            Tx_CANId.SRR = Extract_CANId.SRR;
            Tx_CANId.RxIDE = Extract_CANId.RxIDE;

            Tx_CANId.PS = Extract_CANId.PS;
            Tx_CANId.SAddress = Extract_CANId.SAddress;

            Tx_CANId.RXRTR = Extract_CANId.RXRTR;
            Tx_CANId.RxRB1 = Extract_CANId.RxRB1;
            Tx_CANId.RxRB0 = Extract_CANId.RxRB0;
            Tx_CANId.DLC = Extract_CANId.DLC;
            Tx_IDFlag = 0;
        }
    }
}

void SendCANdata(char SequenceNo, unsigned char Byte1, unsigned int Byte23, unsigned int Byte45, unsigned int Byte67)
{
	CAN_ID();
	OutData0[0] = From2Bytes(SequenceNo,Byte1);
	OutData0[1] = Byte23;
	OutData0[2] = Byte45;
	OutData0[3] = Byte67;
	CAN_Send();
    
}
/****************************************************************
Function name 	: void ClearRx_CAN_ID(void);
Returns 		: None
Arguments		: void
Date created 	: 14_March_2017
Description 	: Used to Clear Received CAN parameters
****************************************************************/
void ClearRx_CAN_ID(void)
{
	Extract_CANId.Priority = 0;
	Extract_CANId.Reserved = 0;
	Extract_CANId.DataPage = 0;
	Extract_CANId.PF = 0;
	Extract_CANId.SRR = 0;
	Extract_CANId.RxIDE = 0;
	Extract_CANId.PS = 0;
	Extract_CANId.SAddress = 0;
	Extract_CANId.RXRTR = 0;
	Extract_CANId.RxRB1 = 0;
	Extract_CANId.RxRB0 = 0;
	Extract_CANId.DLC = 0;
	Received_PGN = 0;

	Tx_CANId.Priority = 0;
	Tx_CANId.Reserved = 0;
	Tx_CANId.DataPage = 0;
	Tx_CANId.PF = 0;
	Tx_CANId.SRR = 0;
	Tx_CANId.RxIDE = 0;
	Tx_CANId.PS = 0;
	Tx_CANId.SAddress = 0;
	Tx_CANId.RXRTR = 0;
	Tx_CANId.RxRB1 = 0;
	Tx_CANId.RxRB0 = 0;
	Tx_CANId.DLC = 0;
	Tx_IDFlag = 0;
}

/****************************************************************
Function name 	: void CANDataFlow(void);
Returns 		: None
Arguments		: void
Date created 	: 04_10_2017(Added from Payload-II-MCU-6.13)
Description 	: Used to identify the CAN ID and based on CAN ID, separate the process
****************************************************************/
void CANDataFlow(void)
{
    if (Flags.InitCAN) {RxCANPGN=0;return;}
    if (RxCANPGN==0) return;
    
	if(RxCANPGN == 1)
	{
    	Tx_CANReport_Generate(Tx_TripCount);				// ID : 18F200FF
	}
	else if(RxCANPGN == 2)
	{
		Tx_CANReport_Generate(Tx_LatchedWt);				// ID : 18F201FF
	}
	else if(RxCANPGN == 3)
	{
		Tx_CANReport_Generate(Tx_CurrentWt);				// ID : 18F202FF
	}
	else if(RxCANPGN == 4)
	{
		Tx_CANReport_Generate(Tx_CumulativeWt);		    	// ID : 18F203FF
	}
	else if(RxCANPGN == 5)
	{
		Tx_LatestRec_Details(1);                            // ID : 18F204FF	
	}
	else if(RxCANPGN == 6)
	{
		Tx_CANReport_Generate(Tx_Individual_Ch_Wt);			// ID : 18F205FF
	}
	else if(RxCANPGN == 7)
	{
		Tx_CANReport_Generate(Tx_Individual_Axle_Wt);       // ID : 18F206FF
	}
    ClearRx_CAN_ID();
	Received_PGN = 0;
	RxCANPGN = 0;
	C1R0_flag = 0;
	C1R1_flag = 0;
	C2R0_flag = 0;
	C2R1_flag = 0;
	FrameSequenceNumber = 1;
}

/****************************************************************
Function name 	: void Tx_TripNumber(void);
Returns 		: None
Date created 	: 04_10_2017(Added from Payload-II-MCU-6.13)
Description 	: Used to send curent Trip nubmer of the truck through CAN.
****************************************************************/
void Tx_CANReport_Generate(char Req_Data)
{
	unsigned char Reserved = 255;
    unsigned char IO_Status = 0;
	unsigned int LatchedWeight = 0;
    unsigned long CLoad = 0;    
    
    if(Req_Data == Tx_TripCount)
    {
        TripCount = AllChannel.TripCount;
        Byte1 = (unsigned char)(0x01);//sequence
        Byte2 = (unsigned char)(TripCount >> 8) ;
        Byte3 = (unsigned char)((TripCount << 8) >> 8);
        Byte4 = Reserved;
        Byte5 = Reserved;
        Byte6 = Reserved;
        Byte7 = Reserved;
        Byte8 = Reserved;    
        SendCANdata(Byte1, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));        
    }
    else if(Req_Data == Tx_LatchedWt)
    {
        if(Bits.Disp_Latch_Wt == 1)
            LatchedWeight = AllChannel.LatchedWt;
        else
            LatchedWeight = 0;

        Byte1 = (unsigned char)(0x01);//sequence
        Byte2 = (unsigned char)DecimalPoint;
        Byte3 = ' ';//use it as polarity
        Byte4 = (unsigned char)((AllChannel.LatchedWt & 0xFF000000) >> 24);
        Byte5 = (unsigned char)((AllChannel.LatchedWt & 0x00FF0000) >> 16);
        Byte6 = (unsigned char)((AllChannel.LatchedWt & 0x0000FF00) >> 8);
        Byte7 = (unsigned char)(AllChannel.LatchedWt & 0x000000FF);
        Byte8 = (unsigned char)Unit;//unit 1-KG,2-TON        
        SendCANdata(Byte1, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));        
    }
    else if((Req_Data == Tx_CurrentWt) ||(Req_Data == Tx_CumulativeWt))
    {
        if(Req_Data == Tx_CurrentWt)        
        {
            if(AllChannel.CurWeight < 0)
            {
                CLoad = labs(AllChannel.CurWeight);
                Byte3 = '-';//use it as polarity
            }
            else
            {
                CLoad = (unsigned long)AllChannel.CurWeight;
                Byte3 = ' ';//use it as polarity
            }
            Byte1 = (unsigned char)(0x01);//sequence
            Byte2 = (unsigned char)DecimalPoint;
            Byte4 = (unsigned char)((CLoad & 0xFF000000) >> 24);
            Byte5 = (unsigned char)((CLoad & 0x00FF0000) >> 16);
            Byte6 = (unsigned char)((CLoad & 0x0000FF00) >> 8);
            Byte7 = (unsigned char)(CLoad & 0x000000FF);
            Byte8 = (unsigned char)Unit;//unit 1-KG,2-TON           
//            CLoad = AllChannel.CurWeight;   
//            if(_RC2 != 1)	//Input 1
//                IO_Status = IO_Status | 0x01;
//            else
//                IO_Status = (IO_Status | 0x00);
//
//            if(_RC1 != 1)	//Input 2
//                IO_Status = IO_Status | 0x02;
//            else
//                IO_Status = (IO_Status | 0x00);
//            Byte5 = (unsigned char) IO_Status ;
//            Byte6 = 0;              
        	SendCANdata(Byte1, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));
        }
        else if(Req_Data == Tx_CumulativeWt)
        {
            Byte1 = (unsigned char)(0x01);//sequence
            Byte2 = (unsigned char)DecimalPoint;
            Byte3 = ' ';//use it as polarity
            Byte4 = (unsigned char)((AllChannel.CumulativeWeight & 0xFF000000) >> 24);
            Byte5 = (unsigned char)((AllChannel.CumulativeWeight & 0x00FF0000) >> 16);
            Byte6 = (unsigned char)((AllChannel.CumulativeWeight & 0x0000FF00) >> 8);
            Byte7 = (unsigned char)(AllChannel.CumulativeWeight & 0x000000FF);
            Byte8 = (unsigned char)Unit;//unit 1-KG,2-TON               
        	SendCANdata(Byte1, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));
        }
    }
    else if(Req_Data == Tx_Individual_Ch_Wt)
    {    
        Byte2 = (unsigned char)DecimalPoint;
        CLoad = (unsigned long)Loadcell[CH1].CurWeight;         
        Byte3 = (unsigned char)(CLoad >> 8) ;
        Byte4 = (unsigned char)((CLoad << 8) >> 8);    
        CLoad = (unsigned long)Loadcell[CH2].CurWeight;    
        Byte5 = (unsigned char)(CLoad >> 8) ;
        Byte6 = (unsigned char)((CLoad << 8) >> 8);    
        CLoad = (unsigned long)Loadcell[CH3].CurWeight;    
        Byte7 = (unsigned char)(CLoad >> 8) ;
        Byte8 = (unsigned char)((CLoad << 8) >> 8);    

    	SendCANdata(0x01, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));

        Byte2 = (unsigned char)DecimalPoint;
        CLoad = (unsigned long)Loadcell[CH4].CurWeight;         
        Byte3 = (unsigned char)(CLoad >> 8) ;
        Byte4 = (unsigned char)((CLoad << 8) >> 8);    
        CLoad = (unsigned long)Loadcell[CH5].CurWeight;    
        Byte5 = (unsigned char)(CLoad >> 8) ;
        Byte6 = (unsigned char)((CLoad << 8) >> 8);    
        CLoad = (unsigned long)Loadcell[CH6].CurWeight;    
        Byte7 = (unsigned char)(CLoad >> 8) ;
        Byte8 = (unsigned char)((CLoad << 8) >> 8);  

    	SendCANdata(0x02, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));
    }
}

/****************************************************************
Function name 	: char Tx_LatestRec_Details(char Mode);
Returns 		: Defualt 1
Arguments		: Mode (Optional)
Date created 	: 04_10_2017(Added from Payload-II-MCU-6.13)
Description 	: Used to send latest trip details through CAN.
****************************************************************/
void Tx_LatestRec_Details(char Mode)
{
	unsigned long TempRecNo = 0;	
	FrameSequenceNumber = 1;

	if(Bits.RecOverWrite == 1)
		TempRecNo = (AllChannel.TripCount % MAXRECORDS);
	else
		TempRecNo = AllChannel.TripCount;
    
    Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TempRecNo-1) * (sizeof(TripRecord)))),sizeof(TripRecord));

    Byte2 = (unsigned char)(TripRecord.Date & 0xff);           //Record Date   
    Byte3 = (unsigned char)(TripRecord.Month & 0xff);			//Record Month
    Byte4 = (unsigned char)((TripRecord.Year >> 8) & 0xff);	//Record Year
    Byte5 = (unsigned char)(TripRecord.Year & 0xff);			//Record Year
    Byte6 = (unsigned char)(TripRecord.Hour & 0xff);           //Record Hour
    Byte7 = (unsigned char)(TripRecord.Minute & 0xff);			//Record Minutes
    Byte8 = (unsigned char)(TripRecord.Polarity & 0xff);       //Record Polarity
    SendCANdata(0x01, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));

    Byte2 = (unsigned char)((TripRecord.Weight & 0xFF000000) >> 24);
    Byte3 = (unsigned char)((TripRecord.Weight & 0x00FF0000) >> 16);
    Byte4 = (unsigned char)((TripRecord.Weight & 0x0000FF00) >> 8);
    Byte5 = (unsigned char)(TripRecord.Weight & 0x000000FF);
    Byte6 = 0xFF;												//Reserved
    Byte7 = 0xFF;												//Reserved
    Byte8 = 0xFF;												//Reserved
    SendCANdata(0x02, Byte2, ((Byte4 << 8) | Byte3),((Byte6 << 8) | Byte5), ((Byte8 << 8) | Byte7));
    FrameSequenceNumber++;
}

unsigned char LSBByte(unsigned int Buff)
{
	unsigned char val;
	val = ((Buff & 0xFF00) >> 8 );
	return(val);
}
unsigned char MSBByte(unsigned int Buff)
{
	unsigned char val;
	val = ((Buff & 0x00FF));
	return(val);
}

unsigned int From2Bytes(char LSBByte, char MSBByte)	// while storing in TX Buff MSB & LSB will store correctly but while sending through can it will reverse 
{
	unsigned int Value = 0;
	Value = (unsigned int)(MSBByte << 8) | LSBByte;
	return(Value);
}

char RxMoveData(unsigned char ExSequence)
{
	CurrFrame=GetLSB(InData0[0]);
	if (CurrFrame==1) {
		ExpFrame=1;BytesCount=7;
	}
	if ((ExpFrame != CurrFrame) && (RxCANPGN1 != RxCANPGN)) {
        RxDateReceived = 0;
		return 0;
	}

    PackRcvd2[BytesCount++]=GetMSB(InData0[0]);
	PackRcvd2[BytesCount++]=GetLSB(InData0[1]);
	PackRcvd2[BytesCount++]=GetMSB(InData0[1]);
	PackRcvd2[BytesCount++]=GetLSB(InData0[2]);
	PackRcvd2[BytesCount++]=GetMSB(InData0[2]);
	PackRcvd2[BytesCount++]=GetLSB(InData0[3]);
	PackRcvd2[BytesCount++]=GetMSB(InData0[3]);
    
	ExpFrame++;
	if (CurrFrame == ExSequence) {
    RxDateReceived = 1;
	}

	return(1);
}

void RequestInfo(unsigned char TxCANPGN)
{
//	if (Flags.InitCAN) { return;}
	if (C1ER_flag == 1) { return;}
	Tx_CANId.Priority = 6 ;		//6 constant
	Tx_CANId.Reserved = 0 ;		//0 constant
	Tx_CANId.DataPage = 0;		//0 constant
	Tx_CANId.PF = 0xf1;			//241 PDU format
    Tx_CANId.PS = TxCANPGN;			// PDU Specific
	Tx_CANId.SAddress = 255 ;	//255 Broad cast
    if(TxCANPGN == 23)
    {
        if((RecordsRead + NoOfRecords) <= RecordsAvailable)
            TripRecordad = NoOfRecords;
        else
            TripRecordad = RecordsAvailable - RecordsRead;
        
        RecordOffset = TripRecordad + m_ReadCounter;
        if(RecordOffset > MAXRECORDS)
            RecordOffset = RecordOffset - MAXRECORDS;
        
        ByteArr[0] = 1;
        ByteArr[1] = 1;
        ByteArr[2] = GetMSB(RecordsRead + 1000);
        ByteArr[3] = GetLSB(RecordsRead + 1000);
        ByteArr[4] = GetMSB(TripRecordad);
        ByteArr[5] = GetLSB(TripRecordad);
        ByteArr[6] = 0;
        ByteArr[7] = 0;
    }
    if(CTempVal)
        Tx_CANId.SAddress = BytesCount;
    SendCANdata(ByteArr[0], ByteArr[1], ((ByteArr[3] << 8) | ByteArr[2]),((ByteArr[5]<< 8) | ByteArr[4]), ((ByteArr[7] << 8) | ByteArr[6]));
    return;
}



