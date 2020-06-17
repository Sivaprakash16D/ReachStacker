/*
	For CAN, 
		Power ON condition Braodcast message number 255
		GPRS module content Reading Braodcast message number 254
*/
#include "p30fxxxx.h"
#include "libpic30.h"	//EEROM
#include "dsp.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "float.h"
#include "Defaults.h"
#include "Includes.h"
#include "Keypad.h"
//#include "Application.c"
//#include "GPRS_App.c"
//#include "Menu.c"
//#include "WeightCalc.c"

//#define FCY			7500000
_FOSC(FRC_PLL4);				// FOSC = ~30mhz 29.9222 = ( 7.37MHZ * 4 Pll) +1.5%   
_FWDT(WDT_OFF);                 /* Turn off the Watch-Dog Timer.  */
_FBORPOR(MCLR_EN & PWRT_OFF);   /* Enable MCLR reset pin and turn off the power-up timers. */
_FGS(CODE_PROT_OFF);

#define FOSCOSEL 0x000001
#define FPOR 0x000007
#define LCD_RS _RD1
//#define RPM_1200	40000

//#define	RTSCHECK_10_VINC_RTS    		      _RB4//(From Payload-Tripper counter RTS pin changed to RB4 from RB9 - 13-04-2018) //same as prev // (input)		To check the writing or not		
#define	RTSCHECK_10_VINC_RTS    		      _RB9//(From Payload-Tripper counter RTS pin changed to RB9 from RB4 - 08-05-2019) //same as prev // (input)		To check the writing or not		
#define	USBAVAIL_11_LD1_VINC_IO8              _RD10 //(Prev-RB14)  // (input)		To check the usb available or not (input)
#define	RESET_9_VINC_RESET                    _RD0  //(Prev-RB15)  // (Output)    To set the vinculum controller after USB detected
#define TRANSFERCOMPLETE_19_VINC_IO11         _RD11 //             // (input)     To check the data transfer complete or not

#define ThirdOrder	(9 * pow(10,-12))
#define SecondOrder (pow(10,-6))

#define p2ThirdOrder	(2* pow(10,-7))
#define p2SecondOrder	(3* pow(10,-6))

unsigned long TimeoutCounter;
unsigned long TimeOut_Cnt2 = 0,TimeOut_Cnt3 = 0;
extern unsigned int Temp_rec_counter;
void Init_Oscillator(void)
{
	int Counter;
 	OSCCON = 0x0700;			// FOSC = ~30mhz 29.9222 = ( 7.37MHZ * 4 Pll) +1.5%
 	OSCTUN = 0x0002;		    // Tune 1.5% 
	for(Counter=0;Counter<=1000;Counter++) {
	}

	OSCCON = 0x7846;			// Lock Osc so that accidental overwrite is avoided
	OSCCON = 0x9A57;			// Lock bits
	for(Counter=0;Counter<=1000;Counter++) {
	}
//	while(!OSCCONbits.LOCK);
}

int main(void)
{
//  Initialize the device
    char i=0,j=0;
    long DummyVariable = 0;
	PowerOn();
    FRAMInit();
    InitLCDDisplay();
 
	Read_E2((char*)&MemoryCheck,EEPROM_Check,1);Delay_ms(20);
    if(MemoryCheck != 95)	//95
	{
        Read_E2((char*)&Sec_MemoryCheck,EE_Sec_MemoryCheck,1);Delay_ms(20);
        if(Sec_MemoryCheck!=95)
        {
            memcpy(Header1_Name," Reach Stacker  ",16);
            Write_E2(EES_POD_first_Line,(char*)&Header1_Name,16);	    
        }
        else
        {
            Read_E2((char *)&Header1_Name,EES_POD_first_Line,16);
        }
    }
    else
    {
        Read_E2((char *)&Header1_Name,EES_POD_first_Line,16);
    }
//    TwoLineDisplay((char *)Header1_Name,(char *)"     Ver2.1     ",2);    LcdDispBuff
//    memcpy(Header1_Name,"   FIXED BODY   ",16);
//    Write_E2(EES_POD_first_Line,(char*)&Header1_Name,16);
    Read_E2(Header1_Name,EES_POD_first_Line,16);
    LcdDisplay(TOP,Header1_Name);
    LcdDisplay(BOTTOM,"     Ver1.0.18   ");
//    Read_E2((char *)&System_setting.CAN_BuadRate,ECAN_BuadRate,sizeof(System_setting.CAN_BuadRate));
//    if(System_setting.CAN_BuadRate > 3)
//        System_setting.CAN_BuadRate = 1;
    
    Read_EEPROM_Data();
    //Can_Init();
    UART_Config();    Delay1_1s(1);
//    for(i=0;i<ADC_Average_Count;i++)
//    {
//        for(j=0;j<NO_OF_CH;j++)
//        {
//            DummyVariable = GetAdc(j);
//        }
//    }
    PowerOnRTC();
        Read_E2((char *)&RT_Temp_Date,ERT_Temp_Date,sizeof(RT_Temp_Date));
    Read_E2((char *)&RT_Count,ERT_Count,sizeof(RT_Count));
    if(RT_Temp_Date !=RTC.Date )
    {
        RT_Temp_Date = RTC.Date;
        Write_E2(ERT_Temp_Date,(char *)&RT_Temp_Date,sizeof(RT_Temp_Date));Delay_ms(10);
        RT_Count = 0;
        Write_E2(ERT_Count,(char *)&RT_Count,sizeof(RT_Count));Delay_ms(10);
    }
   
            
    RT_Count1 = RT_Count;         
    PowerOn_SetBit();
   // Power_On_KeypressFunction();
    assigning_values();
//    if(AllChannel.TripCount != 0)
//    {
//        Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((AllChannel.TripCount-1) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
//        AllChannel.LatchedWt = TripRecord.Weight;
//    }
//    else
//        AllChannel.LatchedWt = 0;
//    Weight_Calc();
    if(Application_Setting.ComMode == COM_GPRS_MODULE)
    {
        Periodic_Report_Assign();
        ProtocolDump(3,1);
    }
//    if((Application_Setting.PLMS_Status > LOAD) && (Application_Setting.OperationLogic == ParkingBreak))   
//    {
//        if((Application_Setting.PLMS_Status == HAULING) && (AllChannel.CurWeight >=  Application_Setting.LatchedWt))
//        {
//            AllChannel.LatchedWt = TripRecord.Weight;
//            if(IPARKING != Application_Setting.Input1Logic)
//            {
//                Status_Write(HAULING);//Application_Setting.PLMS_Status = HAULING;
//                Bits.Disp_Latch_Wt = 1;
//            }
//            if(IPARKING == Application_Setting.Input1Logic)
//            {
//                Status_Write(DUMP);//Application_Setting.PLMS_Status = DUMP;
//                Bits.Disp_Latch_Wt = 1;
//            }
//    //        BotDisp = 8;
//        }
//        if(AllChannel.CurWeight < Application_Setting.MinWt)
//        {
//            if((Application_Setting.PLMS_Status == DUMP) || (Application_Setting.PLMS_Status == HAULING))
//            {
//                Status_Write(RETURN);//Application_Setting.PLMS_Status = RETURN;
//                changeLoadFlag = 1;
////                BodyUpFlag = 1;
//                Bits.Disp_Latch_Wt = 0;            
//            }
//            if((IPARKING == Application_Setting.Input1Logic) && (Application_Setting.PLMS_Status == RETURN))
//            {
//                Status_Write(LOAD);//Application_Setting.PLMS_Status = LOAD;
//                Bits.Disp_Latch_Wt = 0;
//            }            
//            if(Application_Setting.PLMS_Status == RETURN)
//            {
//                changeLoadFlag = 1;
//                Bits.Disp_Latch_Wt = 0;
//            }
//        }
//    }
////    Rec_Count_read();
    RS232_Dump_Time_Flag = CAN_BroadcastFlag = 0;
    
//    while(1)
//    {
//        MainScreenDisplay();
//         if((IPARKING == Application_Setting.Input1Logic))//&&(Input1sts_Bit == 0))
//            {        
//                  RT_Count +=(long)(TimeOut_Cnt3/1000);
//                 Write_E2(ERT_Count,(char *)&RT_Count,sizeof(RT_Count));Delay_ms(100);     
//            }
//    }
    while(1)
    {
      //  Weight_Calc();
        Keyfunction();
        if(RT_Temp_Date !=RTC.Date )    // Sivaprakash
        {
            RT_Temp_Date = RTC.Date;
            Write_E2(ERT_Temp_Date,(char *)&RT_Temp_Date,sizeof(RT_Temp_Date));Delay_ms(10);
            RT_Count = 0;
            Write_E2(ERT_Count,(char *)&RT_Count,sizeof(RT_Count));Delay_ms(10);
        }
//        if(RxCANPGN != 0)
//            CANDataFlow();
//        if(CAN_BroadcastFlag)
//        {
//            CAN_BroadcastFlag = 0;
//            Tx_CANId.Priority = 6 ;		//6 constant
//            Tx_CANId.Reserved = 0 ;		//0 constant
//            Tx_CANId.DataPage = 0;		//0 constant
//            Tx_CANId.PF = 0xf2;			//241 PDU format
//            Tx_CANId.PS = 2;			// PDU Specific
//            Tx_CANId.SAddress = 255 ;	//255 Broad cast
//            Tx_CANReport_Generate(2);				// ID : 18F202FF    Tx_CurrentWt
////            LcdDisplay(BOTTOM,"     Ver2.10    ");
////            Delay1_1s(10);
//        }
//        if (Flags.InitCAN) {
//            Can_Init();	C1ER_flag = 0;
//            Flags.InitCAN=0;
////            LcdDisplay(BOTTOM,"     Ver2.9     ");
////            Delay1_1s(10);
//        }
        MainScreenDisplay();
//        RangeOutput();
//        if(Application_Setting.OperationLogic == ParkingBreak)
//            PayloadProcess_ParkingBreak();
//        else
//            PayloadProcess_BodyUp();
        //USB_Check();
//        namechange_via_RS232();
        if(Application_Setting.ComMode == COM_GPRS_MODULE)
            Record_Management();
//        else if((Application_Setting.ComMode == COM_DUMP) && (RS232_Dump_Time_Flag))
//        {
//            RS232_Dump_Time_Flag = 0;
//            PC_Dump();
//        }
        
         
    }
}

void Keyfunction()
{
    char i=0;
	char Dump_Prot_Buff[25] = {0};     
    KeySense();
    if(MenuKey)
    {
        Display_Deg = 0;        
        if(GetPassword())
            DoMenu();          
        assigning_values();
    }
    else if(EnterKey)
    {
 //       FIFO_Rec_Retrival();
        Display_Deg = 0;        
        if(GetPassword())        
            TareFunction();
    }
    else if(UpKey)
    {
        BotDisp++;
        if(BotDisp >= 9)
            BotDisp = 0;
		memset(LcdDispBuff,' ',32);     
//        RecordStorage();

    }
    else if(LeftKey)
    {
        Display_Deg = 0;        
        PrintResetOption();
        //FIFO_RecordReset();
    }    
}

void PrintResetOption()
{
	while(1)
	{
        TwoLineDisplay("      PRESS     "," UP-CLR LEFT-PRN",0);
		KeySense();
		if(MenuKey)
		{
			break;
		}
		else if(EnterKey)
		{
            Calibration_Dump();
			break;
		}        
		else if(UpKey)
		{
			TwoLineDisplay(" ENTER PASSWORD "," (RESET TRIPS)  ",2);
            if(GetPassword())
			{
				Reset_Cum_Trip();
			}
			break;            
		}
		else if(LeftKey)
		{
            if(AllChannel.TripCount != 0)
                PrintOption();
            else
                TwoLineDisplay("                "," * NO RECORD *  ",2);
			break;
		}
		if(TimeOutFlag.TimeOut)
			return;
	}
	return;
}

void Reply_NameChange(char Header_r_Bin,char Header_or_Bin_No)
{
	char temparray_1[20]={0};
	char Counter = 0;
	if(Header_r_Bin == 0)//0-BIN
	{
        memcpy(temparray_1,"%MODEL*",7);
        sprintf(temparray_1+7,"%d",Header_or_Bin_No);
	  	Counter+=8;
	}
	else	//HEADER
	{
		memcpy(temparray_1,"[HEADER",7);		
		Counter+=8;
	}
	memcpy(temparray_1+Counter,"*changed]",9);
	Com2StringSend(temparray_1,(Counter+9));	
}

void namechange_via_RS232(void)
{
	if(ReceivedFlag == 1)
	{
        if(!strncmp((char *)ReceiveBuff,"COMCHK",6))
        {
            Com2StringSend((char *)"[ALIVE]",7);                
			Rx_ClearVar();          
        }
		else if((!strncmp((char *)ReceiveBuff,"HEADER*",7)))
		{
		//	if(ReceiveCounter1 >= 23)
			{
				memcpy(Header1_Name,ReceiveBuff+7,16);
				Write_E2(EES_POD_first_Line,(char*)&Header1_Name,16);	
				Reply_NameChange(1,1);
//				LcdDisplay(TOP,Header1_Name);
//				Delay_1sec(10);
			}
//			else
//				Com2StringSend("Header length Error",19);	
			Rx_ClearVar();
		}
		//ReceivedFlag =0;
	}
}

void Record_Management(void)
{
    if((Flag.Periodic_Send_Flag == 1)&& (TimerCntr2 == 0))
    {
        ProtocolDump(3,0);
        
            Flag.AckFlag = 1;
            TimerCntr2 = 1;
        
    }
    else if((Flag.Latched_Send_Flag == 1) && (TimerCntr2 == 0))
    {
        ProtocolDump(2,0);
        
            Flag.AckFlag = 2;
            TimerCntr2 = 1;
        
    }
    if((Periodic_Time == 0) || ((Periodic_Time != 0) && (Send_Protocol == 1) ))
    {
        Periodic_Report_Assign();
        Flag.Periodic_Send_Flag = 1;
        Send_Protocol = 0;
    }
    if((Input1sts_Bit == 1)&& (TimerCntr2 == 0)&&(Input1sts_Bit1 == 0))
    {
        if(!PowerOn_Bit1 == 1)     // Power on time ignore 
        SMS_Alert(1);
        Periodic_Report_Assign();
        Flag.Periodic_Send_Flag = 1;
        Flag.AckFlag = 4;
        TimerCntr2 = 1;
        Input1sts_Bit1 = 1;
        Input1sts_Bit2 = 0;
        PowerOn_Bit1 =PowerOn_Bit2= 0;
        
    }
     if((Input1sts_Bit == 2)&& (TimerCntr2 == 0)&&(Input1sts_Bit2 == 0))
    {
         
        
        if(!PowerOn_Bit2 == 1)
        SMS_Alert(2);
        Periodic_Report_Assign();
        Flag.Periodic_Send_Flag = 1;
        Flag.AckFlag = 4;
        TimerCntr2 = 1;
        Input1sts_Bit2 = 1;
        Input1sts_Bit = 0;
        Input1sts_Bit1 = 0;
        PowerOn_Bit1 = PowerOn_Bit2 = 0;
    }
     if((Input2sts_Bit == 1)&& (TimerCntr2 == 0)&&(Input2sts_Bit1 == 0))
    {
        if(!PowerOn_Bit3 == 1)
        SMS_Alert(3);  
        Periodic_Report_Assign();
        Flag.Periodic_Send_Flag = 1;
        Flag.AckFlag = 4;
        TimerCntr2 = 1;
        Input2sts_Bit1 = 1;
        Input2sts_Bit2 = 0;
        PowerOn_Bit3 =PowerOn_Bit4 = 0;
    }
     if((Input2sts_Bit == 2)&& (TimerCntr2 == 0)&&(Input2sts_Bit2 == 0))
    {
        if(!PowerOn_Bit4 == 1)
        SMS_Alert(4);
        Periodic_Report_Assign();
        Flag.Periodic_Send_Flag = 1;
        Flag.AckFlag = 4;
        TimerCntr2 = 1;
        Input2sts_Bit = 0;
        Input2sts_Bit1 = 0;
        Input2sts_Bit2 = 1;
        PowerOn_Bit3 = PowerOn_Bit4 = 0;
    }
    Interrupt_Check(); 
    if((IPARKING == Application_Setting.Input1Logic))//&&(Input1sts_Bit == 0))
    {        
        Input1sts_Bit = 1;        
    }
    else if((IPARKING != Application_Setting.Input1Logic))//&&(Input1sts_Bit == 1))
    {
        
        Input1sts_Bit = 2;
    }
    if((IBODYUP ==	Application_Setting.Input2Logic))//&&(Input2sts_Bit == 0))
    {
        
        Input2sts_Bit = 1;
    }
    else if((IBODYUP !=	Application_Setting.Input2Logic))//&&(Input2sts_Bit == 1))
    {
        
        Input2sts_Bit = 2;
    }
   
}

void MainScreenDisplay()
{
    unsigned int   Minute = 0,Hour = 0;
//	if(Bits.ERRFlag)			
//	{
//		ORDisplay();
//	}
//	else if(Bits.OCERRFlag1)
//	{
//		memcpy(LcdDispBuff,"LOAD( ):   OC   ",16);
//	}
//	else
//	{
////        if(Application_Setting.PLMS_Status == RETURN)		
////        {
////           sprintf(LcdDispBuff,"LOAD( ):%05ld   ",(unsigned long)0);
////        }
//        if(Bits.Disp_Latch_Wt == 1)			
//        {
//           sprintf(LcdDispBuff,"LOAD( ):%05ld   ",AllChannel.LatchedWt);
//        }
//        else
//        {
//           sprintf(LcdDispBuff,"LOAD( ):%05ld   ",AllChannel.CurWeight);   
//        }
//    //  sprintf(LcdDispBuff,"LOAD( ):%05ld  ",AllChannel.CurWeight);   //            sprintf(LcdDispBuff,"CtLoad: %04ld   ",AllChannel.CurWeight);   
//
//        if(Application_Setting.OperationLogic == ParkingBreak)
//        {
//            if(Application_Setting.PLMS_Status == LOAD)
//                LcdDispBuff[5] = 'L';
//            if(Application_Setting.PLMS_Status == HAULING)
//                LcdDispBuff[5] = 'H';
//            if(Application_Setting.PLMS_Status == RETURN)	
//                LcdDispBuff[5] = 'R';
//            if(Application_Setting.PLMS_Status == EMPTY)	
//                LcdDispBuff[5] = 'E';
//            if(Application_Setting.PLMS_Status == DUMP)
//                LcdDispBuff[5] = 'D';	
//            if(Application_Setting.PLMS_Status == INIT)
//                LcdDispBuff[5] = 'I';
//        }
//        else
//        {
//            if(MinWt_Check_Counter == 1)
//            {
//                LcdDispBuff[5] = 'D';
//            }
//            else
//            {
//                LcdDispBuff[5] = 'L';
//            }            
//        }
//        PutDp((unsigned char*)LcdDispBuff+8,DecimalPoint,5,1);       
//
//        if((Unit-1) == 0)
//            memcpy(LcdDispBuff+14,"Kg",2);
//        else
//            memcpy(LcdDispBuff+14," T",2);                    
//	}

	ReadRTC_DateTime();
//	if(!Bits.ERRFlag)
//	{
//		if(BotDisp == 0)
//		{
//			if(AllChannel.TripCount > 9999)
//			{
//				memcpy(LcdDispBuff+16,"TRIPS   : MAX   ",16);
//			}
//			else
//			{
//				sprintf(LcdDispBuff+16,"TRIPS  : %4u   ",AllChannel.TripCount);
//			}
//		}
//		else if(BotDisp == 1)
//		{
//            if(Application_Setting.OperationLogic == ParkingBreak)
//            {
//                sprintf(LcdDispBuff+16,"C:%4d  W:%05lu",Average_Count,(unsigned long)Average_Wt);        
//                PutDp((unsigned char*)LcdDispBuff+26,DecimalPoint,5,1);
//            }
//            else
//                sprintf(LcdDispBuff+16,"Count  : %4d   ",Average_Count);        
//		}
//        else if(BotDisp == 2)
//        {
//            WeightDisplay(2);   // Angle Display
//        }
//        else if(BotDisp == 3)
//        {
//            WeightDisplay(3);   // 1 & 2 channel Display        
//        }
//        else if(BotDisp == 4)
//        {
//            WeightDisplay(4);   // 3 & 4 Channel Display
//        }
//        else if(BotDisp == 5)
//        {
//            if((Application_Setting.LCED[CH3] == 1)||(Application_Setting.LCED[CH4] == 1))
//            {
//                sprintf(LcdDispBuff+16,"R:%04ld ",(long)(Loadcell[CH3].CurWeight + Loadcell[CH4].CurWeight));
//                PutDp((unsigned char*)LcdDispBuff+18,1,4,1);
//            }
//            if((Application_Setting.LCED[CH5] == 1)||(Application_Setting.LCED[CH6] == 1))
//            {
//                sprintf(LcdDispBuff+24,"F:%04ld ",(long)(Loadcell[CH5].CurWeight + Loadcell[CH6].CurWeight));
//                PutDp((unsigned char*)LcdDispBuff+26,1,4,1);            
//            }
//        }
//		else if(BotDisp == 6)
//		{
//            sprintf(LcdDispBuff+16,"%02u/%02u/%02d  %02u:%02u",RTC.Date,RTC.Month,(RTC.Year%2000),RTC.Hour,RTC.Minute);
//		}
//        else if(BotDisp == 7)
//        {
//            sprintf(LcdDispBuff+16,"L.Wt   : %04ld  ",AllChannel.LatchedWt);   
//            PutDp((unsigned char*)LcdDispBuff+25,DecimalPoint,4,1);     
//            if(Unit == 1)
//              memcpy(LcdDispBuff+30,"Kg",2);
//            else
//              memcpy(LcdDispBuff+30," T",2);           
//        }
//        else if(BotDisp == 8)
//        {
//            if(Bits.Disp_Latch_Wt == 1)
//            {
//                sprintf(LcdDispBuff+16,"Ct Load:%05ld    ",AllChannel.CurWeight);   
//                PutDp((unsigned char*)LcdDispBuff+24,DecimalPoint,5,1);     
//                if(Unit == 1)
//                   memcpy(LcdDispBuff+30,"Kg",2);
//                else
//                   memcpy(LcdDispBuff+30," T",2);           
//            }
//            else
//                BotDisp = 0;
//        }        
//	}
    memcpy(LcdDispBuff,"I/P1:    I/P2:  ",16);  
    if(IPARKING == Application_Setting.Input1Logic)
         memcpy(LcdDispBuff+5,"1",1);
	else if(IPARKING != Application_Setting.Input1Logic)
        memcpy(LcdDispBuff+5,"0",1);
	if(IBODYUP ==	Application_Setting.Input2Logic)
        memcpy(LcdDispBuff+14,"1",1);
	else if(IBODYUP !=	Application_Setting.Input2Logic)
        memcpy(LcdDispBuff+14,"0",1);
   
    Minute = (((TimeOut_Cnt3/1000)+RT_Count1)/60);  // Run Time Calculation
                if(Minute>60)
                     Minute = (int)((((TimeOut_Cnt3/1000)+RT_Count1)/60)%60);
        Hour = (int)((((TimeOut_Cnt3/1000)+RT_Count1)/60)/60);
    if(IPARKING == Application_Setting.Input1Logic)
    sprintf((char *)(LcdDispBuff+16),"RT:%02d:%02d:%02d      ",Hour,Minute,(int)((((TimeOut_Cnt3/1000)+RT_Count1)%60)));
    else
    sprintf(LcdDispBuff+16,"%02u/%02u/%02d  %02u:%02u",RTC.Date,RTC.Month,(RTC.Year%2000),RTC.Hour,RTC.Minute);
    TwoLineDisplay(LcdDispBuff,LcdDispBuff+16,0);
 //   TwoLineDisplay(ReceiveBuff,ReceiveBuff+16,0);
}

void Delay(unsigned long int counter)
{
	while(counter > 0) 
	{
		counter--; 
	}	
}

void ReadADCCounts(unsigned char Channel)
{
   ADCHS = Channel;
   Delay_ms(1);
   	
   ADCON1bits.ADON = 1;
			
	
  ADCSampleVal = 0;

  for(Samples = 0; Samples < 20; Samples++)
  {
	ADCValue = 0; // clear value
	ADC16Ptr = &ADCBUF0;

	Flags.ADDATAREADY = 0;
	while(!Flags.ADDATAREADY);

	for (count = 0; count < 16; count++) // average the 15 ADC value
	{
	  ADCValue = ADCValue + *ADC16Ptr++;
	}

	ADCValue = ADCValue >> 4;		//16 pgm samples 
		
	ADCSampleVal+= ADCValue;
 	 
 }

 ADCSampleVal  = ADCSampleVal / 20; //In trip counter we are using 12 bit ADC
	
// ADCSampleVal  = ADCSampleVal >> 3;//Enable this line for 9-bit ADC.9-bit adc used in Payload-II-MCU projects
}

void PowerOn()
{
	Initialise_Ports();
	Init_Oscillator();
	Init_Interrupt();
	ADC_Init();
	InitLCDDisplay();
	SRbits.IPL	= 0;
    RESET_9_VINC_RESET = 1; Nop();Nop();Nop();
    CanFTimer.StartTimer=0;CanFTimer.TimerOver=0;
	CanCTimer.StartTimer=0;CanCTimer.TimerOver=0;
    Flags.InitCAN=0;
	Flags.FromCAN=0;
	RxCANPGN=0;
    
}

void Initialise_Ports()
{
// port b-2,3,4,5 as output, rest as inputs 
//	TRISB = 0Xfffb;

//	TRISB = 0Xcfc3;		// Anbu - Keypad port change 29/05/2013
	TRISB = 0XffD3;		// Iyyappan - 03/06/2016 //RB9 and RB14 as input from vinculum IC(RTS,IOBUS11),RESET o/p for vinculum is RB15
//	TRISB = 0x3fc3;		// For Matrix Keypad RB12,RB13 used as input RB14,RB15 used as output 
	PORTB=0x00;

//port G-15,14,13,12,11,10 input, 9,8,7,6 as output, 5,4 as input, 3,2 as output, 1 & 0 as input
//	TRISG = 0xfc33; 	// Anbu - Keypad port change 29/05/2013		 
	TRISG = 0xac33; 
	TRISC = 0x0006;		//PortC.1,2 as i/p
//	TRISF = 0x0020;
//	TRISD = 0xFFFF;

	TRISD = 0xFc00;		// EWP_A RD8,EWP_B RD9 as o/p's  // 30/4/2016 - RD10,RD11 as input for (IO8(LD1)),(IO11(LD2)),RD0 as output for RESET//already did i didn't change anything.
	
//	c0x03c3;//Nop();
//	PORTB=0x3c ;//Nop();
	
	PORTG=0x00;

	_TRISD9 = 0;	//Write protect for FRAM
	_TRISD8 = 0;	//Chip select for FRAM
	_TRISG2 = 0;	//CLK for FRAM
	_TRISG3 = 1;	//SDO for FRAM
	_TRISF6 = 0;	//SDI for FRAM
	_TRISD0 = 0;	

	_RD8 = 1;Nop();	//Write protect enable
	_RD9 = 0;Nop();	// Active low chip select
}


void Init_Interrupt(void)
{

/*
//	SR		= 0X0010;
//	CORCON	= 0X0000;
//	INTCON1	= 0XE700;
	INTCON2	= 0X0000;
//	IFS0	= 0X0201;
	IFS1bits.C1IF	= 1;
	IFS1bits.C1IF	= 1;

//	IFS2	= 0X0000;
//	IEC0	= 0X1201;
	IEC1bits.C1IE   = 1;
//	IEC1	= 0X0800;

//	IEC2	= 0X0000;

//	IPC0	= 0X0004;
//	IPC1	= 0X0000;
//	IPC2	= 0X0000;
//	IPC3	= 0X0000;
//	IPC4	= 0X0000;
//	IPC5	= 0X0000;
//	IPC6	= 0X0004;
//	IPC7	= 0X0000;
//	IPC8	= 0X0000;
//	IPC9	= 0X0000;
//	IPC10	= 0X0000;
//	INTTREG	= 0X0200;
	
*/	
	

	INTCON2 = 0x001E;       /*Setup INT1, INT2, INT3 & INT4 pins to interupt */
                                /*on falling edge and set up INT0 pin to interupt */
                                /*on rising edge */

//      IFS0bits.INT0IF = 0;    /*Reset INT0 interrupt flag */
//      IEC0bits.INT0IE = 1;    /*Enable INT0 Interrupt Service Routine */

//      IFS1bits.INT1IF = 0;    /*Reset INT1 interrupt flag */
//	  IEC1bits.INT1IE = 1;    /*Enable INT1 Interrupt Service Routine */

//      IFS1bits.INT2IF = 0;    /*Reset INT2 interrupt flag */
//      IEC1bits.INT2IE = 1;    /*Enable INT2 Interrupt Service Routine */

//      IFS2bits.INT3IF = 0;    /*Reset INT3 interrupt flag */
//      IEC2bits.INT3IE = 1;    /*Enable INT3 Interrupt Service Routine */

      IFS2bits.INT4IF = 0;    /*Reset INT4 interrupt flag */
      IEC2bits.INT4IE = 1;    /*Enable INT4 Interrupt Service Routine */

 
	/* ensure Timer 1 is in reset state */
	T1CON = 0;
	/* Reset Timer1 interrupt flag */
	IFS0bits.T1IF = 0;
	T1CONbits.TSIDL=0;		// Resume even if in idle mode
 	PR1	  =	Delay_1mS_Cnt;	//  Value = ( 1 / (FCY / pres)) * PR1) = delay 0x1D4C
 	/* select external timer clock */
	T1CONbits.TCS = 0;
	T1CONbits.TGATE=0;
	T1CONbits.TSYNC=0;
	IEC0bits.T1IE = 1;  	// Enable Timer1 interrupt 
	T1CONbits.TON = 1;		/* enable Timer 1 and start the count */ 

}

void Com1Init()
{
	/* COM PORT 1*/
	U1MODE = 0xA080;
	U1STAbits.UTXISEL = 0;
	U1STAbits.UTXBRK  = 0;
	U1STAbits.UTXEN	  = 1;
	U1STAbits.URXISEL = 0;
	U1STAbits.ADDEN	  = 1;
	U1STAbits.URXISEL = 1;	// enable receive interrupt when a character is available
	U1STAbits.UTXISEL = 1;	// enable transmit interrupt when a character is available
//	U1BRG  = Baud4800;
//	U1BRG  = Baud115200;	
//	U1BRG  = Baud9600;			
//	U1BRG  = Baud57600;
	U1BRG  = Baud38400;         //For USB communication baud changed to 38400
	IEC0bits.U1RXIE   	= 1;	//Receiver Interrupt Enable bit		
	IEC0bits.U1TXIE   	= 1;	//transmit Interrupt Enable bit	
	
	IFS0bits.U1TXIF = 0;		 // clear TX interrupt flag
	IFS0bits.U1RXIF = 0;		 // clear RX interrupt flag
	U1STAbits.OERR = 0; 		// Clear Overrun Error to receive data
	U1STAbits.FERR =0;
	U1STAbits.PERR =0;
	_TRISF2=1;_TRISF3=0;
	_RF2=0;_LATF3=0;
	
 	Flags.Stx1Received=0;Flags.Etx1Received=0;
	CharIn1=0;CharOut1=0;BytesToSend1=0;
}

void Com2Init(unsigned long BuadRate)
{
	/* COM PORT 2*/
	U2MODE = 0xA080;
	U2STAbits.UTXISEL = 0;
	U2STAbits.UTXBRK  = 0;
	U2STAbits.UTXEN	  = 1;
	U2STAbits.URXISEL = 0;
	U2STAbits.ADDEN	  = 1;
	U2STAbits.URXISEL = 1;	// enable interrupt when a character is available
	U2STAbits.UTXISEL = 1;	// enable transmit interrupt when a character is available
//	                            sprintf(LcdDispBuff,"1:%lu",BuadRate);
//                            LcdDisplay(TOP,LcdDispBuff);
//                            Delay1_1s(5);
    if(BuadRate == 1)    // Baud Rate = 1200; 
    	U2BRG  = 0x0185;
    else if(BuadRate == 2)    // Baud Rate = 2400; 
    	U2BRG  = 0x00C2;
    else if(BuadRate == 3)    // Baud Rate = 9600; 
    	U2BRG  = 0x002F;
    else if(BuadRate == 4)    // Baud Rate = 19200; 
    	U2BRG  = 0x0017;
    else if(BuadRate == 5)    // Baud Rate = 38400; 
    	U2BRG  = 0x000B;
    else if(BuadRate == 6)    // Baud Rate = 57600; 
    	U2BRG  = 0x0007;
    else if(BuadRate == 7)    // Baud Rate = 115200; 
    	U2BRG  = 0x0003;
    else                      // Baud Rate = 9600; 
    	U2BRG  = 0x002F;
//    U2BRG  = 0x0003;
//	U2BRG  = Baud115200;		//For Baudrate changed to 115200
	IEC1bits.U2RXIE   	= 1;	//Receiver Interrupt Enable bit		
	IEC1bits.U2TXIE   	= 1;	//transmit Interrupt Enable bit		

	IFS1bits.U2TXIF = 0;		// clear TX interrupt flag
	IFS1bits.U2RXIF = 0;		// clear RX interrupt flag
	U2STAbits.OERR = 0; 		// Clear Overrun Error to receive data
	U2STAbits.FERR =0;
	U2STAbits.PERR =0;
	_TRISF4=1;_TRISF5=0;
	_RF4=0;_LATF5=0;

 	Flags.Stx2Received=0;Flags.Etx2Received=0;
 	CharIn2=0;CharOut2=0;BytesToSend2=0;
}

void UART_Config()
{
	Com1Init();
	Com2Init(System_setting.BuadRate);	
}

void Delay_ms(unsigned char Delay)
{
	Delayval=Delay;
	while (Delayval !=0);
	return ;
}


void Com1Bytesend(char c)		/*  COM 1 byte send*/
{
	while(!U1STAbits.TRMT);
	U1TXREG = c;		
}

void Com1StringSend(char * Buff,char NoOfBytes)	/*  COM 2 string send*/
{
	unsigned char Increment;

	for(Increment=0; Increment < NoOfBytes; Increment++){	// This loop reads in the text string and 
//        sprintf(LcdDispBuff,"%d:%d:%x:%c",Increment,Buff[Increment],Buff[Increment],Buff[Increment]);
//        LcdDisplay(TOP,LcdDispBuff);
//        Delay1_1s(1);
		Com1Bytesend(Buff[Increment]);		
	}
}

void Com2Bytesend(char c)	/*  COM 2 byte send*/
{
	while(!U2STAbits.TRMT);
	U2TXREG = c;		
}

void Com2StringSend(char * Buff,unsigned int NoOfBytes)	/*  COM 2 string send*/
{
	unsigned int Increment;

	for(Increment=0; Increment < NoOfBytes; Increment++){	// This loop reads in the text string and 
		Com2Bytesend(Buff[Increment]);		
	}
}


#define COM_1   0
#define COM_2   1

void PrintChar(char UartNo,unsigned char Schar, unsigned int count)
{
	unsigned int i = 0;
	for(i = 0; i < count; i++)
	{
		if(UartNo== COM_1)
		{
            Com1Bytesend(Schar);
		}
		else if(UartNo== COM_2)
		{
			Com2Bytesend(Schar);
		}			
	}
}	

void PrintCRLF(char UartNo,unsigned int count)
{
	unsigned int i = 0;
	for(i = 0; i < count; i++)
	{
		if(UartNo== COM_1)
		{
			Com1Bytesend(0x0d);
			Com1Bytesend(0x0a);
		}
		else if(UartNo== COM_2)
		{
			Com2Bytesend(0x0d);
			Com2Bytesend(0x0a);
		}
	}
}
extern unsigned long TimeOut_Cnt1;

void _ISR __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void)
{	
	// Insert ISR Code Here
	// 1000 - 1 sec 
    // 60000 - 1 min
    
    
    if(IPARKING == Application_Setting.Input1Logic)
    {
        TimeOut_Cnt3++;
    }
	if (Delayval > 0 ) {
	 	Delayval--;
	}
	if (TimeoutCounter > 0 ) {
	 	TimeoutCounter++;
	}
    
    if((RS232_Dump_Time_Cntr++ >= (Application_Setting.RS232_Dump_Time*100)) && (RS232_Dump_Time_Flag != 5) && (Application_Setting.RS232_Dump_Time != 0) )
    {
        RS232_Dump_Time_Cntr = 0;
        RS232_Dump_Time_Flag = 1;
    }
    if((CAN_BroadcastCntr++ >= (Application_Setting.CAN_Broadcast_Time*100)) && (CAN_BroadcastFlag != 5) && (Application_Setting.CAN_Broadcast_Time != 0) )
    {
        CAN_BroadcastCntr = 0;
        CAN_BroadcastFlag = 1;
    }
    if(TimerCntr1 != 0)										// This is used for GPRS Menu timeout
        TimerCntr1++;
    if(TimerCntr2 != 0)										// Periodic or Collection acknoledgement timeout
        TimerCntr2++;
	if(TimerCntr2 > 10000)//( (Flag.AckFlag != 0) && 
	{
		TimerCntr2 = 0;
		Flag.AckFlag = 0;
	}    
	if(Bits.InputCheck == 1)
	{
		if(TimeOut_Cnt1++ >=  (unsigned long)(Latching_Time))// && (ILATCH == Setup.InputLogic))	
		{
			Bits.ILATCH_val = 1;
			TimeOut_Cnt1 = 0;	
            Bits.InputCheck = 0;
		}	
	}    
    
    if(TimeOut_Cnt2++ >=  (unsigned long)(Periodic_Time))// && (ILATCH == Setup.InputLogic))	
    {
        TimeOut_Cnt2 = 0;	
        Send_Protocol = 1;
    }        

    if (SecondsCounter >0) {
		if (--SecondsCounter == 0) {
			SecondsCounter=1000;
			Flags.ReadRTC=1;
			
			if (CanFTimer.StartTimer) {
				CanFTimer.TimerValue--;
				if (CanFTimer.TimerValue==0) {
					CanFTimer.StartTimer=0;
					CanFTimer.TimerOver=1;
					Flags.InitCAN=1;
				}	
			}		
			if (CanCTimer.StartTimer) {
				CanCTimer.TimerValue--;
				if (CanCTimer.TimerValue==0) {
					CanCTimer.StartTimer=0;
					CanCTimer.TimerOver=1;
				}	
			}		
		}
	}
    
	IFS0bits.T1IF = 0;
}

//
//void CheckPort1()
//{
//	if (U1STAbits.URXDA) {
//		if(U1STAbits.OERR == 1) {
//			U1STAbits.OERR = 0; // Clear Overrun Error to receive data
//		} 
//		if ((U1STAbits.FERR ==0) && (U1STAbits.PERR ==0) ) {
//			//ReceivedChar = U1RXREG; // Read Data if there is no parity or framing error
//			CharRTable1[CharIn1] = U1RXREG;
//			CharIn1++;
//			if (CharIn1 >= ArraySize) CharIn1=0;
//		}	
//	}	
//}	

void _ISR __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
	if(U1STAbits.OERR == 1) {
		U1STAbits.OERR = 0; // Clear Overrun Error to receive data
	} 

      ReceivedChar = U1RXREG;

//    if(Bits.UART_1_Ready == 0)
//    {
//        if((Bits.UART_1_Start == 1 )&& (ReceivedChar == '$'))
//        {
//            Bits.UART_1_Start = 0;
//            Bits.UART_1_Ready = 1;
//        }
//        if(Bits.UART_1_Start == 1)
//            ReceiveBuff[ReceiveCounter1++] = ReceivedChar;			
//        if(ReceivedChar == '%')
//        {
//            ReceiveCounter1 = 0;
//            Bits.UART_1_Start = 1;
//        }
//    }

	IFS0bits.U1RXIF = 0; // clear RX interrupt flag}
}

void _ISR __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
	Flags.SendData1 = True ;

	if (BytesToSend1!=0) {
		if (BytesSent1 < BytesToSend1) {
			U1TXREG = PackToSend1[BytesSent1];
			BytesSent1++;
		}
		else 
			{BytesToSend1=0;}
	}
	IFS0bits.U1TXIF = 0; // clear RX interrupt flag}
}
unsigned char Received_Char = 0;
void _ISR __attribute__((interrupt,no_auto_psv)) _U2RXInterrupt(void)
{
	if(U2STAbits.OERR == 1) {
		U2STAbits.OERR = 0; // Clear Overrun Error to receive data
	} 
	if ((U2STAbits.FERR ==0) && (U2STAbits.PERR ==0) ) 
    {
        Received_Char = U2RXREG;
        if(ReceivedFlag == 0)
        {
            if( (Received_Char == ']') && (ReceiveStartFlag == 1) )
            {
                ReceivedFlag = 1;
                ReceiveStartFlag = 0;
            }
            else if(ReceiveStartFlag == 1)
            {
                ReceiveBuff[ucount++] = Received_Char;
            }
            else if(Received_Char == '[')
            {
                ucount = 0;
                ReceivedFlag = 0;
                ReceiveStartFlag = 1;   
            }
        }
        if(Received_Char == 'U')
            Bits.Com2Check = 1;
		if (ucount >= ReceiveBuff_size)
            ucount=0;
	}
	IFS1bits.U2RXIF = 0; // clear RX interrupt flag}
}


void _ISR __attribute__((interrupt,no_auto_psv)) _U2TXInterrupt(void)
{
//	Flags.SendData2 = True ;

//	if (BytesToSend2!=0) {
//		if (BytesSent2 < BytesToSend2) {
//			U2TXREG = PackToSend2[BytesSent2];
//			BytesSent2++;
//		}
//		else 
//			{BytesToSend2=0;}
//	}
	IFS1bits.U2TXIF = 0; // clear RX interrupt flag}
}

void __attribute__((interrupt,no_auto_psv)) _ADCInterrupt(void)
{
		Flags.ADDATAREADY  = 1;
		ADCOUNTS = ADCBUF0;
        IFS0bits.ADIF = 0;
//		 if(ADCntr++ >= 999999)
//			ADCntr = 0;
}

void __attribute__((interrupt,no_auto_psv)) _INT0Interrupt(void)
{

	if(IFS0bits.INT0IF)
	{	
		IFS0bits.INT0IF = 0;
	}
		    //Clear the INT0 interrupt flag or else
            //the CPU will keep vectoring back to the ISR
}

void __attribute__((interrupt,no_auto_psv)) _INT1Interrupt(void)	// interrupt one 
{
	if(IFS1bits.INT1IF)	// interrupt one status flag
	{	
		InterruptFlag++;
	//	EEPROMINC_VAL++;
		IFS1bits.INT1IF = 0;		
	}
		
}

void __attribute__((interrupt,no_auto_psv)) _INT2Interrupt(void)		// interrupt two
{
	if(IFS1bits.INT2IF)		// interrupt two  status flag
	{	
		InterruptFlag++;
	//	EEPROMINC_VAL++;
		IFS1bits.INT2IF = 0;		
	}
		
}

void __attribute__((interrupt,no_auto_psv)) _INT3Interrupt(void)		// interrupt three 
{
	if(IFS2bits.INT3IF)		// interrupt three status flag
	{	
		InterruptFlag++;
	//	EEPROMINC_VAL++;
		IFS2bits.INT3IF = 0;		
	}
		
}

void __attribute__((interrupt,no_auto_psv)) _INT4Interrupt(void)		// interrupt four
{
	if(IFS2bits.INT4IF)		// interrupt four status flag
	{	
//		PulseCount++;
		IFS2bits.INT4IF = 0;		
	}
		
}

unsigned char GetLSB(unsigned int Integer)
{
	return (Integer & 0xff);
}

unsigned char GetMSB(unsigned int Integer)
{
	return ((Integer >> 8 ) & 0x0ff);
}

void CalcCRC(unsigned int BytesCount, unsigned char *SPtr)
{

	unsigned char CharAvail,a;
	unsigned char Counter,Counter2;
	unsigned int Check2Sum;
	Check2Sum=0xffff;
	for (Counter=0;Counter < BytesCount; Counter++) 
    {
		CharAvail = *(SPtr);
		SPtr++;
		Check2Sum ^= CharAvail;
		for (Counter2=0;Counter2<8;Counter2++) 
        {
			a = (Check2Sum & 1) ;
			Check2Sum  = Check2Sum / 2;
			if (a) 
            {
				Check2Sum ^= 0xa001;
			}
		}
	}	
	MSBCSum = (Check2Sum >> 8 ) & 0xff;
	LSBCSum = (Check2Sum & 0xff);
}

void BytesToBits_SET(unsigned int ReceivedData)
{
	unsigned char BitIncrementer;
	unsigned char Result;	
	for(BitIncrementer = 0;BitIncrementer<=15;BitIncrementer++)
	{
		if((ReceivedData>>BitIncrementer) & 1)
			Result = 1;
		else 
			Result = 0;			
	
		I_OStatusLeval[BitIncrementer] = Result;
	}
	
}

void PowerOn_SetBit()
{
    PowerOn_Bit1 = 1;
    PowerOn_Bit2 = 1;
    PowerOn_Bit3 = 1;
    PowerOn_Bit4 = 1;
}


