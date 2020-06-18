#include "Application.h"
//#include "Includes.h"
//#include "Includes.h"-   21/08/2018
//#include "Includes.h"-   16/08/2018
//#include "Includes.h"-   14/08/2018
//#include "Includes.h"-   13/08/2018
//#include "Includes.h"
//#include "Includes.h"
unsigned short CheckSum(unsigned char* ChecksumBuff,unsigned int ByteCount)                //	Calculating LRC for Received protocol	//	Here Received bytes and no.of received bytes are passed
{
	unsigned long LRC_Value = 0;
	unsigned int TempByteCount = 0;

	for(TempByteCount = 0;TempByteCount < ByteCount;TempByteCount++){
		LRC_Value = LRC_Value + ChecksumBuff[TempByteCount];
	}
	LRC_Value = LRC_Value % 100;												//	Extracting Last two digits for LRC value
	return((unsigned short)LRC_Value);																	//	Returning LRC value
}
void Interrupt_Check(void)
{
    unsigned char CheckSumFlag = 0;
	unsigned char ProcessBuff[200] = {0};
	unsigned int ProcessCnt = 0;
	char DataBuff[16] = {0};
	unsigned int LRC = 0;
	char TempVersion[2] = {0};
	unsigned int NumberCheck,Mob_Digit = 0; 
	
	if(ReceivedFlag == 1)
	{
		if((ucount == 65) && (Periodic_Time != 0))
		{
			if( (Flag.AckFlag == 4) && (!strncmp((char *)ReceiveBuff,"BUF1OK",6)) )   //Not used
			{
			//	GPRS_Status_Read(1);
				Flag.AckFlag = 0;
				TimerCntr2 = 0;
                LcdDisplay(TOP,(char *)   "    SMS ALERT   ");
                LcdDisplay(BOTTOM,(char *)"  Sent  Success ");
                    Delay1_1s(3);
               
			}
			else if( (Flag.AckFlag == 2) && (!strncmp((char *)ReceiveBuff,"BUF2OK",6)) )  // latched Report
			{
			//	GPRS_Status_Read(1);
//				Flag.Process_Send_Flag = 0;
                Flag.Latched_Send_Flag = 0;
//                Flag.Periodic_Send_Flag = 0;                
				Flag.AckFlag = 0;
				TimerCntr2 = 0;
                
			}
/*			else if( (Flag.AckFlag == 9) && (!strncmp((char *)ReceiveBuff,"BUF9OK",6)) )  //Periodic without storing
			{
			//	GPRS_Status_Read(1);
				Flag.AckFlag = 0;
				TimerCntr2 = 0;
//				Flag.Periodic_Send_Cntr = 0;
//				Flag.Periodic_Send_Flag = 0;
			}*/
			else if((Flag.AckFlag == 1) && (!strncmp((char *)ReceiveBuff,"BUF3OK",6)))   //Periodic 
			{
//				GPRS_Status_Read(1);
//                Flag.Latched_Send_Flag = 0;
                Flag.Periodic_Send_Flag = 0;                
				Flag.AckFlag = 0;
				TimerCntr2 = 0;
                
			}
			//Flag.GPRS_COMErr_Flag = 0;
			Rx_ClearVar();
		}
		else
		{
            if(!strncmp((char *)ReceiveBuff,"MRD",3))
			{
//				LcdDisplay(TOP," Reach Stacker 1");Delay1_1s(10);
					if((ReceiveBuff[3] == '*') && (ReceiveBuff[6] == '*') && (ReceiveBuff[10] == '*'))
					{
						Mob_Digit = 0;
						for(NumberCheck=3;NumberCheck<=22;NumberCheck++)
						{
								if(ReceiveBuff[NumberCheck] != '*')
								{
									MobileNumBuff[Mob_Digit] = ReceiveBuff[NumberCheck];
									Mob_Digit++;
									MobileNumRdy = 1;
                                    
//                                    LcdDisplay(TOP,MobileNumBuff);Delay1_1s(2);

								}
						}
					}
					Rx_ClearVar();
			}
            if(!strncmp((char *)ReceiveBuff,"VSCRD",5))
			{
					if(ReceiveBuff[5] == '*') 
					{
						MobileNumBuff[0] = ReceiveBuff[6];
						MobileNumBuff[1] = ReceiveBuff[7];
						MobileNumBuff[2] = ReceiveBuff[8];
						MobileNumBuff[3] = ReceiveBuff[9];
						MobileNumBuff[4] = ReceiveBuff[10];
						MobileNumBuff[5] = ReceiveBuff[11];
						MobileNumBuff[6] = ReceiveBuff[12];
						MobileNumBuff[7] = ReceiveBuff[13];
						MobileNumBuff[8] = ReceiveBuff[14];
						MobileNumBuff[9] = ReceiveBuff[15];
					
						MobileNumRdy = 1;
					}
					Rx_ClearVar();
			}
            if(!strncmp((char *)ReceiveBuff,"VGLRD",5))
			{
					if(ReceiveBuff[5] == '*') 
					{
						MobileNumBuff[0] = ReceiveBuff[6];
						MobileNumBuff[1] = ReceiveBuff[7];
						MobileNumBuff[2] = ReceiveBuff[8];
						MobileNumBuff[3] = ReceiveBuff[9];
						MobileNumBuff[4] = ReceiveBuff[10];
						MobileNumBuff[5] = ReceiveBuff[11];
						MobileNumBuff[6] = ReceiveBuff[12];
						MobileNumBuff[7] = ReceiveBuff[13];
						MobileNumBuff[8] = ReceiveBuff[14];
						MobileNumBuff[9] = ReceiveBuff[15];
					    MobileNumBuff[10] = ReceiveBuff[16];
						MobileNumBuff[11] = ReceiveBuff[17];
						MobileNumBuff[12] = ReceiveBuff[18];
					    MobileNumBuff[13] = ReceiveBuff[19];
						MobileNumBuff[14] = ReceiveBuff[20];
						
						
						
					
						MobileNumRdy = 1;
					}
					Rx_ClearVar();
			}
           // Rx_ClearVar();
//			if(!strncmp((char *)ReceiveBuff,"STS001",6))	//[STS001LL]
//			{
//				Flag.GPRS_COMErr_Flag = 0;
//				Rx_ClearVar();
//			}
//			else if(!strncmp((char *)ReceiveBuff,"STATUS",6)) //STATUS*
//			{
//				//GPRS_Status_Read(1);
//				Flag.GPRS_COMErr_Flag = 0;
//			}
		}
        
            if(!strncmp((char *)ReceiveBuff,"ERR",3))
			{
                Flag.AckFlag = 0;
				TimerCntr2 = 0;
            }
        
        
        
        
        ReceivedFlag = 0;
//		ProcessCnt = ucount;
//		memcpy(ProcessBuff,ReceiveBuff,ProcessCnt);
//		LRC = CheckSum(ProcessBuff,(ProcessCnt-2));							//	LRC Calculation
//		
//		memset(DataBuff,' ',16);
//		memcpy(DataBuff,ProcessBuff+(ProcessCnt-2),2);
//		if(LRC == atoi(DataBuff))
//			CheckSumFlag = 1;
//		else 
//			CheckSumFlag = 0;
//        
//        if(CheckSumFlag == 1)
//		{
//			if(!strncmp((char *)ReceiveBuff,"MRD",3))
//			{
//					if((ReceiveBuff[3] == '*') && (ReceiveBuff[6] == '*') && (ReceiveBuff[10] == '*'))
//					{
//						Mob_Digit = 0;
//						for(NumberCheck=3;NumberCheck<=22;NumberCheck++)
//						{
//								if(ReceiveBuff[NumberCheck] != '*')
//								{
//									MobileNumBuff[Mob_Digit] = ReceiveBuff[NumberCheck];
//									Mob_Digit++;
//									MobileNumRdy = 1;
//								}
//						}
//					}
//					Rx_ClearVar();
//			}
//        }
		
	}

}

void Read_EEPROM_Data(void)
{
    int Readfrommemory=0;
    static int loopi = 0;
	unsigned int Tempvariable = 0;    
//    unsigned char MemoryCheck = 0;
//    unsigned char Sec_MemoryCheck = 0;    
//    unsigned char Dump_Prot_Buff[150] = {0};
    while(1)
	{
		KeySense();
		if(LeftKey)
		{
			LcdDisplay(TOP,(char *)" ENTER PASSWORD ");
			LcdDisplay(BOTTOM,(char *)"     RESET      ");
			if(GetPassword() == 1)
			{
                MemoryCheck = 0;
                Sec_MemoryCheck = 0;      
                loopi = 1;
            }
			break;
		}
		if(Tempvariable++>1000)
		{
			Tempvariable = 0;
            loopi = 0;
			break;
		}  
	}	 
	Read_E2((char*)&MemoryCheck,EEPROM_Check,1);Delay_ms(20);
    if((MemoryCheck != 95) || (loopi == 1))	//95
	{
        Read_E2((char*)&Sec_MemoryCheck,EE_Sec_MemoryCheck,1);Delay_ms(20);
        if((Sec_MemoryCheck!=95) || (loopi == 1))
        {
            MemoryCheck=95;
            Write_E2(EEPROM_Check,(char*)&MemoryCheck,1);Delay_ms(10);
            Sec_MemoryCheck=95;
            Write_E2(EE_Sec_MemoryCheck,(char*)&Sec_MemoryCheck,1);Delay_ms(10);
            memcpy(Header1_Name," Reach Stacker  ",16);
            Write_E2(EES_POD_first_Line,(char*)&Header1_Name,16);
            Confirmation_Function(Loaddefault); 
            Confirmation_Function(InitGenSettings);  
			Confirmation_Function(InitAppSettings);
            Reset_Cum_Trip();
            Confirmation_Function(InitVoltValue);        
            Confirmation_Function(TakeLoadCalBackup);  
            Confirmation_Function(TakeVoltBackup);
            
            RT_Temp_Date = RT_Count = 0;
            Write_E2(ERT_Temp_Date,(char *)&RT_Temp_Date,sizeof(RT_Temp_Date));Delay_ms(10);
            Write_E2(ERT_Count,(char *)&RT_Count,sizeof(RT_Count));Delay_ms(10);
            
        }
        else
        {
            Readfrommemory=1;
            MemoryCheck=95;
            Write_E2(EEPROM_Check,(char*)&MemoryCheck,sizeof(MemoryCheck));Delay_ms(10);            
        }
    }
	else
	{
        Readfrommemory=1;
    }
    
    if(Readfrommemory == 1)
    {
        PowerOn_Read();
//        LcdDisplay(TOP,"crossed-2       ");
//        Delay_1sec(10);        
//        for(loopi = 0;loopi < Array_Counter;loopi++)
//        {
//            Print_CRLF(1);
//            sprintf(Dump_Prot_Buff,"%d:%d",loopi,No_of_Samplesarray[loopi]);
//            Com1StringSend(Dump_Prot_Buff,10);    
//        }
//        LcdDisplay(TOP,"crossed-3       ");
//        Delay_1sec(10);        
    }
}


long Volt_calculation(char ChannelNo)
{
	char Dump_Prot_Buff[15] = {0};
    int CurADC = 0;
    long Ch_Volt = 0;    
    CalcADC();    
    if(ChannelNo == 1)
    {
    //    CurADC = (long)ADC_GetConversion(1);
    //    Loadcell[CH1].RawADC = 770;
        Ch_Volt = Calf_Volt((int)Loadcell[CH1].RawADC,(int)VoltageCalib.Ch1Ref1,(int)VoltageCalib.Ch1Ref2,(int)(abs(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1)));//VoltageCalib.ChxRef_Volt);
    }
    else if(ChannelNo == 2)
    {
    //    Loadcell[CH2].RawADC = 770;
        //    CurADC = (long)ADC_GetConversion(0);
        Ch_Volt = Calf_Volt((int)Loadcell[CH2].RawADC,(int)VoltageCalib.Ch2Ref1,(int)VoltageCalib.Ch2Ref2,(int)(abs(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1)));//VoltageCalib.ChxRef_Volt);
    }
    else if(ChannelNo == 3)
    {
    //    Loadcell[CH3].RawADC = 770;        
    //    CurADC = (long)ADC_GetConversion(5);
        Ch_Volt = Calf_Volt((int)Loadcell[CH3].RawADC,(int)VoltageCalib.Ch3Ref1,(int)VoltageCalib.Ch3Ref2,(int)(abs(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1)));//VoltageCalib.ChxRef_Volt);
    }
    else if(ChannelNo == 4)
    {
     //   Loadcell[CH4].RawADC = 770;        
    //    CurADC = (long)ADC_GetConversion(4);
        Ch_Volt = Calf_Volt((int)Loadcell[CH4].RawADC,(int)VoltageCalib.Ch4Ref1,(int)VoltageCalib.Ch4Ref2,(int)(abs(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1)));//VoltageCalib.ChxRef_Volt);
    }
    else if(ChannelNo == 5)
    {
     //   Loadcell[CH5].RawADC = 770;        
    //    CurADC = (long)ADC_GetConversion(4);
        Ch_Volt = Calf_Volt((int)Loadcell[CH5].RawADC,(int)VoltageCalib.Ch5Ref1,(int)VoltageCalib.Ch5Ref2,(int)(abs(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1)));//VoltageCalib.ChxRef_Volt);
    }
    else if(ChannelNo == 6)
    {
     //   Loadcell[CH5].RawADC = 770;        
    //    CurADC = (long)ADC_GetConversion(4);
        Ch_Volt = Calf_Volt((int)Loadcell[CH6].RawADC,(int)VoltageCalib.Ch6Ref1,(int)VoltageCalib.Ch6Ref2,(int)(abs(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1)));//VoltageCalib.ChxRef_Volt);
    }
    return(Ch_Volt);
}

void Volt_Assign(char ChannelNo)
{
	char Dump_Prot_Buff[15] = {0};
    int CurADC = 0;
   long Ch_Volt = 0;    
    Ch_Volt = Volt_calculation(ChannelNo); 
    memset(Dump_Prot_Buff,' ',15);       
    memcpy(Dump_Prot_Buff,"*",1);	
    sprintf(Dump_Prot_Buff+1,"%05d",Ch_Volt);
    PutDp((unsigned char *)Dump_Prot_Buff+1,(long)3,5,1);	
    Com1StringSend(Dump_Prot_Buff,8);                
}

unsigned int CalcLRC(char* ChecksumBuff,unsigned int ByteCount)                //	Calculating LRC for Received protocol	//	Here Received bytes and no.of received bytes are passed
{
	unsigned long LRC_Value = 0;
	unsigned int TempByteCount = 0;
    memset(LcdDispBuff,' ',10);
	for(TempByteCount = 0;TempByteCount < ByteCount;TempByteCount++)
    {
//      Print_CRLF(1);
        LRC_Value = LRC_Value + ChecksumBuff[TempByteCount];
//      sprintf(LcdDispBuff,"%d:%d:%c:%d",TempByteCount,LRC_Value,ChecksumBuff[TempByteCount],ChecksumBuff[TempByteCount]);
//      Com1StringSend(LcdDispBuff,10);    
    }
	LRC_Value = LRC_Value % 100;												//	Extracting Last two digits for LRC value
	return(LRC_Value);															//	Returning LRC value
}

void Periodic_Report_Assign(void)
{
    char i=0;
    ReadRTC_DateTime();
    if(Run_Time_Update != 1)
    {
        RT_Count +=(long)(TimeOut_Cnt3/1000);
        Write_E2(ERT_Count,(char *)&RT_Count,sizeof(RT_Count));Delay_ms(10);
    }
    Run_Time_Update = 0;
	Periodic_Report.Date = RTC.Date;
	Periodic_Report.Month = RTC.Month;
	Periodic_Report.Year = RTC.Year;
	Periodic_Report.Hour = RTC.Hour;
	Periodic_Report.Minute = RTC.Minute;
	Periodic_Report.Second = RTC.Second;
	Periodic_Report.SumWeight = AllChannel.CurWeight;
    TimeOut_Cnt3 = 0;
    for(i=0;i<NO_OF_CH;i++)
    {
        Periodic_Report.RawADC[i] = Loadcell[i].RawADC;          
        Periodic_Report.CurWeight[i] = (long)Loadcell[i].CurWeight;
    }
	Periodic_Report.Average_Cnt = Average_Count;
	Periodic_Report.Trip_Cnt = AllChannel.TripCount;
	if(IPARKING != Application_Setting.Input1Logic)
        Periodic_Report.Input[0] = '0'; 
	else
        Periodic_Report.Input[0] = '1';
	if(IBODYUP !=	Application_Setting.Input2Logic)
        Periodic_Report.Input[1] = '0';
	else
        Periodic_Report.Input[1] = '1';
    if(MinWt_Check_Counter == 1)
        Periodic_Report.PLMS_Status = 'D';
    else
        Periodic_Report.PLMS_Status = 'L';
    /*    if(Setup.PLMS_Status == LOAD)
        Periodic_Report.PLMS_Status = 'L';
    if(Setup.PLMS_Status == RETURN)	
        Periodic_Report.PLMS_Status = 'R';
    if(Setup.PLMS_Status == EMPTY)	
        Periodic_Report.PLMS_Status = 'E';
    if(Setup.PLMS_Status == DUMP)
        Periodic_Report.PLMS_Status = 'D';	
    if(Setup.PLMS_Status == INIT)
        Periodic_Report.PLMS_Status = 'I'; 
    if(Setup.PLMS_Status == HAULING)
        Periodic_Report.PLMS_Status = 'H'; */
    
	Periodic_Report.Average_Weight = 0;//(unsigned long)Average_Wt;
}

void Latched_Report_Assign(void)
{
    char i=0;
    ReadRTC_DateTime();
	Latch_Report.Date = RTC.Date;
	Latch_Report.Month = RTC.Month;
	Latch_Report.Year = RTC.Year;
	Latch_Report.Hour = RTC.Hour;
	Latch_Report.Minute = RTC.Minute;
	Latch_Report.Second = RTC.Second;
	Latch_Report.SumWeight = 0;
    
	Latch_Report.Average_Cnt = 0;
	Latch_Report.Trip_Cnt = 0;
	if(IPARKING != Application_Setting.Input1Logic)
        Latch_Report.Input[0] = '0'; 
	else
        Latch_Report.Input[0] = '1';
	if(IBODYUP !=	Application_Setting.Input2Logic)
        Latch_Report.Input[1] = '0';
	else
        Latch_Report.Input[1] = '1';

   /* if(Setup.PLMS_Status == LOAD)
        Latch_Report.PLMS_Status = 'L';
    if(Setup.PLMS_Status == RETURN)	
        Latch_Report.PLMS_Status = 'R';
    if(Setup.PLMS_Status == EMPTY)	
        Latch_Report.PLMS_Status = 'E';
    if(Setup.PLMS_Status == DUMP)
        Latch_Report.PLMS_Status = 'D';	
    if(Setup.PLMS_Status == INIT)
        Latch_Report.PLMS_Status = 'I';   
    if(Setup.PLMS_Status == HAULING)*/
    if(MinWt_Check_Counter == 1)
        Latch_Report.PLMS_Status = ' ';
    else
        Latch_Report.PLMS_Status = ' ';  
	Latch_Report.Average_Weight = 0;//(unsigned long)Average_Wt;
}
void SMS_Alert(char status)
{
    unsigned int TotalChar = 0;
    unsigned int LRC = 0;
	unsigned char Dump_Prot_Buff[200] = {0},lcdtump[32] ={0}, i =0; 
    unsigned int   Minute = 0,Hour = 0, Sec = 0;
    Print_CRLF(1);
    memset(Dump_Prot_Buff," ",200);
    //ReadRTC_DateTime();
	 CalcADC();
    memcpy((Dump_Prot_Buff+(TotalChar)),"11System Alert:",15);
    
    sprintf((char *)(Dump_Prot_Buff+(TotalChar+=15)),"%02lu\r\n",System_setting.DeviceID);
    //memset(Dump_Prot_Buff+16," ",105);
    if(status == 1)
    {
         memcpy((Dump_Prot_Buff+(TotalChar+=4)),"Engine-IP:HIGH\r\n",16);
         if(IBODYUP !=	Application_Setting.Input2Logic)
         memcpy((Dump_Prot_Buff+(TotalChar+=16)),"Parking-IP:LOW \r\n",17); 
         else
         memcpy((Dump_Prot_Buff+(TotalChar+=16)),"Parking-IP:HIGH\r\n",17);
         
     //   Dump_Prot_Buff+=16;
         if(Bits.ORERRFlag1 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=17)),"HWT :HIGH\r\n",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=17)),"HWT :LOW \r\n",Loadcell[0].RawADC);
         if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LOP :HIGH\r\n",Loadcell[1].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LOP :LOW \r\n",Loadcell[1].RawADC);
         if(Bits.ORERRFlag3 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"OST :HIGH\r\n",Loadcell[2].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"OST :LOW \r\n",Loadcell[2].RawADC);
         if(Bits.ORERRFlag4 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LCL :HIGH\r\n",Loadcell[3].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LCL :LOW \r\n",Loadcell[3].RawADC);
         if(Bits.ORERRFlag5 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TCO :HIGH\r\n",Loadcell[4].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TCO :LOW \r\n",Loadcell[4].RawADC);
         if(Bits.ORERRFlag6 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TL  :HIGH\r\n",Loadcell[5].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TL  :LOW \r\n",Loadcell[5].RawADC);
         
          
         
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"DT:%02u/%02u/%04u ",RTC.Date,RTC.Month,RTC.Year);    //14 byte
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=14)),"T:%02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);
//         memcpy((char*) Dump_Prot_Buff+(TotalChar+=11),"   ",3);
         memcpy((char*) Dump_Prot_Buff+(TotalChar+=10),"   ",3);
         TotalChar+=3;
    }
    if(status == 2)
    {
        
                
            
        Minute = ((RT_Count)/60);  // Run Time Calculation
                if(Minute>60)
                     Minute = (int)(((RT_Count)/60)%60);
        Hour = (int)(((RT_Count)/60)/60);
        Sec =(int) ((RT_Count)%60);
        memcpy((Dump_Prot_Buff+(TotalChar+=4)),"EngineIP:LOW\r\n",14);
        if(IBODYUP !=	Application_Setting.Input2Logic)
        memcpy((Dump_Prot_Buff+(TotalChar+=14)),"ParkingIP:LOW \r\n",16); 
        else
        memcpy((Dump_Prot_Buff+(TotalChar+=14)),"ParkingIP:HIGH\r\n",16);
        
        

         
          if(Bits.ORERRFlag1 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=16)),"HWT:HIGH\r\n",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=16)),"HWT:LOW \r\n",Loadcell[0].RawADC);
         if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"LOP:HIGH\r\n",Loadcell[1].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"LOP:LOW \r\n",Loadcell[1].RawADC);
         if(Bits.ORERRFlag3 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"OST:HIGH\r\n",Loadcell[2].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"OST:LOW \r\n",Loadcell[2].RawADC);
         if(Bits.ORERRFlag4 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"LCL:HIGH\r\n",Loadcell[3].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"LCL:LOW \r\n",Loadcell[3].RawADC);
         if(Bits.ORERRFlag5 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"TCO:HIGH\r\n",Loadcell[4].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"TCO:LOW \r\n",Loadcell[4].RawADC);
         if(Bits.ORERRFlag6 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"TL :HIGH\r\n",Loadcell[5].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"TL :LOW \r\n",Loadcell[5].RawADC);
         
          
         
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=10)),"RT:%02d:%02d:%02d ",Hour,Minute,Sec);
        
        //TotalChar+=12;
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=12)),"DT:%02u/%02u/%04u ",RTC.Date,RTC.Month,RTC.Year);    //14 byte
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=14)),"T:%02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);
//         memcpy((char*) Dump_Prot_Buff+(TotalChar+=11),"   ",3);
         TotalChar+=10;
    }
    if(status == 3)
    {
        if(IPARKING != Application_Setting.Input1Logic)
        memcpy((Dump_Prot_Buff+(TotalChar+=4)),"Engine-IP:LOW \r\n",16);         
        else
        memcpy((Dump_Prot_Buff+(TotalChar+=4)),"Engine-IP:HIGH\r\n",16);
        
        memcpy((Dump_Prot_Buff+(TotalChar+=16)),"Parking-IP:HIGH\r\n",17);
        
      //      TotalChar+=16;
         
         if(Bits.ORERRFlag1 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=17)),"HWT :HIGH\r\n",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=17)),"HWT :LOW \r\n",Loadcell[0].RawADC);
         if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LOP :HIGH\r\n",Loadcell[1].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LOP :LOW \r\n",Loadcell[1].RawADC);
         if(Bits.ORERRFlag3 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"OST :HIGH\r\n",Loadcell[2].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"OST :LOW \r\n",Loadcell[2].RawADC);
         if(Bits.ORERRFlag4 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LCL :HIGH\r\n",Loadcell[3].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LCL :LOW \r\n",Loadcell[3].RawADC);
         if(Bits.ORERRFlag5 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TCO :HIGH\r\n",Loadcell[4].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TCO :LOW \r\n",Loadcell[4].RawADC);
         if(Bits.ORERRFlag6 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TL  :HIGH\r\n",Loadcell[5].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TL  :LOW \r\n",Loadcell[5].RawADC);
         
          
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"DT:%02u/%02u/%04u ",RTC.Date,RTC.Month,RTC.Year);    //14 byte
         sprintf((char *)(Dump_Prot_Buff+(TotalChar+=14)),"T:%02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);
         memcpy((char*) Dump_Prot_Buff+(TotalChar+=10),"   ",3);
         TotalChar+=3;
    }
    if(status == 4)
    {
        if(IPARKING != Application_Setting.Input1Logic)
        memcpy((Dump_Prot_Buff+(TotalChar+=4)),"Engine-IP:LOW \r\n",16);         
        else
        memcpy((Dump_Prot_Buff+(TotalChar+=4)),"Engine-IP:HIGH\r\n",16);
        
        memcpy((Dump_Prot_Buff+(TotalChar+=16)),"Parking-IP:LOW \r\n",17);
        
        
        
         
         if(Bits.ORERRFlag1 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=17)),"HWT :HIGH\r\n",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=17)),"HWT :LOW \r\n",Loadcell[0].RawADC);
         if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LOP :HIGH\r\n",Loadcell[1].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LOP :LOW \r\n",Loadcell[1].RawADC);
         if(Bits.ORERRFlag3 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"OST :HIGH\r\n",Loadcell[2].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"OST :LOW \r\n",Loadcell[2].RawADC);
         if(Bits.ORERRFlag4 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LCL :HIGH\r\n",Loadcell[3].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"LCL :LOW \r\n",Loadcell[3].RawADC);
         if(Bits.ORERRFlag5 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TCO :HIGH\r\n",Loadcell[4].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TCO :LOW \r\n",Loadcell[4].RawADC);
         if(Bits.ORERRFlag6 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TL  :HIGH\r\n",Loadcell[5].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"TL  :LOW \r\n",Loadcell[5].RawADC);
         
          
           
          sprintf((char *)(Dump_Prot_Buff+(TotalChar+=11)),"DT:%02u/%02u/%04u ",RTC.Date,RTC.Month,RTC.Year);    //14 byte
          sprintf((char *)(Dump_Prot_Buff+(TotalChar+=14)),"T:%02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);
          memcpy((char*) Dump_Prot_Buff+(TotalChar+=10),"   ",3);
          TotalChar+=3;
    }
    
    
    
        //10 
     
     
            
    LRC = CalcLRC((char *)Dump_Prot_Buff,TotalChar);							//	LRC Calculation  121+24
    sprintf((char *)(Dump_Prot_Buff+TotalChar),"%02d",LRC);
    TotalChar+=2;
    Rx_ClearVar();
    Com2StringSend((char *)"[",1);   
    Com2StringSend((char *)Dump_Prot_Buff,TotalChar);    //TotalChar = 123     123 is SMS Length. It  was fixed 
    Com2StringSend((char *)"]",1);  

//            LcdDisplay(TOP,(char *)   "    SMS ALERT   ");
//            LcdDisplay(BOTTOM,(char *)"   Try to Sent  ");
//            Delay1_1s(1);
            

}

void ProtocolDump(char BankNumber,char PowerON)
{
    char i=0,j=0;
    unsigned int TotalChar = 0;
    unsigned int LRC = 0;
	unsigned char Dump_Prot_Buff[150] = {0};
    unsigned int   Minute = 0,Hour = 0, Sec = 0;
    Print_CRLF(1);
    Minute = ((RT_Count)/60);  // Run Time Calculation
                if(Minute>60)
                     Minute = (int)(((RT_Count)/60)%60);
        Hour = (int)(((RT_Count)/60)/60);
        Sec =(int) ((RT_Count)%60);
//  Below one used for GPRS Module
    sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"%d*",BankNumber);
//    if(BankNumber == 2)
//        BankNumber = 3;
//    else if(BankNumber == 3)
//        BankNumber = 2;
    sprintf((char *)(Dump_Prot_Buff+(TotalChar+=2)),"%02lu",System_setting.DeviceID);
    memcpy((Dump_Prot_Buff+(TotalChar+=2)),"*",1);	
//    if(((AllChannel.Polarity == 1) && (LatchedProt == 0))||((TripRecord.Polarity == 1) && (LatchedProt == 1)))
//      memset((Dump_Prot_Buff+(TotalChar+=1)),'-',1);
//    else
        memset((Dump_Prot_Buff+(TotalChar+=1)),' ',1);      
        
    if(BankNumber == 2)//Latched report
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%04ld",Latch_Report.SumWeight);
    else
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%04ld",Periodic_Report.SumWeight);
    
    PutDp((unsigned char*)(Dump_Prot_Buff+(TotalChar)),DecimalPoint,4,1);
    TotalChar+=5;
   
    for(i=0;i<2;i++)
    {
        memcpy((Dump_Prot_Buff+(TotalChar)),"*",1);	
        if(BankNumber == 2)//Latched report
        {
            if((i==0)||(i==1))
                sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%4ld ",(long)Latch_Report.CurWeight[i]);            
            else
                sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%04ld",(long)Latch_Report.CurWeight[i]);
        }
        else
        {
            if((i==0)||(i==1))
                sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%4ld ",(long)Periodic_Report.CurWeight[i]);                        
            else
                sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%04ld",(long)Periodic_Report.CurWeight[i]);
        }
        if(i > 1)
            PutDp((unsigned char*)(Dump_Prot_Buff+(TotalChar)),DecimalPoint,4,1);       
//        else
//            PutDp((unsigned char*)(Dump_Prot_Buff+(TotalChar)),1,4,1);                  
        TotalChar+=5;
    }
    
         if(Bits.ORERRFlag1 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"* HIGH",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"* LOW ",Loadcell[0].RawADC);
        TotalChar+=6;     
        
        if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"* HIGH",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"* LOW ",Loadcell[0].RawADC);
        TotalChar+=6; 
        
    
         if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*HIGH",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*LOW ",Loadcell[0].RawADC);
        TotalChar+=5; 
        
        if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*HIGH",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*LOW ",Loadcell[0].RawADC);
        TotalChar+=5; 
        
        if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*HIGH",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*LOW ",Loadcell[0].RawADC);
        TotalChar+=5; 
        
        if(Bits.ORERRFlag2 == 1)
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*HIGH",Loadcell[0].RawADC);
         else
             sprintf((char *)(Dump_Prot_Buff+(TotalChar)),"*LOW ",Loadcell[0].RawADC);
        TotalChar+=5; 
        
        
        
    Dump_Prot_Buff[TotalChar++] = '*';	
    if(BankNumber == 2)    
    {
        Dump_Prot_Buff[TotalChar++] = Latch_Report.Input[0];
        Dump_Prot_Buff[TotalChar++] = Latch_Report.Input[1];
    }
    else 
    {
        Dump_Prot_Buff[TotalChar++] = Periodic_Report.Input[0];
        Dump_Prot_Buff[TotalChar++] = Periodic_Report.Input[1];   
    }
    Dump_Prot_Buff[TotalChar] = '*';
    if(PowerON == 1)
        memset((Dump_Prot_Buff+(TotalChar+=1)),'1',2);
    else
        memset((Dump_Prot_Buff+(TotalChar+=1)),'0',2);
    Dump_Prot_Buff[(TotalChar+=2)] = '*';
    if(BankNumber == 2)
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%04u",Latch_Report.Month,Latch_Report.Date,Latch_Report.Year);    
    else
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%04u",Periodic_Report.Month,Periodic_Report.Date,Periodic_Report.Year);    
    Dump_Prot_Buff[(TotalChar+=8)] = '*';
    if(BankNumber == 2)    
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%02u",Hour,Minute,Sec);    
    else
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%02u",Periodic_Report.Hour,Periodic_Report.Minute,Periodic_Report.Second);    
    Dump_Prot_Buff[(TotalChar+=6)] = '*';
    
        memcpy((char *)(Dump_Prot_Buff+(TotalChar+=1)),"0000",4);    
    Dump_Prot_Buff[(TotalChar+=4)] = '*';
    sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%02u",Hour,Minute,Sec);  // Run Time   
    Dump_Prot_Buff[(TotalChar+=6)] = '*';    
    
        memcpy((char *)(Dump_Prot_Buff+(TotalChar+=1)),"00000",5);    
    Dump_Prot_Buff[TotalChar+=5] = '*';
    LRC = CalcLRC((char *)Dump_Prot_Buff,(TotalChar+1));							//	LRC Calculation
    sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02d",LRC);
    TotalChar+=2;
    Rx_ClearVar();
    Com2StringSend((char *)"[",1);   
    Com2StringSend((char *)Dump_Prot_Buff,TotalChar);    
    Com2StringSend((char *)"]",1);    
    
//Below one used for Protocol dump     
/*	char Dump_Prot_Buff[15] = {0};
    char i=0;
    Print_CRLF(1);
    memcpy(Dump_Prot_Buff,"%*",2);
    sprintf(Dump_Prot_Buff+2,"%02d",Setup.DeviceID);
    Com1StringSend(Dump_Prot_Buff,5);   
    memset(Dump_Prot_Buff,' ',15);       
    memcpy(Dump_Prot_Buff,"*",1);	
    if(AllChannel.Polarity == 1) 
        memset(Dump_Prot_Buff+1,'-',1);                 
    else
        memset(Dump_Prot_Buff+1,' ',1);      
    sprintf(Dump_Prot_Buff+2,"%04ld",AllChannel.CurWeight);
    PutDp((unsigned char*)Dump_Prot_Buff+2,DecimalPoint,4,1);
    Com1StringSend(Dump_Prot_Buff,8);
    for(i=0;i<2;i++)
    {
        memset(Dump_Prot_Buff,' ',15);       
        memcpy(Dump_Prot_Buff,"*",1);	
        sprintf(Dump_Prot_Buff+1,"%04ld",Axle[i].CurWeight);
        PutDp((unsigned char*)Dump_Prot_Buff+1,DecimalPoint,4,1);            
        Com1StringSend(Dump_Prot_Buff,7);
    }
    for(i=0;i<4;i++)
    {
        memset(Dump_Prot_Buff,' ',15);   
        memcpy(Dump_Prot_Buff,"*",1);	
        sprintf(Dump_Prot_Buff+1,"%04ld",Loadcell[i].CurWeight);
        PutDp((unsigned char*)Dump_Prot_Buff+1,DecimalPoint,4,1);            
        Com1StringSend(Dump_Prot_Buff,7);
    }
    Append_IO_Status(Dump_Prot_Buff);		//Append IO Status "[III][OOO]"        
    Com1StringSend(Dump_Prot_Buff,7);
    for(i=0;i<4;i++)
    {
        memset(Dump_Prot_Buff,' ',15);   
        memcpy(Dump_Prot_Buff,"*",1);	
        sprintf(Dump_Prot_Buff+1,"%4ld",Loadcell[i].RawADC);
    //    PutDp((unsigned char*)Dump_Prot_Buff+1,DecimalPoint,4,1);            
        Com1StringSend(Dump_Prot_Buff,7);
    }
    memset(Dump_Prot_Buff,' ',15);   
    memcpy(Dump_Prot_Buff,"*",1);	
    if(Setup.PLMS_Status == LOAD)
        Dump_Prot_Buff[1] = 'L';
    if(Setup.PLMS_Status == RETURN)	
        Dump_Prot_Buff[1] = 'R';
    if(Setup.PLMS_Status == EMPTY)	
        Dump_Prot_Buff[1] = 'E';
    if(Setup.PLMS_Status == DUMP)
        Dump_Prot_Buff[1] = 'D';	
    if(Setup.PLMS_Status == INIT)
        Dump_Prot_Buff[1] = 'I';  
    Com1StringSend(Dump_Prot_Buff,7);    
    Com1StringSend("$",1);    */
}
void PC_Dump(void)  //have to check polarity of sumweight
{
    unsigned int TotalChar = 0;
    unsigned int LRC = 0;
	unsigned char Dump_Prot_Buff[150] = {0};
    Print_CRLF(1);
    memcpy((Dump_Prot_Buff+(TotalChar)),"%",1);
    sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02lu",System_setting.DeviceID);
    memcpy((Dump_Prot_Buff+(TotalChar+=2)),"*",1);
    
    if(Bits.Disp_Latch_Wt == 1)
    {
       sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%04ld",AllChannel.LatchedWt);
    }
    else
    {
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%04ld",AllChannel.CurWeight);
    }
    PutDp((unsigned char*)(Dump_Prot_Buff+(TotalChar)),DecimalPoint,4,1);
//    TotalChar+=5;
    Dump_Prot_Buff[TotalChar+=5] = '*';
//    Dump_Prot_Buff[(TotalChar+=1)] = Application_Setting.PLMS_Status;
    if(Application_Setting.PLMS_Status == LOAD)
        Dump_Prot_Buff[(TotalChar+=1)] = 'L';
    if(Application_Setting.PLMS_Status == HAULING)
        Dump_Prot_Buff[(TotalChar+=1)] = 'H';
    if(Application_Setting.PLMS_Status == RETURN)	
        Dump_Prot_Buff[(TotalChar+=1)] = 'R';
    if(Application_Setting.PLMS_Status == EMPTY)	
        Dump_Prot_Buff[(TotalChar+=1)] = 'E';
    if(Application_Setting.PLMS_Status == DUMP)
        Dump_Prot_Buff[(TotalChar+=1)] = 'D';	
    if(Application_Setting.PLMS_Status == INIT)
        Dump_Prot_Buff[(TotalChar+=1)] = 'I';
    
    Dump_Prot_Buff[TotalChar+=1] = '*';
//    ReadRTC_DateTime();
    if(Bits.Disp_Latch_Wt == 1)
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%04u",TripRecord.Month,TripRecord.Date,TripRecord.Year);    
    else
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%04u",RTC.Month,RTC.Date,RTC.Year);    
    Dump_Prot_Buff[(TotalChar+=8)] = '*';
    if(Bits.Disp_Latch_Wt == 1)
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%02u",TripRecord.Hour,TripRecord.Minute,TripRecord.Second);    
    else
        sprintf((char *)(Dump_Prot_Buff+(TotalChar+=1)),"%02u%02u%02u",RTC.Hour,RTC.Minute,RTC.Second);    
    Dump_Prot_Buff[(TotalChar+=6)] = '*';
    
    if(IPARKING != Application_Setting.Input1Logic)
        Dump_Prot_Buff[TotalChar+=1] = '0'; 
	else
        Dump_Prot_Buff[TotalChar+=1] = '1';
	if(IBODYUP !=	Application_Setting.Input2Logic)
        Dump_Prot_Buff[TotalChar+=1] = '0';
	else
        Dump_Prot_Buff[TotalChar+=1] = '1';
    
    Dump_Prot_Buff[TotalChar+=1] = '*';
    Dump_Prot_Buff[TotalChar+=1] = '0';
    TotalChar+=1;
    Com2StringSend(Dump_Prot_Buff,TotalChar);    
    Com2StringSend("$",1);
    
//Below one used for Protocol dump     
/*	char Dump_Prot_Buff[15] = {0};
    char i=0;
    Print_CRLF(1);
    memcpy(Dump_Prot_Buff,"%*",2);
    sprintf(Dump_Prot_Buff+2,"%02d",Setup.DeviceID);
    Com1StringSend(Dump_Prot_Buff,5);   
    memset(Dump_Prot_Buff,' ',15);       
    memcpy(Dump_Prot_Buff,"*",1);	
    if(AllChannel.Polarity == 1) 
        memset(Dump_Prot_Buff+1,'-',1);                 
    else
        memset(Dump_Prot_Buff+1,' ',1);      
    sprintf(Dump_Prot_Buff+2,"%04ld",AllChannel.CurWeight);
    PutDp((unsigned char*)Dump_Prot_Buff+2,DecimalPoint,4,1);
    Com1StringSend(Dump_Prot_Buff,8);
 
    Append_IO_Status(Dump_Prot_Buff);		//Append IO Status "[III][OOO]"        
    Com1StringSend(Dump_Prot_Buff,7);
    for(i=0;i<4;i++)
    {
        memset(Dump_Prot_Buff,' ',15);   
        memcpy(Dump_Prot_Buff,"*",1);	
        sprintf(Dump_Prot_Buff+1,"%4ld",Loadcell[i].RawADC);
    //    PutDp((unsigned char*)Dump_Prot_Buff+1,DecimalPoint,4,1);            
        Com1StringSend(Dump_Prot_Buff,7);
    }
    memset(Dump_Prot_Buff,' ',15);   
    memcpy(Dump_Prot_Buff,"*",1);	
    if(Setup.PLMS_Status == LOAD)
        Dump_Prot_Buff[1] = 'L';
    if(Setup.PLMS_Status == RETURN)	
        Dump_Prot_Buff[1] = 'R';
    if(Setup.PLMS_Status == EMPTY)	
        Dump_Prot_Buff[1] = 'E';
    if(Setup.PLMS_Status == DUMP)
        Dump_Prot_Buff[1] = 'D';	
    if(Setup.PLMS_Status == INIT)
        Dump_Prot_Buff[1] = 'I';  
    Com1StringSend(Dump_Prot_Buff,7);    
    Com1StringSend("$",1);    */
}

void Calibration_Dump(void)
{
    char testing=0;
	char Dump_Prot_Buff[32] = {0};
    Print_CRLF(1);
/*  sprintf(Dump_Prot_Buff,"%04d",VoltageCalib.Ch1Ref1);   
    sprintf(Dump_Prot_Buff+5,"%04d",VoltageCalib.Ch2Ref1);   
    sprintf(Dump_Prot_Buff+10,"%04d",VoltageCalib.Ch3Ref1);   
    sprintf(Dump_Prot_Buff+16,"%04d",VoltageCalib.Ch4Ref1);   
    sprintf(Dump_Prot_Buff+21,"%04d",VoltageCalib.Ch5Ref1);   
    sprintf(Dump_Prot_Buff+26,"%04d",VoltageCalib.Ch1Ref2);   
    Com1StringSend(Dump_Prot_Buff,32);
    memset(Dump_Prot_Buff,' ',25);          
    Print_CRLF(1);   
    sprintf(Dump_Prot_Buff,"%04d",VoltageCalib.Ch2Ref2);   
    sprintf(Dump_Prot_Buff+5,"%04d",VoltageCalib.Ch3Ref2);   
    sprintf(Dump_Prot_Buff+10,"%04d",VoltageCalib.Ch4Ref2);   
    sprintf(Dump_Prot_Buff+16,"%04d",VoltageCalib.Ch5Ref2);   
    sprintf(Dump_Prot_Buff+21,"%04d",VoltageCalib.REFvolt1);   
    sprintf(Dump_Prot_Buff+26,"%04d",VoltageCalib.REFvolt2);   
    Com1StringSend(Dump_Prot_Buff,32);
    memset(Dump_Prot_Buff,' ',25);      */    
    memcpy(Dump_Prot_Buff,"Max Capacity:",13);
    sprintf(Dump_Prot_Buff+15,"%05ld",MaxCapacity);
    PutDp((unsigned char*)Dump_Prot_Buff+15,DecimalPoint,5,1);
    Com2StringSend(Dump_Prot_Buff,25);
    memset(Dump_Prot_Buff,' ',25);          
    Print_CRLF(1);
    memcpy(Dump_Prot_Buff,"  Resolution:",13);
    sprintf(Dump_Prot_Buff+15,"%d",Resolution);
    Com2StringSend(Dump_Prot_Buff,25);
    memset(Dump_Prot_Buff,' ',25);          
    Print_CRLF(1);
    memcpy(Dump_Prot_Buff,"  Decimal Pt:",13);
    sprintf(Dump_Prot_Buff+15,"%d",DecimalPoint);
    Com2StringSend(Dump_Prot_Buff,25);
    memset(Dump_Prot_Buff,' ',25);          
    Print_CRLF(1);
    memcpy(Dump_Prot_Buff,"        Unit:",13);
    if((Unit-1) == 0)
        memcpy(Dump_Prot_Buff+15,"Kg",2);
    else
        memcpy(Dump_Prot_Buff+15," T",2);        
    Com2StringSend(Dump_Prot_Buff,25);
    memset(Dump_Prot_Buff,' ',25);          
    Print_CRLF(1);
    Com2StringSend((char *)" CH.No.    CalZero    CalZero1    CalSpan   CalCapacity",55);
    for(testing=0;testing<NO_OF_CH;testing++)
    {
        Print_CRLF(1);
        sprintf((char *)Dump_Prot_Buff,"   %d        %4ld        %4ld",(testing+1),Loadcell[testing].Zero,Loadcell[testing].Zero1);
        Com2StringSend(Dump_Prot_Buff,28);//Delay(5000);

        if(testing < 2)
        {
            sprintf((char *)Dump_Prot_Buff,"        %4ld      %5ld  ",Loadcell[testing].Span,Loadcell[testing].CalCapacity);
        }
        else
        {
            sprintf((char *)Dump_Prot_Buff,"        %4ld      %05ld    ",Loadcell[testing].Span,Loadcell[testing].CalCapacity);            
            PutDp((unsigned char*)Dump_Prot_Buff+18,DecimalPoint,5,1);    
        }
        Com2StringSend(Dump_Prot_Buff,26);//Delay(5000);
//        memset(Dump_Prot_Buff,' ',30);         
    }
}

/*
void Append_IO_Status(char *Buffer)
{
	(*Buffer++) = '*';
	if(ILATCH != Setup.InputLogic)
    {
		(*Buffer++) = '0';
	}
	else
    {
		(*Buffer++) = '1';
	}
	if(IRUN !=	Setup.InputLogic)
    {
		(*Buffer++) = '0';
	}
	else
    {
		(*Buffer++) = '1';
	}
	(*Buffer++) = '*';
	(*Buffer++) = '0';
    (*Buffer++) = '0';
//	(*Buffer++) = '*';
}
*/
/*void CAN_Transmission(void)
{
//    CANDataFlow();
//    if(CAN_Tx_Flag == 1)
//    {
//        sprintf(CAN_Tx_Buff,"%07ld",AllChannel.CurWeight);
//        CAN_Trans_Data.frame.idType = 2;
//        CAN_Trans_Data.frame.id = 0x18FEEAFF;
//        CAN_Trans_Data.frame.dlc = 7;
//        CAN_Trans_Data.frame.data0 = CAN_Tx_Buff[0];
//        CAN_Trans_Data.frame.data1 = CAN_Tx_Buff[1];
//        CAN_Trans_Data.frame.data2 = CAN_Tx_Buff[2];
//        CAN_Trans_Data.frame.data3 = CAN_Tx_Buff[3];
//        CAN_Trans_Data.frame.data4 = CAN_Tx_Buff[4];
//        CAN_Trans_Data.frame.data5 = CAN_Tx_Buff[5];
//        CAN_Trans_Data.frame.data6 = CAN_Tx_Buff[6];
//        CAN_Trans_Data.frame.data7 = CAN_Tx_Buff[7];  
//        CAN_transmit(&CAN_Trans_Data); //  Delay(1000);  
//    }
//    else
//    {
//        if(CAN_Tx_Flag == 1)//if(CAN_messagesInBuffer())        
//        {
//            CAN_receive(&CAN_Rxed_Data);            
//            CAN_Trans_Data.frame.idType = CAN_Rxed_Data.frame.idType;
//            CAN_Trans_Data.frame.id = CAN_Rxed_Data.frame.id;
//            CAN_Trans_Data.frame.dlc = CAN_Rxed_Data.frame.dlc;
//            CAN_Trans_Data.frame.data0 = CAN_Rxed_Data.frame.data0;
//            CAN_Trans_Data.frame.data1 = CAN_Rxed_Data.frame.data1;
//            CAN_Trans_Data.frame.data2 = CAN_Rxed_Data.frame.data2;
//            CAN_Trans_Data.frame.data3 = CAN_Rxed_Data.frame.data3;
//            CAN_Trans_Data.frame.data4 = CAN_Rxed_Data.frame.data4;
//            CAN_Trans_Data.frame.data5 = CAN_Rxed_Data.frame.data5;
//            CAN_Trans_Data.frame.data6 = CAN_Rxed_Data.frame.data6;
//            CAN_Trans_Data.frame.data7 = CAN_Rxed_Data.frame.data7;    
//            CAN_transmit(&CAN_Trans_Data); //  Delay(1000);  
//            CAN_Tx_Flag = 0;
//        }
//    }
//            sprintf(LcdDispBuff,"%x:%x:%8x",CAN_Rxed_Data.frame.idType,CAN_Rxed_Data.frame.dlc,CAN_Rxed_Data.frame.id);
//            sprintf(LcdDispBuff+16,"%x:%x:%x:%x:%x:%x:%x:%x",CAN_Rxed_Data.frame.data0,CAN_Rxed_Data.frame.data1,CAN_Rxed_Data.frame.data2,
//                    CAN_Rxed_Data.frame.data3,CAN_Rxed_Data.frame.data4,CAN_Rxed_Data.frame.data5,CAN_Rxed_Data.frame.data6,CAN_Rxed_Data.frame.data7);
//            LcdDisplay(TOP,LcdDispBuff);
//            LcdDisplay(BOTTOM,LcdDispBuff+16);
//            Delay_1sec(25);                    
}
*/
long Calf_Volt(int CurCnt,int R1Cnt,int R2Cnt,int R2Volt)
{
	long Ch_Volt;
	float Cur_Volt = 0;
	Cur_Volt = (float)((float)(R2Volt)/ (float)abs(R2Cnt - R1Cnt));	
	Cur_Volt = (float)((float)(Cur_Volt)*(float)abs(CurCnt));// - R1Cnt));
//	Ch_Volt = (long)(Cur_Volt * 100);
//	Ch_Volt = RoundOff(Ch_Volt,100);
//	Ch_Volt = (Ch_Volt/100);
//	if(CurCnt <= 15)	// to zero the mA under 1 mA
//		Ch_Volt = VoltageCalib.Offset_Volt;
	return((long)Cur_Volt);	
}

void Calc_Volt()			/* For Checking Cumulative mA */
{
//	Enb_Volt = (int)(VoltageCalib.REFvolt2-VoltageCalib.REFvolt1);
//	memset(LcdDispBuff,' ',32);
	while(1)
	{
		Weight_Calc();
		sprintf(LcdDispBuff,"Cum Cnt:  %6d",AllChannel.RAWADC);
		memcpy(LcdDispBuff+16,"Cum Ld :             ",16);
		sprintf(LcdDispBuff+25,"%04ld",AllChannel.CurWeight);
		PutDp((unsigned char*)LcdDispBuff+25,DecimalPoint,4,1);
//            if(AllChannel.Polarity == 1) 
//                memset(LcdDispBuff+24,'-',1);                 
//            else
//                memset(LcdDispBuff+24,' ',1);  
		LcdDisplay(TOP,LcdDispBuff);
		LcdDisplay(BOTTOM,LcdDispBuff+16);
		
		KeySense();
		if(UpKey)
		{	
			DiagnoseFlag = 1;
			break;
		}
		else if(MenuKey)
		{	
			DiagnoseFlag = 0;
			break;
		}
//		if(TimeOutFlag.TimeOut)
//			break;
	}
}

void Volt_Calc(char Disp_Flag)
{
	int CurADC = 0,voltage = 0;
	char DeciBuff[10]={0};    
    if(Disp_Flag == 5)
    {
        if(Application_Setting.LCED[CH1] == 1)
        {	
            Delay_ms(10);
            memcpy(LcdDispBuff+16,"1:",2);
            Ch1Volt = Volt_calculation(1);
            sprintf(DeciBuff,"%04ld",Ch1Volt);
            PutDp((unsigned char *)DeciBuff,(long)3,4,1);	
            memcpy(LcdDispBuff+18,DeciBuff,5);	
        }
        if(Application_Setting.LCED[CH2] == 1)
        {
            Delay_ms(10);
            memcpy(LcdDispBuff+25,"2:",2);
            Ch2Volt = Volt_calculation(2);
            sprintf(DeciBuff,"%04ld",Ch2Volt);
            PutDp((unsigned char *)DeciBuff,(long)3,4,1);
            memcpy(LcdDispBuff+27,DeciBuff,5);
        }
    }
    else if(Disp_Flag == 6)
    {
        if(Application_Setting.LCED[CH3] == 1)
        {
            Delay_ms(10);
            memcpy(LcdDispBuff+16,"3:",2);
            Ch3Volt = Volt_calculation(3);
            sprintf(DeciBuff,"%04ld",Ch3Volt);
            PutDp((unsigned char *)DeciBuff,(long)3,4,1);
            memcpy(LcdDispBuff+18,DeciBuff,5);
        }
        if(Application_Setting.LCED[CH4] == 1)
        {
            Delay_ms(10);
            memcpy(LcdDispBuff+25,"4:",2);            
            Ch4Volt = Volt_calculation(4);
            sprintf(DeciBuff,"%04ld",Ch4Volt);
            PutDp((unsigned char *)DeciBuff,(long)3,4,1);
            memcpy(LcdDispBuff+27,DeciBuff,5);
        }
    }
    else if(Disp_Flag == 7)
    {    
        if(Application_Setting.LCED[CH5] == 1)
        {
            Delay_ms(10);
            memcpy(LcdDispBuff+16,"5:",2);            
            Ch5Volt = Volt_calculation(5);
            sprintf(DeciBuff,"%04ld",Ch5Volt);
            PutDp((unsigned char *)DeciBuff,(long)3,4,1);
            memcpy(LcdDispBuff+18,DeciBuff,5);
        }    
        if(Application_Setting.LCED[CH6] == 1)
        {
            Delay_ms(10);
            memcpy(LcdDispBuff+25,"6:",2);            
            Ch6Volt = Volt_calculation(6);
            sprintf(DeciBuff,"%04ld",Ch6Volt);
            PutDp((unsigned char *)DeciBuff,(long)3,4,1);
            memcpy(LcdDispBuff+27,DeciBuff,5);
        }    
    }
}

void Show_Volt()
{
//    unsigned char UPKey_Flag = 1;
    LcdDisplay(TOP,"  Channel Volt  ");
	memset(LcdDispBuff,' ',32);
    while(1)
	{
        Volt_Calc(DiagnoseFlag);
		LcdDisplay(BOTTOM,LcdDispBuff+16);
		
		KeySense();
		if(UpKey)
		{
            DiagnoseFlag++;
            if(DiagnoseFlag >= 8)
                break;
		}
		else if(MenuKey)
		{
			memset(LcdDispBuff+16,' ',16);
            DiagnoseFlag = 0;
			break;
		}
	}
}

void LastCalibRecord(void)
{
    unsigned char DispFlag = 0;
    while(1)
    {
        if(DispFlag == 0)
        {
            memcpy(LcdDispBuff+16,"DATE :          ",16);
            sprintf(LcdDispBuff+23,"%02u/%02u/%02d",LastCalib.Date,LastCalib.Month,(LastCalib.Year%2000));            
        }
        else
        {
            memcpy(LcdDispBuff+16,"TIME :          ",16);
            sprintf(LcdDispBuff+23,"%02u:%02u:%02u",LastCalib.Hour,LastCalib.Minute,LastCalib.Second);
        }
        LcdDisplay(BOTTOM,LcdDispBuff+16);
        KeySense();
        if(MenuKey)
        {
            memset(LcdDispBuff+16,' ',16);
            break;
        }
        else if(UpKey)
        {
            if(DispFlag++ > 1)
            {
                DispFlag = 0;
            }
        }
    }
}
void LastCalibStore(void)
{
	LastCalib.Date = RTC.Date;
	LastCalib.Month = RTC.Month;
	LastCalib.Year = RTC.Year;
	LastCalib.Hour = RTC.Hour;
	LastCalib.Minute = RTC.Minute;
	LastCalib.Second = RTC.Second;
    
    Write_E2(E_LastCalib,(char*)&LastCalib.Date,sizeof(LastCalib));	
}

void LasttareRecord(void)
{
    unsigned char DispFlag = 0;
    while(1)
    {
        if(DispFlag == 0)
        {
            memcpy(LcdDispBuff+16,"DATE :          ",16);
            sprintf(LcdDispBuff+23,"%02u/%02u/%02d",LastTare.Date,LastTare.Month,(LastTare.Year%2000));            
        }
        else
        {
            memcpy(LcdDispBuff+16,"TIME :          ",16);
            sprintf(LcdDispBuff+23,"%02u:%02u:%02u",LastTare.Hour,LastTare.Minute,LastTare.Second);
        }
        LcdDisplay(BOTTOM,LcdDispBuff+16);
        KeySense();
        if(MenuKey)
        {
            memset(LcdDispBuff+16,' ',16);
            break;
        }
        else if(UpKey)
        {
            if(DispFlag++ > 1)
            {
                DispFlag = 0;
            }
        }
    }    
}
void LasttareStore(void)
{
	LastTare.Date = RTC.Date;
	LastTare.Month = RTC.Month;
	LastTare.Year = RTC.Year;
	LastTare.Hour = RTC.Hour;
	LastTare.Minute = RTC.Minute;
	LastTare.Second = RTC.Second;        
    
    Write_E2(E_LastTare,(char*)&LastTare.Date,sizeof(LastTare));	    
}

void ORDisplay()
{
	unsigned char Ch[NO_OF_CH] = {0};

	memcpy(LcdDispBuff,"  CHANNEL ERROR ",16);

	if(Bits.ORERRFlag1 || Bits.URERRFlag1 )//|| Bits.OCERRFlag1)
		Ch[CH1] = '1';
	if(Bits.ORERRFlag2 || Bits.URERRFlag2 )//|| Bits.OCERRFlag2)
		Ch[CH2] = '2';
	if(Bits.ORERRFlag3 || Bits.URERRFlag3 )//|| Bits.OCERRFlag3)
		Ch[CH3] = '3';
	if(Bits.ORERRFlag4 || Bits.URERRFlag4 )//|| Bits.OCERRFlag4)
		Ch[CH4] = '4';
	if(Bits.ORERRFlag5 || Bits.URERRFlag5 )//|| Bits.OCERRFlag4)
		Ch[CH5] = '5';
	if(Bits.ORERRFlag6 || Bits.URERRFlag6 )//|| Bits.OCERRFlag4)
		Ch[CH6] = '6';
    
	sprintf(LcdDispBuff+16,"%c  %c  %c  %c  %c  %c",Ch[CH1],Ch[CH2],Ch[CH3],Ch[CH4],Ch[CH5],Ch[CH6]);
	Bits.ORERRFlag1 = Bits.ORERRFlag2 = Bits.ORERRFlag3 = Bits.ORERRFlag4 = Bits.ORERRFlag5 = Bits.ORERRFlag6 = 0;
	Bits.URERRFlag1 = Bits.URERRFlag2 = Bits.URERRFlag3 = Bits.URERRFlag4 = Bits.URERRFlag5 = Bits.URERRFlag6 = 0;
}

void WeightDisplay(char Weight_or_Count)
{
//    Weight_Calc();
    memset(LcdDispBuff+16,' ',16);
    if(Weight_or_Count == 2)
    {
        if(Application_Setting.LCED[CH1] == 1)
        {
            memcpy(LcdDispBuff+16,"X:",2);        
            if(Bits.ORERRFlag1 == 1)
                memcpy(LcdDispBuff+18,"OR   ",5);
            else if(Bits.URERRFlag1 == 1)
                memcpy(LcdDispBuff+18,"UR   ",5);
            else
            {
                sprintf(LcdDispBuff+18,"%4ld",(long)Loadcell[CH1].CurWeight);
            }
        }
        if(Application_Setting.LCED[CH2] == 1)
        {
            memcpy(LcdDispBuff+25,"Y:",2);
            if(Bits.ORERRFlag2 == 1)
                memcpy(LcdDispBuff+27,"OR   ",5);
            else if(Bits.URERRFlag2 == 1)
                memcpy(LcdDispBuff+27,"UR   ",5);
            else
            {
                sprintf(LcdDispBuff+27,"%4ld",(long)Loadcell[CH2].CurWeight);
            }
        }
    }
    else if(Weight_or_Count == 3)
    {
        if(Application_Setting.LCED[CH3] == 1)
        {
            memcpy(LcdDispBuff+16,"3:",2);        
            if(Bits.ORERRFlag3 == 1)
                memcpy(LcdDispBuff+18,"OR  ",4);
            else if(Bits.URERRFlag3 == 1)
                memcpy(LcdDispBuff+18,"UR  ",4);
            else
            {
                sprintf(LcdDispBuff+18,"%04ld",(long)Loadcell[CH3].CurWeight);
                PutDp((unsigned char*)LcdDispBuff+18,DecimalPoint,4,1);
            }
        }
        if(Application_Setting.LCED[CH4] == 1)
        {
            memcpy(LcdDispBuff+25,"4:",2); 
            if(Bits.ORERRFlag4 == 1)
                memcpy(LcdDispBuff+27,"OR   ",4);
            else if(Bits.URERRFlag4 == 1)
                memcpy(LcdDispBuff+27,"UR   ",4);
            else
            {
                sprintf(LcdDispBuff+27,"%04ld",(long)Loadcell[CH4].CurWeight);
                PutDp((unsigned char*)LcdDispBuff+27,DecimalPoint,4,1);
            }
        }
    }
    else if(Weight_or_Count == 4)
    {
        if(Application_Setting.LCED[CH5] == 1)
        {
            memcpy(LcdDispBuff+16,"5:",2); 
            if(Bits.ORERRFlag5 == 1)
                memcpy(LcdDispBuff+18,"OR   ",5);
            else if(Bits.URERRFlag5 == 1)
                memcpy(LcdDispBuff+18,"UR   ",5);
            else
            {
                sprintf(LcdDispBuff+18,"%04ld ",(long)Loadcell[CH5].CurWeight);
                PutDp((unsigned char*)LcdDispBuff+18,1,4,1);
            }
        }    
        if(Application_Setting.LCED[CH6] == 1)
        {
            memcpy(LcdDispBuff+25,"6:",2); 
            if(Bits.ORERRFlag6 == 1)
                memcpy(LcdDispBuff+27,"OR   ",5);
            else if(Bits.URERRFlag6 == 1)
                memcpy(LcdDispBuff+27,"UR   ",5);
            else
            {
                sprintf(LcdDispBuff+27,"%04ld ",(long)Loadcell[CH6].CurWeight);
                PutDp((unsigned char*)LcdDispBuff+27,1,4,1);
            }
        }
    }
}




//void Show_LC_Values(void)
//{
//    memset(LcdDispBuff,' ',32);    
//	while(1)
//	{
//        WeightDisplay(Weight_Display);
//		LcdDisplay(TOP,LcdDispBuff);
//		LcdDisplay(BOTTOM,LcdDispBuff+16);		
//		KeySense();
//		if(UpKey)
//		{	
//			DiagnoseFlag = 4;
//			break;
//		}
//		else if(MenuKey)
//		{	
//			DiagnoseFlag = 0;
//			break;
//		}
////		if(TimeOutFlag.TimeOut)
////			break;
//	}
//}

/*
long RAMData_Weight_Calc(int RAMADC,char loadcell)
{
	float WeightWithDecimal = 0;
    char i=0;
    long loc_weight = 0;
	RAM_EnggWt=0;

    WeightWithDecimal = 0.0;
    WeightWithDecimal = (float)((float)((float)Loadcell[loadcell].CalCapacity) / (float)labs(Loadcell[loadcell].Span - Loadcell[loadcell].Zero1));//(float)(Setup.Channel1+Setup.Channel2+Setup.Channel3+Setup.Channel4)		//NO_OF_LOADCELL
    WeightWithDecimal = (float)((float)(WeightWithDecimal)*(float)labs(RAMADC - Loadcell[loadcell].Zero));				

    loc_weight = (long)WeightWithDecimal * 100;
    loc_weight = RoundOff(loc_weight,100);
    loc_weight = (loc_weight/100);
    loc_weight = RoundOff(loc_weight,resdata[Resolution-1]);

//    if(RAMADC < Loadcell[loadcell].Zero)
//    {
//        loc_weight = (loc_weight * (-1));
//    }           
    if((signed int)(Loadcell[i].Span - Loadcell[i].Zero1) < 0)
    {
        if(RAMADC > Loadcell[i].Zero)   
            Loadcell[i].CurWeight = loc_weight * (-1);
        else
            Loadcell[i].CurWeight = loc_weight;                
    }
    else if((signed int)(Loadcell[i].Span - Loadcell[i].Zero1) > 0)
    {
        if(RAMADC < Loadcell[i].Zero)   
            Loadcell[i].CurWeight = loc_weight * (-1);
        else
            Loadcell[i].CurWeight = loc_weight;                  
    }    
    return(loc_weight);
}
*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Function Title			: Get Stable weight.
	Function Argument		: Nil
	Function Return Value	: Nil
	Function Description	: This routine checks the current weight for stability. And updates
	a global variable. Stb_Wt and Stb_Flag.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void GetStableWt()
{
/*	if(labs(AllChannel.CurWeight - ChkStableWt ) <= Application_Setting.StableToll)
	{
		if(++StableWtDelayCnt > Setup.StableCntr)
		{
			StableWtFlg=TRUE;
			StableWtDelayCnt = Setup.StableCntr + 1;
		}
		else
		{*/
		/*	if(IndexCntr <MAX_BUFFER)
				StableWt[IndexCntr]=AllChannel.CurWeight;   	//Added by sathish

			IndexCntr++;
			if(IndexCntr >=MAX_BUFFER)
			{
				IndexCntr = 0;
				WtBuffOverflowFlg = 1;
			}*/
/*			StableWtFlg=FALSE;
		}
//		sprintf(LcdDispBuff,"B:%3ld A:%3ld l%2ld",labs(Calibration.EnggWt - ChkStableWt ),StableWtDelayCnt,Setup.ToleranceBand);
//		LcdDisplay(TOP,LcdDispBuff);
	}
	else
	{
		WtBuffOverflowFlg = 0;
		IndexCntr = 0;
		StableWtDelayCnt=0;
		StableWtFlg=FALSE;
		ChkStableWt = AllChannel.CurWeight;
	}
	return;*/
}

/****************************************************************
Function name 	: Do_Stble_CaptureWeight
Returns 		: None
Arguments		: None
Description 	: Used to check the stable weight,
				 & measure the weights
****************************************************************/
void Do_Stble_CaptureWeight()
{
	unsigned char Cond_Check = 0;
//	unsigned long LoopCntr = 0;
	unsigned char Max_Cntr = 0;
    unsigned char Prot_Buff[15]={0};
//	float WtAvg = 0;
	long loc_weight = 0;
    WtBuffOverflowFlg = 0;
	ChkStableWt = 0;
	IndexCntr = 0;
	StableWtDelayCnt=0;
	StableWtFlg=FALSE;

	while(1)
	{
        if(Application_Setting.ComMode == COM_GPRS_MODULE)
            Record_Management();
        else if((Application_Setting.ComMode == COM_DUMP) && (RS232_Dump_Time_Flag))
        {
            RS232_Dump_Time_Flag = 0;
            PC_Dump();
        }
        Weight_Calc();
		MainScreenDisplay();
        Keyfunction();
        CANDataFlow();
        RangeOutput();        
        if(IPARKING != Application_Setting.Input1Logic)  //Dump break released
        {
//            loc_weight = 0;
//            if(CounterOverflowFlg == 1)
//                Max_Cntr = MAX_BUFFER;
//            else
//                Max_Cntr = Wtstored;
//            for(Cond_Check=0;Cond_Check < Max_Cntr;Cond_Check++)
//            {
//                loc_weight = loc_weight + LatchedWts[Cond_Check];
//            }
//            loc_weight = loc_weight / Max_Cntr;
            if(AllChannel.CurWeight >=  Application_Setting.LatchedWt)	
            {
                Bits.InputCheck = 1;
                if((Bits.ILATCH_val) && (IPARKING != Application_Setting.Input1Logic))
                {
//                    Print_CRLF(1);
//                    sprintf(Prot_Buff,"%ld",loc_weight);
//                    Com1StringSend(Prot_Buff,sizeof(Prot_Buff));  
                    StoredWeight = AllChannel.CurWeight;// + (Application_Setting.BucketWt/2));
                    Status_Write(HAULING);//Application_Setting.PLMS_Status = HAULING;
//                    Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);                    
                    Bits.LatchedWt = 1;                
                    Bits.InputCheck = 0;Bits.ILATCH_val=0;//TimeOut_Cnt1 =0; 
                    WtBuffOverflowFlg = 0;
                    IndexCntr = 0;
                    StableWtDelayCnt=0;
                    StableWtFlg=FALSE;
                    ChkStableWt = 0;
                    Wt_Array_Reset();
                    break;
                }
            }
            else
            {
                Bits.InputCheck = 0;Bits.ILATCH_val=0;//TimeOut_Cnt1 =0; 
                WtBuffOverflowFlg = 0;
                IndexCntr = 0;
                StableWtDelayCnt=0;
                StableWtFlg=FALSE;
                ChkStableWt = 0;
                Wt_Array_Reset();
                break;                    
            }
        }
        else    //Dump break given
        {
            Bits.InputCheck = 0;Bits.ILATCH_val=0; //TimeOut_Cnt1 =0;         
//            LatchedWts[Wtstored] = AllChannel.CurWeight;
//            Wtstored++;
//            if(Wtstored >=MAX_BUFFER)
//            {
//                Wtstored = 0;
//              WtBuffOverflowFlg_2 = 1;
//            }                    
        }
	}		//End of while loop
}

void Wt_Array_Reset(void)
{
	unsigned char Cond_Check = 0;    
    Wtstored = 0;
    CounterOverflowFlg = 0;
    for(Cond_Check=0;Cond_Check < MAX_BUFFER;Cond_Check++)
        LatchedWts[Cond_Check] = 0;     
}

void Do_CaptureWeight()
{
/*	char WtAvg_Flag = 0;
	unsigned long CapTime = 0;
	long Wt_Avg = 0;
	long Loop_Var = 0;
	unsigned long OverCapPercentValue = 0;
	CapTime = (unsigned long)((Setup.CaptureTime * 62500)/10); //  1 second = 62500 counts
	Cap_Cursor = 0;
	Wt_Avg_Cntr = 0;
	CaptureTimeCounter = 0;
	//Bits.Cap_Time_Start = 1;
//    sprintf(LcdDispBuff+16,"6:%d:%d:%d:%lu:%lu",ILATCH,Setup.InputLogic,Bits.InputCheck,CapTime,CaptureTimeCounter);          
//    LcdDisplay(BOTTOM," Inside Cap Fun ");
//    Delay_1sec(10);
//	OverCapPercentValue =  (MaxCapacity + (unsigned long)((MaxCapacity*OverCapPercent)/100));
//	ReadRTC_DateTime();    
//	sprintf(PrintBuff,"3:%02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);    
//    Com1StringSend(PrintBuff,10);
	while(1)	//Before storing need to identify OC, OR values(88888,99999) and number of counter also. After storing into if OC, OR counter is greater then totalcount/2 then that trip is error.
	{
//        sprintf(LcdDispBuff+16,"%d:%lu:%lu",Bits.InputCheck,CapTime,CaptureTimeCounter,TimeOut_Cnt1);
//                        LcdDisplay(BOTTOM,LcdDispBuff+16);  
		CalcADC();
		if(Bits.Cap_Time_Start) 
		{
			if((Cap_Cursor < MAX_CAP_CNT) )  //MAX_CAP_CNT=150
			{
//				Cap_Count_1[Cap_Cursor] = Loadcell[CH1].RawADC;//Calibration.RAWADC;
//                Cap_Count_2[Cap_Cursor] = Loadcell[CH2].RawADC;
//                Cap_Count_3[Cap_Cursor] = Loadcell[CH3].RawADC;
//                Cap_Count_4[Cap_Cursor] = Loadcell[CH4].RawADC;
				Cap_Cursor++;	
			}
			if(CaptureTimeCounter > CapTime)
			{
//                    ReadRTC_DateTime();    
//                    sprintf(PrintBuff,"4:%02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);    
//                    Com1StringSend(PrintBuff,10);                                    
//                sprintf(LcdDispBuff+16,"65:%d:%d:%d:%lu",ILATCH,Setup.InputLogic,Bits.InputCheck,CaptureTimeCounter);          
//                LcdDisplay(BOTTOM,LcdDispBuff+16);                        
				Bits.Cap_Time_Start = 0;
				CaptureTimeCounter = 0;
				if( (Cap_Cursor >= 1) )
				{
					for(Loop_Var=0;Loop_Var<Cap_Cursor;Loop_Var++)	//Calculating Stored RAM count as EngWt
					{
//						Weight_Calc(Axle1,(Cap_Count_1[Loop_Var]),(Cap_Count_2[Loop_Var]),0);
////						Weight_Calc((Cap_Count_2[Loop_Var]),CH2,0);
//						Weight_Calc(Axle2,(Cap_Count_3[Loop_Var]),(Cap_Count_4[Loop_Var]),0);
//						Weight_Calc((Cap_Count_4[Loop_Var]),CH4,0);
                        Weight_Calc();
                        RAM_EnggWt = 0;
                        if((Setup.LCED[CH1] == 1) || (Setup.LCED[CH2] == 1))
                        {
                            if(Axle[Axle1].CurWeight <= (MaxCapacity+(9 * resdata[Resolution-1])))//if load cell in OC condition dont add the weight
                                RAM_EnggWt += Axle[Axle1].CurWeight;
                        }
                        if((Setup.LCED[CH3] == 1) || (Setup.LCED[CH4] == 1))
                        {
                            if(Axle[Axle2].CurWeight <= (MaxCapacity+(9 * resdata[Resolution-1])))//if load cell in OC condition dont add the weight
                                RAM_EnggWt += Axle[Axle2].CurWeight;
                        }

//                        Cap_Wt[Loop_Var] = RAM_EnggWt;	//For Data Dump purpose
				//		if( RAM_EnggWt < OverCapPercentValue )
						{
//							Wt_Avg = Wt_Avg + Cap_Wt[Loop_Var];
							Wt_Avg_Cntr++;
						}
					}
//                    ReadRTC_DateTime();    
//                    sprintf(PrintBuff,"5:%02u:%02u02u",RTC.Hour,RTC.Minute,RTC.Second);    
//                    Com1StringSend(PrintBuff,10);                    
					if(Wt_Avg_Cntr != 0)
					{
						StoredWeight = (long)(10 * (float)(Wt_Avg/(Wt_Avg_Cntr)));
					}
					else
					{
						Wt_Avg = 0;
						for(Loop_Var=0;Loop_Var<Cap_Cursor;Loop_Var++)
						{
//							Wt_Avg = Wt_Avg + Cap_Wt[Loop_Var];
						}
						StoredWeight = (long)(10 * (float)(Wt_Avg/(Cap_Cursor)));
					}
					StoredWeight = RoundOff(StoredWeight,10);
					StoredWeight = StoredWeight/10;
				}
                Bits.Cap_Time_Over = 1;
			}
			if(ILATCH != Setup.InputLogic)
			{
				break;
			}

		}
		else
			break;
	}*/
}

void Status_Write(unsigned char Status)
{
    Application_Setting.PLMS_Status = Status;            
    Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	                
}

void PayloadProcess_ParkingBreak(void)  
{
    long TemporaryValue_1 = 0;
    long TemporaryValue_2 = 0;
    char CheckingWt = 0;
    float WtCalculation = 0;
	if(IPARKING == Application_Setting.Input1Logic) 		// Dump Break Given 
	{
        if(Application_Setting.PLMS_Status == LOAD)	
        {
            Bits.InputCheck = 1; 
            Average_Count = Average_Wt = 0;Previous_Average_Wt = 1;
            Bits.Disp_Latch_Wt = 0;  
            BodyUpFlag = 0;
        }
        if((Application_Setting.PLMS_Status == RETURN) && (changeLoadFlag == 1))
        {
            Bits.InputCheck = 0;
            Bits.ILATCH_val = 0;
            Status_Write(LOAD);//Application_Setting.PLMS_Status = LOAD;
            Bits.Disp_Latch_Wt = 0;        
        }
        if((Application_Setting.PLMS_Status == HAULING) && (Bits.Disp_Latch_Wt == 1) && (AllChannel.CurWeight > Application_Setting.MinWt))
        {
            Status_Write(DUMP);
            MinWt_Check_Counter = 0;            
        }
        if(Application_Setting.PLMS_Status == DUMP)
        {
            if((IBODYUP != Application_Setting.Input2Logic) && (Application_Setting.AngleEnDis == Disable) && (AllChannel.CurWeight > Application_Setting.LatchedWt))
            {
                WtCalculation = (float)(Average_Wt * Average_Count);
                WtCalculation = (float)(WtCalculation + (float)AllChannel.CurWeight);
                Average_Wt = (float)(WtCalculation/(Average_Count + 1));
                Average_Count++;                
            }
            if(AllChannel.CurWeight < Application_Setting.MinWt)		
            {
    //          Status_Write(RETURN);//Application_Setting.PLMS_Status = RETURN;
                Bits.ILATCH_val = 0;
                Bits.InputCheck = 0;
                changeLoadFlag = 0;
                if((IBODYUP != Application_Setting.Input2Logic) && (Application_Setting.AngleEnDis == Disable))
                {
                    StoredWeight = AllChannel.LatchedWt;
                    StoreRecord(1); 
                    Average_Count = Average_Wt = 0;
                }
                else if(Average_Count  != 0)
                {
                    if(Average_Wt >= Application_Setting.LatchedWt)
                        StoredWeight = Average_Wt;
                    else
                        StoredWeight = AllChannel.LatchedWt;
                    StoreRecord(1); 
                    Average_Count = Average_Wt = 0;
                }
                BodyUpFlag = 1;
//              Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	
            }
            else if(IBODYUP == Application_Setting.Input2Logic)// && (Application_Setting.AngleEnDis == Disable))
            {
                Bits.ILATCH_val = 0;
                Bits.InputCheck = 0;
                changeLoadFlag = 0;
                BodyUpFlag = 1;
                if(Average_Count  != 0)
                {
                    if(Average_Wt >= Application_Setting.LatchedWt)
                        AllChannel.LatchedWt = StoredWeight = Average_Wt; 
                    else
                        StoredWeight = AllChannel.LatchedWt;                    
                    AllChannel.LatchedWt = RoundOff(AllChannel.LatchedWt,resdata[Resolution-1]);
                }
            }
        }
    }
    if((IPARKING != Application_Setting.Input1Logic) && (Application_Setting.PLMS_Status == DUMP) && (AllChannel.CurWeight < Application_Setting.MinWt) && (BodyUpFlag == 1))
    {
        Status_Write(RETURN);
        changeLoadFlag = 1;
//      AllChannel.LatchedWt = 0; 
        Bits.ILATCH_val = 0;
        Bits.InputCheck = 0;
        if(Average_Count  != 0)
        {
            if(Average_Wt >= Application_Setting.LatchedWt)
                StoredWeight = Average_Wt;
            else
                StoredWeight = AllChannel.LatchedWt;
            StoreRecord(1); 
            Average_Count = Average_Wt = 0;Previous_Average_Wt = 1;
        }
 //       Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	
        Bits.Disp_Latch_Wt = 0;      
    }
    if(IPARKING != Application_Setting.Input1Logic) 
    {
        if(Application_Setting.PLMS_Status == HAULING)
        {
            if(DecimalPoint == 0)
                CheckingWt = 2;
            else
                CheckingWt = 20;
            if(((Bits.Disp_Latch_Wt == 1) && (AllChannel.LatchedWt <= AllChannel.CurWeight)) && (Application_Setting.AngleEnDis == Enable))
            {
                TemporaryValue_1 = (long)Loadcell[CH1].CurWeight;
                TemporaryValue_2 = (long)Loadcell[CH2].CurWeight;
                if(((labs(TemporaryValue_1)) <= Application_Setting.AngleConfig) && ((labs(TemporaryValue_2)) <= Application_Setting.AngleConfig))
                {
                    WtCalculation = (float)(Average_Wt * Average_Count);
                    WtCalculation = (float)(WtCalculation + (float)AllChannel.CurWeight);
                    Average_Wt = (float)(WtCalculation/(Average_Count + 1));
                    Average_Count++;
                }
                else
                   Onetime_average_Counter = 0;
            }
        }
        else if(Application_Setting.PLMS_Status == DUMP)
        {
            if((Bits.Disp_Latch_Wt == 1) && (AllChannel.CurWeight > Application_Setting.MinWt) && (BodyUpFlag == 0))
            {
                Status_Write(HAULING);//Application_Setting.PLMS_Status = HAULING;            
                Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	            
                MinWt_Check_Counter = 0;                        
            }
        }
        else if((Application_Setting.PLMS_Status == DUMP) && (AllChannel.CurWeight < Application_Setting.MinWt))
        {
            changeLoadFlag = 1;
            Status_Write(RETURN);
            AllChannel.LatchedWt = 0;       
            Bits.Disp_Latch_Wt = 0;               
        }
    }
	if(Application_Setting.PLMS_Status == LOAD)
	{
		if((Bits.ILATCH_val) && (IPARKING == Application_Setting.Input1Logic))
		{
            Bits.ILATCH_val = 0;
            Bits.InputCheck = 0;  
            Weight_Calc();
            Do_Stble_CaptureWeight();
			if(Bits.LatchedWt == 1)
			{
				Bits.LatchedWt = 0;
				StoreRecord(0);
                //BotDisp = 8;
			}
		}
	}
//	}    
/*	if(IPARKING == Application_Setting.InputLogic) 		// Dump Break Given 
	{
        if(Application_Setting.PLMS_Status == LOAD)	
        {
            Bits.InputCheck = 1;        
        }
        if((Application_Setting.PLMS_Status == RETURN) && (changeLoadFlag == 1))
        {
            Bits.InputCheck = 0;
            Bits.ILATCH_val = 0;
            Application_Setting.PLMS_Status = LOAD;
            Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	            
            Bits.Disp_Latch_Wt = 0;        
        }   
        if((Application_Setting.PLMS_Status == HAULING) && (Bits.Disp_Latch_Wt == 1) && (AllChannel.CurWeight > Application_Setting.MinWt))
        {
            Application_Setting.PLMS_Status = DUMP;            
            Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	            
            MinWt_Check_Counter = 0;            
        }
        if(Application_Setting.PLMS_Status == DUMP)
        {
            if(AllChannel.CurWeight < Application_Setting.MinWt)		
            {
                MinWt_Check_Counter++;
                if(MinWt_Check_Counter > 15)
                {
                    Application_Setting.PLMS_Status = RETURN;
                    AllChannel.LatchedWt = 0; 
                    Bits.ILATCH_val = 0;
                    Bits.InputCheck = 0;
                    Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	
                    Bits.Disp_Latch_Wt = 0;   
                }
            }
            else
                MinWt_Check_Counter = 0;
        }
    }
    if(IPARKING != Application_Setting.InputLogic)
    {
        if(Application_Setting.PLMS_Status == DUMP)
        {
            if(AllChannel.CurWeight < Application_Setting.MinWt)
            {
                MinWt_Check_Counter++;
                if(MinWt_Check_Counter > 15)
                {
                    Application_Setting.PLMS_Status = RETURN;
                    AllChannel.LatchedWt = 0; 
                    Bits.ILATCH_val = 0;
                    Bits.InputCheck = 0;
                    changeLoadFlag = 1;
                    Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);	
                    Bits.Disp_Latch_Wt = 0;   
                    MinWt_Check_Counter = 0;
                }
            }
            else
            {
                MinWt_Check_Counter = 0;
            }
        }
        else if(Application_Setting.PLMS_Status == RETURN)
            changeLoadFlag = 1;
    }
	if(Application_Setting.PLMS_Status == LOAD)
	{
		if((Bits.ILATCH_val) && (IPARKING == Application_Setting.InputLogic))
		{
            Bits.ILATCH_val = 0;
            Bits.InputCheck = 0;  
            Weight_Calc();
            Do_Stble_CaptureWeight();
			if(Bits.LatchedWt == 1)
			{
				Bits.LatchedWt = 0;
                Bits.Disp_Latch_Wt = 0;     
                changeLoadFlag = 0;
				StoreRecord(1); 
				Application_Setting.PLMS_Status = HAULING;
				Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay(2);
			}
		}
    }*/
}

void PayloadProcess_BodyUp()
{
    static int loopi = 0;
    long TemporaryValue_1 = 0;
    long TemporaryValue_2 = 0;
    int CheckingCounter = 0;
    float WtCalculation = 0;
    
	if(IBODYUP == Application_Setting.Input2Logic) 		// Body up signal Given 
	{
        if(MinWt_Check_Counter == 0)
        {
            Bits.InputCheck = 1;
            if(Bits.ILATCH_val)
            {
                Bits.ILATCH_val = 0;
                Bits.InputCheck = 0;            
                if((Average_Count != 0) || (CounterOverflowFlg == 1))
                {
                    Average_Wt = 0;
                    if(CounterOverflowFlg == 1)
                        CheckingCounter = Application_Setting.No_of_Samples;
                    else
                        CheckingCounter = Average_Count;
                    for(loopi = 0;loopi < CheckingCounter;loopi++)
                    {
                        Average_Wt = Average_Wt + No_of_Samplesarray[loopi];
                    }
                    Average_Wt = Average_Wt / CheckingCounter;
                }
                if(Average_Wt >= Application_Setting.LatchedWt)
                {
                    Status_Write(RETURN);
                    Bits.ILATCH_val = 0;
                    Bits.InputCheck = 0;
                    StoredWeight = Average_Wt;
                    AllChannel.LatchedWt = StoredWeight; 
                    StoreRecord(1); 
                    MinWt_Check_Counter = 1;
                    Write_E2(EMinWt_Check_Counter,(char*)&MinWt_Check_Counter,sizeof(MinWt_Check_Counter));
                    Average_Count = Average_Wt = CounterOverflowFlg = 0;
                    Write_E2(ECounterOverflowFlg,(char*)&CounterOverflowFlg,sizeof(CounterOverflowFlg));
                    Write_E2(EAverage_Count,(char*)&Average_Count,sizeof(Average_Count));
                }
            }
        }
    }
    else if(IBODYUP != Application_Setting.Input2Logic) //Moving Condition
    {
        Bits.ILATCH_val = 0;
        Bits.InputCheck = 0;
        if(Application_Setting.AngleEnDis == Enable)
        {
            if((Application_Setting.LatchedWt <= AllChannel.CurWeight) && (MinWt_Check_Counter == 0))
            {
                TemporaryValue_1 = (long)Loadcell[CH1].CurWeight;
                TemporaryValue_2 = (long)Loadcell[CH2].CurWeight;
                if(((labs(TemporaryValue_1)) <= Application_Setting.AngleConfig) && ((labs(TemporaryValue_2)) <= Application_Setting.AngleConfig))
                {
                    No_of_Samplesarray[Average_Count] = (int) AllChannel.CurWeight;
                    Write_E2((ENo_of_SamplesArray+(Average_Count * 2)),(char*)&No_of_Samplesarray[Average_Count],2);
                    Average_Count++;
                    if(Average_Count > Application_Setting.No_of_Samples)
                    {
                        Average_Count = 0;
                        CounterOverflowFlg = 1;
                        Write_E2(ECounterOverflowFlg,(char*)&CounterOverflowFlg,sizeof(CounterOverflowFlg));
                    }
                    Write_E2(EAverage_Count,(char*)&Average_Count,sizeof(Average_Count));
                }
            }
        }
        else
        {
            if((Application_Setting.LatchedWt <= AllChannel.CurWeight) && (MinWt_Check_Counter == 0))
            {
                No_of_Samplesarray[Average_Count] = (int) AllChannel.CurWeight;
                Write_E2((ENo_of_SamplesArray+(Average_Count * 2)),(char*)&No_of_Samplesarray[Average_Count],2);
                Average_Count++;
                if(Average_Count > Application_Setting.No_of_Samples)
                {
                    Average_Count = 0;
                    CounterOverflowFlg = 1;
                    Write_E2(ECounterOverflowFlg,(char*)&CounterOverflowFlg,sizeof(CounterOverflowFlg));
                }
                Write_E2(EAverage_Count,(char*)&Average_Count,sizeof(Average_Count));
            }
        }
        if((AllChannel.CurWeight < Application_Setting.MinWt) && (MinWt_Check_Counter == 1))
        {
            Status_Write(RETURN);
            MinWt_Check_Counter = 0;
            Bits.Disp_Latch_Wt = 0;
            Write_E2(EMinWt_Check_Counter,(char*)&MinWt_Check_Counter,sizeof(MinWt_Check_Counter));
        }
    }
}

void RecordStorage(void)
{
    static unsigned char PressCount = 0;
    static unsigned int TripCount = 0,TripFlag = 0,TripStore = 0;
	RecordStore.Date = RTC.Date;
	RecordStore.Month = RTC.Month;
	RecordStore.Year = RTC.Year;
	RecordStore.Hour = RTC.Hour;
	RecordStore.Minute = RTC.Minute;
	RecordStore.Second = (PressCount+1);
	RecordStore.TempFlag = 0;
	RecordStore.Weight = (100 + (PressCount+1));
    PressCount++;
    if(PressCount > 200)
        PressCount = 0;
    RecordStore.Polarity = ' ';
    FIFO_Rec_Storage(&TripCount,&TripFlag,&TripStore);
    sprintf(LcdDispBuff,"%d:%d:%d",TripCount,TripFlag,TripStore);
    LcdDisplay(TOP,LcdDispBuff);Delay1_1s(3);
}

void StoreRecord(char Send_data)
{
	unsigned long TempRecNo = 0;
	AllChannel.LatchedWt = StoredWeight; 
	AllChannel.LatchedWt = RoundOff(AllChannel.LatchedWt,resdata[Resolution-1]);
    if(Bits.Disp_Latch_Wt == 0)
    {
        if(AllChannel.TripCount >= (MAXRECORDS-50) && AllChannel.TripCount < MAXRECORDS)
        {
            LcdDisplay(TOP,(char *)" RECORD OVERFLOW");
            LcdDisplay(BOTTOM,(char *)"  TAKE REPORTS  ");
            Delay1_1s(2);
        }
        if(AllChannel.TripCount >= MAXRECORDS && Bits.RecOverWrite == 0) 
        {
            Bits.RecOverWrite = 1;
        }
        if(Bits.RecOverWrite == 1)
            TempRecNo = (AllChannel.TripCount % MAXRECORDS);
        else
            TempRecNo = AllChannel.TripCount;

        AllChannel.TripCount++;
        if(AllChannel.TripCount > MAX_TRIPCOUNT)
        {
            AllChannel.TripCount = 0;
            Bits.RecOverWrite = 0;
        }
        Write_E2(EE_TripCount,(char*)&AllChannel.TripCount,sizeof(AllChannel.TripCount));
        TripRecord.Weight = AllChannel.LatchedWt;
        if(TripRecord.Weight != 9999)	//v1.0.6
        {
            AllChannel.CumulativeWeight = AllChannel.CumulativeWeight + TripRecord.Weight;
            Write_E2(EE_CumulativeWeight,(char*)&AllChannel.CumulativeWeight,sizeof(AllChannel.CumulativeWeight));Delay_ms(2000);
        }
    }
    else
    {
        if(AllChannel.TripCount >= MAXRECORDS && Bits.RecOverWrite == 0) 
        {
            Bits.RecOverWrite = 1;
        }
        if(Bits.RecOverWrite == 1)
            TempRecNo = ((AllChannel.TripCount-1) % MAXRECORDS);
        else
            TempRecNo = AllChannel.TripCount-1;        
    }
	if(AllChannel.LatchedWt >= Application_Setting.OverWeight)
	{
		TripRecord.OLStatus = 1;
	}
	else
		TripRecord.OLStatus = 0;

	ReadRTC_DateTime();
	TripRecord.Date = RTC.Date;
	TripRecord.Month = RTC.Month;
	TripRecord.Year = RTC.Year;
	TripRecord.Hour = RTC.Hour;
	TripRecord.Minute = RTC.Minute;
	TripRecord.Second = RTC.Second;
	TripRecord.TempFlag = 0;
	TripRecord.Weight = AllChannel.LatchedWt;
    TripRecord.Polarity = ' ';
	Write_E2((unsigned long)(EEPROM_STORING_ADD+((TempRecNo) * (sizeof(TripRecord)))),(char*)&TripRecord.Date,(sizeof(TripRecord)));	
	Delay_ms(10);
    if(Send_data == 1)
    {
        Latched_Report_Assign();
        Flag.Latched_Send_Flag = 1;
    }
    Bits.Disp_Latch_Wt = 1;
//    Bits.SecondLatch = 1;
}

void Print_record(void)
{
    Print_CRLF(1);
	Com1StringSend((char *)"       FIXED BODY TRUCK WEIGHING        ",40);    
    Print_CRLF(1);
	ReadRTC_DateTime();
	sprintf(PrintBuff,"  Date: %02u/%02u/%4d",RTC.Date,RTC.Month,RTC.Year);
	memcpy(PrintBuff+18,"      ",6);
	sprintf(PrintBuff+24,"Time: %02u:%02u:%02u  ",RTC.Hour,RTC.Minute,RTC.Second);
	Com1StringSend(PrintBuff,40);
    Print_CRLF(1);    
	Com1StringSend((char *)"   SNO     DATE      TIME     T.Wt(T)   ",40);
    Print_CRLF(1);    
    
	if(!Bits.RecOverWrite)
	{    
        for(TripNo = 0;TripNo < AllChannel.TripCount;TripNo++)
        {
            Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
            sprintf(PrintBuff+1,"%4d",(TripNo+1));
            memcpy(PrintBuff+5,"   ",3);
            sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
            memcpy(PrintBuff+18,"   ",3);
            sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
            memcpy(PrintBuff+26,"  ",2);
            sprintf(PrintBuff+28,"%06ld",TripRecord.Weight);                          
            PutDp((unsigned char*)PrintBuff+28,DecimalPoint,6,1);                        
            if(TripRecord.OLStatus)
                memcpy(PrintBuff+35,"*    ",5);
            else
                memcpy(PrintBuff+35,"     ",5);                      
            Com1StringSend(PrintBuff,40);
            Print_CRLF(1);        
        }
    }
    else
    {
		for(TripNo = (AllChannel.TripCount % MAXRECORDS);TripNo < MAXRECORDS;TripNo++)
		{    
            Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
            sprintf(PrintBuff+1,"%4d",TripNo);
            memcpy(PrintBuff+5,"   ",3);
            sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
            memcpy(PrintBuff+18,"   ",3);
            sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
            memcpy(PrintBuff+26,"  ",2);
            sprintf(PrintBuff+28,"%06ld",TripRecord.Weight);                          
            PutDp((unsigned char*)PrintBuff+28,DecimalPoint,6,1);                        
            if(TripRecord.OLStatus)
                memcpy(PrintBuff+35,"*    ",5);
            else
                memcpy(PrintBuff+35,"     ",5);                      
            Com1StringSend(PrintBuff,40);
            Print_CRLF(1);                
        }
    }
}
/*
void Print_record(void)
{
    sprintf(PrintBuff+1,"%4d",++CumTrips);
    memcpy(PrintBuff+5,"   ",3);
    sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
    memcpy(PrintBuff+18,"   ",3);
    sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
    memcpy(PrintBuff+26,"  ",2);
    sprintf(PrintBuff+28,"%06u",TripRecord.Weight);                          
    PutDp((unsigned char*)PrintBuff+28,1,6,1);                        
    if(TripRecord.OLStatus)
        memcpy(PrintBuff+35,"*    ",5);
    else
        memcpy(PrintBuff+35,"     ",5);                      
    Com1StringSend(PrintBuff,40);
    Print_CRLF(1);        
}

void current_date_assign(void)
{
    CurDate += (TripRecord.Year-2000);
    CurDate *= 10000;
    CurDate += TripRecord.Date;	
    CurDate += (TripRecord.Month * 100);    
}

void record_read_validate(void)
{
#ifdef Execute    
    Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));
//			CurDate += (TripRecord.Year-2000);
//			CurDate *= 10000;
//			CurDate += TripRecord.Date;	
//			CurDate += (TripRecord.Month * 100);
    current_date_assign();	
    if(forsendingdata!=1)
    {
        if(CurDate >= From && CurDate <= End)
        {
            CumTrips += 1;	
            if(TripRecord.Weight != 9999)	//v1.0.6
            {
                Cum_Wt = Cum_Wt + TripRecord.Weight;
            }
            if(TripRecord.OLStatus)
            {	
                OLTrips = OLTrips + 1;
            }
        }
        CurDate = 0;
    }
    else
    {
        CumTrips += 1;	
        if(TripRecord.Weight != 9999)	//v1.0.6
        {
                Cum_Wt = Cum_Wt + TripRecord.Weight;
        }
        if(TripRecord.OLStatus)
        {	
            OLTrips =OLTrips + 1;
        }
        CurDate = 0;
    }    
#endif
}
void PrintReport()
{

#ifndef Execute_report
	int dummy;
	char StartDate[7] = {0},EndDate[7] = {0},TempDate[7]={0};
    CumTrips = OLTrips = TripNo = From = End = Cum_Wt = CurDate = 0;
	ReadRTC_DateTime();
	sprintf(StartDate,"%02u%02u%02u",RTC.Date,RTC.Month,(RTC.Year % 100));
	sprintf(EndDate,"%02u%02u%02u",RTC.Date,RTC.Month,(RTC.Year % 100));
	memcpy(LcdDispBuff,"   Start Date   ",16);
	memcpy(LcdDispBuff+16,"                ",16);
//forsendingdata=0;
    if(forsendingdata!=1)
    {
        if(GetRTCData(3,StartDate,(char *)"010100",(char *)"311299",(char *)"N3N1N0"))
        {
            memcpy(TempDate,StartDate+4,2);
            memcpy(TempDate+2,StartDate+2,2);
            memcpy(TempDate+4,StartDate,2);
            From = atol(TempDate);
            memcpy(LcdDispBuff,"    End Date    ",16);
            memcpy(LcdDispBuff+16,"                ",16);

            if(GetRTCData(3,EndDate,(char *)"010100",(char *)"311299",(char *)"N3N1N0"))
            {
                memcpy(TempDate,EndDate+4,2);
                memcpy(TempDate+2,EndDate+2,2);
                memcpy(TempDate+4,EndDate,2);
                End = atol(TempDate);
            }
            else
                return;
        }
        else
            return;
    }
	LcdDisplay(TOP,(char *)"                ");
	LcdDisplay(BOTTOM,(char *)"     Wait...    ");

	if(!Bits.RecOverWrite)
	{
//        memcpy(LcdDispBuff,"above for loop  ",16);
//        LcdDisplay(TOP,LcdDispBuff);        
		for(TripNo = 0;TripNo < AllChannel.TripCount;TripNo++)
		{
            record_read_validate();
//          sprintf(LcdDispBuff,"1:%d",TripNo);
////          LcdDisplay(TOP,LcdDispBuff);
//			Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
////          sprintf(LcdDispBuff,"2:%d",TripNo);
////          LcdDisplay(TOP,LcdDispBuff);
//            current_date_assign();
////          sprintf(LcdDispBuff,"3:%lu",CurDate);
////          LcdDisplay(TOP,LcdDispBuff);
//            if(forsendingdata!=1)
//            {
////              sprintf(LcdDispBuff,"4:%lu",CurDate);
////              LcdDisplay(TOP,LcdDispBuff);
//                if(CurDate >= From && CurDate <= End)
//                {
//                    CumTrips += 1;	
//                    if(TripRecord.Weight != 9999)	//v1.0.6
//                    {
//                        Cum_Wt = Cum_Wt + TripRecord.Weight;
////                        sprintf(LcdDispBuff,"1:%lu",Cum_Wt);
////                        LcdDisplay(TOP,LcdDispBuff);
////                        Delay_1sec(10);
//                    }
//                    if(TripRecord.OLStatus)
//                    {	
//                        OLTrips = OLTrips + 1;
//                    }
//                }
//                CurDate = 0;
//            }
//            else
//            {
//                
//                CumTrips += 1;	
//                if(TripRecord.Weight != 9999)	//v1.0.6
//                {
//                        Cum_Wt = Cum_Wt + TripRecord.Weight;
////                    sprintf(LcdDispBuff,"2:%lu",Cum_Wt);
////                    LcdDisplay(TOP,LcdDispBuff);
////                    Delay_1sec(10);
//                }
//                if(TripRecord.OLStatus)
//                {	
//                    OLTrips = OLTrips + 1;
//                }
//                CurDate = 0;
//            }
	   }
	}
	else
	{
		for(TripNo = (AllChannel.TripCount % MAXRECORDS);TripNo < MAXRECORDS;TripNo++)
		{
            record_read_validate();
//			Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));
////			CurDate += (TripRecord.Year-2000);
////			CurDate *= 10000;
////			CurDate += TripRecord.Date;	
////			CurDate += (TripRecord.Month * 100);
//            current_date_assign();	
//			if(forsendingdata!=1)
//			{
//                if(CurDate >= From && CurDate <= End)
//                {
//                    CumTrips += 1;	
//                    if(TripRecord.Weight != 9999)	//v1.0.6
//                    {
//                        Cum_Wt = Cum_Wt + TripRecord.Weight;
////                        sprintf(LcdDispBuff,"3:%lu",Cum_Wt);
////                        LcdDisplay(TOP,LcdDispBuff);
////                        Delay_1sec(10);
//                    }
//                    if(TripRecord.OLStatus)
//                    {	
//                        OLTrips = OLTrips + 1;
//                    }
//                }
//                CurDate = 0;
//			}
//            else
//            {
//                CumTrips += 1;	
//                if(TripRecord.Weight != 9999)	//v1.0.6
//                {
//                        Cum_Wt = Cum_Wt + TripRecord.Weight;
////                    sprintf(LcdDispBuff,"4:%lu",Cum_Wt);
////                    LcdDisplay(TOP,LcdDispBuff);
////                    Delay_1sec(10);
//                }
//                if(TripRecord.OLStatus)
//                {	
//                    OLTrips = OLTrips + 1;
//                }
//                CurDate = 0;
//            }
		}
		for(TripNo = 0;TripNo <= ((AllChannel.TripCount % MAXRECORDS)-1);TripNo++)
		{
			if(AllChannel.TripCount % MAXRECORDS == 0)
				break;
            record_read_validate();
		}
	}

	if(AllChannel.TripCount == 0)
	{
        Com1StringSend((char *)"---------------NO RECORD----------------",40);
        Print_CRLF(2);
//		memcpy(LcdDispBuff,,16);
//		memcpy(LcdDispBuff+16,,16);
		LcdDisplay(TOP,(char *)"                ");
		LcdDisplay(BOTTOM,(char *)"  * NO RECORD * ");
		Delay_1sec(2);
		return;
	}	
	else
	{
//	memcpy(LcdDispBuff,,16);
//	memcpy(LcdDispBuff+16,,16);
	LcdDisplay(TOP,(char *)"                ");
	LcdDisplay(BOTTOM,(char *)"   Printing...  ");

    Print_CRLF(1);
	Com1StringSend((char *)"       FIXED BODY TRUCK WEIGHING        ",40);
    Print_CRLF(1);
	Com1StringSend((char *)"----------------------------------------",40);
    Print_CRLF(1);
	ReadRTC_DateTime();
	sprintf(PrintBuff,"  Date: %02u/%02u/%4d",RTC.Date,RTC.Month,RTC.Year);
	memcpy(PrintBuff+18,"      ",6);
	sprintf(PrintBuff+24,"Time: %02u:%02u:%02u  ",RTC.Hour,RTC.Minute,RTC.Second);
	Com1StringSend(PrintBuff,40);
    Print_CRLF(1);
	Com1StringSend((char *)"----------------------------------------",40);
    Print_CRLF(2);
	memcpy(PrintBuff,"   FROM: xx/xx/20xx    TO: xx/xx/20xx   ",40);
	memcpy(PrintBuff+9,StartDate,2);memcpy(PrintBuff+12,StartDate+2,2);memcpy(PrintBuff+17,StartDate+4,2);
	memcpy(PrintBuff+27,EndDate,2);memcpy(PrintBuff+30,EndDate+2,2);memcpy(PrintBuff+35,EndDate+4,2);
	Com1StringSend(PrintBuff,40);	
    Print_CRLF(2);
	memset(PrintBuff,' ',40);
	memcpy(PrintBuff+5," DEVICE ID              : ",26);
	Read_E2((char*)&Setup.DeviceID,EDeviceID,sizeof(Setup.DeviceID));
//	Setup.deviceid+=48;
	sprintf(PrintBuff+31,"%d",Setup.DeviceID);
	Com1StringSend(PrintBuff,40);	
    Print_CRLF(1);
	memset(PrintBuff,' ',40);
//	memcpy(PrintBuff+5," MODEL     : ",13);
//	for(dummy=0;dummy<=5;dummy++)
//	{
//	if(!strncmp(Setup.modelnumbervalue,"BLANK",5))
//		memcpy(PrintBuff+(18+dummy),"-----",5);
//	else
//		sprintf(PrintBuff+(18+dummy),"%c",Setup.modelnumbervalue[dummy]);
//	}
////memcpy(PrintBuff+18,"   EH600",8);
//	Com1StringSend(PrintBuff,40);	
//	EUSART1_Write(0x0d);
//	EUSART1_Write(0x0a);	
//	memset(PrintBuff,' ',40);
//	memcpy(PrintBuff+5," SERIAL NO : ",13);
//	sprintf(PrintBuff+21,"%06lu ",Setup.TruckSerial);
//	Com1StringSend(PrintBuff,40);
//	Com1StringSend(PrintBuff,26);
//	EUSART1_Write(0x0d);
//	EUSART1_Write(0x0a);
//	EUSART1_Write(0x0d);
//	EUSART1_Write(0x0a);
	memcpy(PrintBuff,"      Cumulative Weight : ",26);
	sprintf(PrintBuff+26,"%06lu",Cum_Wt);
	PutDp((unsigned char*)PrintBuff+26,1,6,1);
	memcpy(PrintBuff+33," T     ",7);
	Com1StringSend(PrintBuff,40);
    Print_CRLF(1);
	memcpy(PrintBuff,"      Number Of Trips   :    ",29);
	sprintf(PrintBuff+29,"%4d",CumTrips);
	memcpy(PrintBuff+33,"        ",7);
	Com1StringSend(PrintBuff,40);
    Print_CRLF(1);
	
//	memcpy(PrintBuff,"      Over Load Trips   :    ",29);
//	sprintf(PrintBuff+29,"%4d",OLTrips);
//	memcpy(PrintBuff+33,"       ",7);
//	Com1StringSend(PrintBuff,40);
//    Print_CRLF(2);
	
	Com1StringSend((char *)"   SNO     DATE      TIME     T.Wt(T)   ",40);
    Print_CRLF(1);
	memset(PrintBuff,' ',40);
	CumTrips = 0; // To print the Serial No

	if(!Bits.RecOverWrite)
	{
		for(TripNo = 0;TripNo < AllChannel.TripCount;TripNo++)
		{
			Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
//            CurDate += (TripRecord.Year-2000);
//			CurDate *= 10000;
//			CurDate += TripRecord.Date;	
//			CurDate += (TripRecord.Month * 100);          
            current_date_assign();
            if(forsendingdata!=1)
            {
                if(CurDate >= From && CurDate <= End)
                {
                    Print_record();
                }
                CurDate = 0;
            }
            else
            {
//                    sprintf(LcdDispBuff,"6:%lu",CurDate);
//                    LcdDisplay(TOP,LcdDispBuff);   
//                    Delay(2000);     
                    Print_record();                
//                    sprintf(PrintBuff+1,"%4d",++CumTrips);
//                    memcpy(PrintBuff+5,"   ",3);
//                    sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
//                    memcpy(PrintBuff+18,"   ",3);
//                    sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
//                    memcpy(PrintBuff+26,"  ",23);
//                    sprintf(PrintBuff+28,"%06lu",TripRecord.Weight);
//                    PutDp((unsigned char*)PrintBuff+28,1,6,1);
//                    if(TripRecord.OLStatus)
//                        memcpy(PrintBuff+35,"*    ",5);
//                    else
//                        memcpy(PrintBuff+35,"     ",5);
//                    Com1StringSend(PrintBuff,40);
//                    Print_CRLF(1);
                    CurDate = 0;
            }
		}
	}
	else
	{
		for(TripNo = (AllChannel.TripCount % MAXRECORDS);TripNo < MAXRECORDS;TripNo++)
		{
			Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));
//			CurDate += (TripRecord.Year-2000);
//			CurDate *= 10000;
//			CurDate += TripRecord.Date;	
//			CurDate += (TripRecord.Month * 100);
            current_date_assign();
            if(forsendingdata!=1)
            {
                if(CurDate >= From && CurDate <= End)
                {
                    Print_record();                
    //				sprintf(PrintBuff+1,"%4d",++CumTrips);
    //				memcpy(PrintBuff+5,"   ",3);
    //				sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
    //				memcpy(PrintBuff+18,"   ",3);
    //				sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
    //				memcpy(PrintBuff+26,"  ",23);
    //				sprintf(PrintBuff+28,"%06lu",TripRecord.Weight);
    //				PutDp((unsigned char*)PrintBuff+28,1,6,1);
    //				if(TripRecord.OLStatus)
    //					memcpy(PrintBuff+35,"*    ",5);
    //				else
    //					memcpy(PrintBuff+35,"     ",5);
    //				Com1StringSend(PrintBuff,40);
    //                Print_CRLF(1);
                }
                CurDate = 0;
			}
			else
			{
                Print_record();            
//				sprintf(PrintBuff+1,"%4d",++CumTrips);
//				memcpy(PrintBuff+5,"   ",3);
//				sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
//				memcpy(PrintBuff+18,"   ",3);
//				sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
//				memcpy(PrintBuff+26,"  ",23);
//				sprintf(PrintBuff+28,"%06lu",TripRecord.Weight);
//				PutDp((unsigned char*)PrintBuff+28,1,6,1);
//				if(TripRecord.OLStatus)
//					memcpy(PrintBuff+35,"*    ",5);
//				else
//					memcpy(PrintBuff+35,"     ",5);
//				Com1StringSend(PrintBuff,40);
//              Print_CRLF(1);
				CurDate = 0;
			}
		}
		for(TripNo = 0;TripNo <= ((AllChannel.TripCount % MAXRECORDS)-1);TripNo++)
		{
			if(AllChannel.TripCount % MAXRECORDS == 0)
				break;
			Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));
//			CurDate += (TripRecord.Year-2000);
//			CurDate *= 10000;
//			CurDate += TripRecord.Date;	
//			CurDate += (TripRecord.Month * 100);
            current_date_assign();
			if(forsendingdata!=1)
			{
				if(CurDate >= From && CurDate <= End)
				{
                    Print_record();                    
    //				sprintf(PrintBuff+1,"%4d",++CumTrips);
    //				memcpy(PrintBuff+5,"   ",3);
    //				sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
    //				memcpy(PrintBuff+18,"   ",3);
    //				sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
    //				memcpy(PrintBuff+26,"  ",23);
    //				sprintf(PrintBuff+28,"%06lu",TripRecord.Weight);
    //				PutDp((unsigned char*)PrintBuff+28,1,6,1);
    //				if(TripRecord.OLStatus)
    //					memcpy(PrintBuff+35,"*    ",5);
    //				else
    //					memcpy(PrintBuff+35,"     ",5);
    //				Com1StringSend(PrintBuff,40);
    //                Print_CRLF(1);
				}
				CurDate = 0;
			}
			else
			{
                Print_record();                
//				sprintf(PrintBuff+1,"%4d",++CumTrips);
//				memcpy(PrintBuff+5,"   ",3);
//				sprintf(PrintBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
//				memcpy(PrintBuff+18,"   ",3);
//				sprintf(PrintBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
//				memcpy(PrintBuff+26,"  ",23);
//				sprintf(PrintBuff+28,"%06lu",TripRecord.Weight);
//				PutDp((unsigned char*)PrintBuff+28,1,6,1);
//				if(TripRecord.OLStatus)
//					memcpy(PrintBuff+35,"*    ",5);
//				else
//					memcpy(PrintBuff+35,"     ",5);
//				Com1StringSend(PrintBuff,40);
//                Print_CRLF(1);
				CurDate = 0;
			}
		}
	}	
    forsendingdata=0;
	Com1StringSend((char *)"----------------------------------------",40);
    Print_CRLF(1);
	Com1StringSend((char *)" *     - OverLoaded                     ",40);
    Print_CRLF(1);
}
#endif
}
*/
void Print_CRLF(char No_of_print)
{
    char dummyvalue = 0;
    for(dummyvalue = 0;dummyvalue < No_of_print;dummyvalue++)
    {
        Com2Bytesend(0x0d);
        Com2Bytesend(0x0a);	
    }
}
/*
void PrintResetOption()
{
	while(1)
	{
		LcdDisplay(TOP,(char *)"      PRESS     ");
		LcdDisplay(BOTTOM,(char *)" UP-PRN LEFT-CLR");

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
			LcdDisplay(TOP,(char *)"                ");
			LcdDisplay(BOTTOM,(char *)"   Printing...  ");            
			Print_record();	
			break;
		}
		else if(LeftKey)
		{
			LcdDisplay(TOP,(char *)" ENTER PASSWORD ");
			LcdDisplay(BOTTOM,(char *)" (RESET TRIPS)  ");
			Delay1_1s(2);
			if(GetPassword())
			{			
				Reset_Cum_Trip();
			}
			break;
		}
	}
	return;
}
*/
void ADC_Init(void)
{
        //ADCON1 Register
        //Set up A/D for Automatic Sampling
        //Use internal counter (SAMC) to provide sampling time
        //Set up A/D conversrion results to be read in unsigned integer format.
        //Set up Sequential sampling for multiple S/H amplifiers
        //All other bits to their default state

		_TRISB8 = 1;		// Input Config
		_TRISB4 = 1;		// Input Config     //  In Ver2.1 from _TRISB9 to _TRISB4
		_TRISB10 = 1;		// Input Config
		_TRISB11 = 1;		// Input Config
		_TRISB12 = 1;		// Input Config
		_TRISB13 = 1;		// Input Config

		//ADPCFG Register
        //Set up channels AN8,9,10,11,12,13 as analog input
        ADPCFG = 0xFFFF;

        _PCFG8  = 0;
		_PCFG4  = 0;    //  In Ver2.1 from _PCFG9 to _PCFG4
		_PCFG10 = 0;
		_PCFG11 = 0;	
		_PCFG12 = 0;
		_PCFG13 = 0;	

        ADCON1bits.FORM = 0;		// Unsigned integer O/P
        ADCON1bits.SSRC = 7;		// internal counter ends & conv start
        ADCON1bits.ASAM = 1;		// imm after last conv
									// SIMSAM = 0

        //ADCON2 Register
        //Set up A/D for interrupting after 16 samples get filled in the buffer
        //All other bits to their default state

		ADCON2bits.VCFG = 3;		// External Ref + and Ref-
        ADCON2bits.SMPI = 15;		// Interuupt after 15 convertions
									// Can reduce depend on speed

		//ADCON2bits.CHPS = 1;		// check after

        //ADCON3 Register
        //We will set up the ADRC as the A/D conversion clock
        //so that the A/D converter can operate when the device is in
        //SLEEP mode. Also, 1 Tad period is allocated for sampling time.
        //The conversion rate for the ADRC oscillator is depends on whether
        //the device is a dsPIC30F or dsPIC33F device and also whether the
        //A/D module is a 10-bit or 12-bit A/D converter.
        //Please refer to the device Datasheet for "ADRC" conversion rate.

        ADCON3bits.SAMC = 1;
        ADCON3bits.ADRC = 1;
		ADCON3bits.ADCS = 0;		// 200 KHZ
		
        //ADCHS Register
        //Set up A/D Channel Select Register to convert AN8 on Mux A input
        //of CH0 S/H amplifiers

        // ADCHS = 0x0008;			// channel No

        //ADCSSL Register
        //Channel Scanning is disabled. All bits left to their default state
        ADCSSL = 0x0000;
        
        //Clear the A/D interrupt flag bit
        IFS0bits.ADIF = 0;

        //Set the A/D interrupt enable bit
        IEC0bits.ADIE = 1;

        //Turn on the A/D converter
        //This is typically done after configuring other registers
      //  ADCON1bits.ADON = 1;

}

//void UART_Config(void)
//{
//	Com1Init();
//	Com2Init(Application_Setting.BuadRate);	
//}


void Reset_Cum_Trip()
{

	while(1)
	{
		TwoLineDisplay(" Clear Cum value"," & Trip Count ? ",0);
		KeySense();		
		if(MenuKey)
		{
			memset(LcdDispBuff,' ',32);
//			LcdDisplay(TOP,LcdDispBuff);
//			LcdDisplay(BOTTOM,LcdDispBuff+16);
			break;				
		}
		else if(EnterKey)
		{
			AllChannel.CumulativeWeight = 0;
			AllChannel.TripCount = 0;
			Bits.RecOverWrite = 0;
			Bits.Disp_Latch_Wt = 0;
    //      AllChannel.LatchedWt = 0;             
			Status_Write(LOAD);//Application_Setting.PLMS_Status = LOAD;
			Write_E2(EE_CumulativeWeight,(char*)&AllChannel.CumulativeWeight,sizeof(AllChannel.CumulativeWeight));Delay_ms(2000);
			Write_E2(EE_TripCount,(char*)&AllChannel.TripCount,sizeof(AllChannel.TripCount));Delay_ms(2000);
	//		Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));Delay_ms(2000);
			break;
		}
	}
}

/*void Power_on_tare_Function(void)
{
    int i=0;
    Weight_Calc();
    if(Setup.PowerOnTare != 0) 
    {
//        sprintf(LcdDispBuff,"1:%d",Setup.PowerOnTare);
//        LcdDisplay(TOP,LcdDispBuff);
//        Delay_1sec(5);        
        for(i=0;i<4;i++)
        {
            if(Loadcell[i].CurWeight <= ((Setup.PowerOnTare*MaxCapacity)/100))		//Power On Tare
            {
                Loadcell[i].Zero = Loadcell[i].RawADC;
            }
        }
    }	      
}*/

//This function added from Tipper software
void Power_On_KeypressFunction()
{
	unsigned int Tempvariable = 0;    
    unsigned char UpKeySel =0,UpKeySel_2 = 0;
    unsigned char Selected_Option = 0;
    ClearLcdDisplay(TOP_BOTTOM);//Delay_1sec(2);//Delay_ms(10);
    LcdDisplay(BOTTOM,(char *)" Please wait... ");
    while(1)
	{
		KeySense();
		if(MenuKey)
		{
            TwoLineDisplay(" ENTER PASSWORD ","     (MENU)     ",1);
			if(GetPassword())
			{
                LcdDisplay(TOP,(char *)"      MENU      ");
                while(1)
                {
                    Selected_Option = OptionChooseFunction("   Volt Calib       Logic Sel   ",2,1);
                    if(Selected_Option)
                    {
                        if(Selected_Option == 1)
                        {
//                            TimeOutFlag.TimeOutEnb = 1;TimeOutFlag.TimeOut = 0;TimeOut_Cnt = 0;        
                            curindex = 1;
                            ClearLcdDisplay(TOP_BOTTOM);
                            DoMenu();
                            break;                       
                        }
                        else if(Selected_Option == 2)
                        {
                            Selected_Option = Application_Setting.OperationLogic;
                            if((Selected_Option > 2) || (Selected_Option <= 0))
                                Selected_Option = 1;
                            Selected_Option = OptionChooseFunction("   LOGIC - 1       LOGIC - 2    ",2,Selected_Option);
                            if(Selected_Option)
                            {
                                Application_Setting.OperationLogic = Selected_Option;
                                sprintf(LcdDispBuff,"LOGIC-%d Selected",Application_Setting.OperationLogic);
                                TwoLineDisplay("                ",LcdDispBuff,2);
                                Write_E2(EE_OperationLogic,(char*)&Application_Setting.OperationLogic,sizeof(Application_Setting.OperationLogic));	
                                Status_Write(LOAD);//Application_Setting.PLMS_Status = LOAD;
            //                    Write_E2(E_PLMS_Status,(char*)&Application_Setting.PLMS_Status,sizeof(Application_Setting.PLMS_Status));      
                                break;
                            }
                            else
                                break;
                        }
                    }
                    else
                        break;
                }
//                if(TimeOutFlag.TimeOut)
//                    return;                    
                }
//			TimeOutFlag.TimeOut = 0;
		}
		if(Tempvariable++>40000)
		{
			Tempvariable = 0;
			break;
		}  
	}
}

void LED_ON_OFF(char LED_1_Status,char LED_2_Status,char LED_3_Status)
{
    if(LED_1_Status == 1)
        GREENLED_OP2 = 1;
    else
        GREENLED_OP2 = 0;

    if(LED_2_Status == 1)
        AMBERLED_OP3 = 1;
    else
        AMBERLED_OP3 = 0;

    if(LED_3_Status == 1)
        REDLED_OP1 = 1;
    else
        REDLED_OP1 = 0;
}

void RangeOutput()
{
    if(Application_Setting.OperationLogic == BodyUp)
    {
        if(IBODYUP == Application_Setting.Input2Logic) 
            LED_ON_OFF(1,0,0);  
        else
            LED_ON_OFF(0,0,0);  
    }
    else
    {
        if(Application_Setting.PLMS_Status == LOAD)
        {
            if(Bits.ERRFlag)
            {
                LED_ON_OFF(0,0,0);
    //            IO_RD7_LED1_SetLow();
    //            IO_RD6_LED2_SetLow();
    //            IO_RB0_LED3_SetLow();
            }
            else
            {
                if(AllChannel.CurWeight >= Application_Setting.Red_Glow)	//RED LAMP ON
                {
                    LED_ON_OFF(0,0,1);                
    //              IO_RD7_LED1_SetLow();
    //              IO_RD6_LED2_SetLow();
    //              IO_RB0_LED3_SetHigh();
                }
                else if(AllChannel.CurWeight >= Application_Setting.Amber_Flash && AllChannel.CurWeight < Application_Setting.Red_Glow)	//AMBER FLASHES
                {
    //              IO_RD7_LED1_SetLow();
    //              IO_RB0_LED3_SetLow();
                    if(OP_Flash_Cntr++ <= 10)
                    {
                        LED_ON_OFF(0,1,0);                     
      //                IO_RD6_LED2_SetHigh();
                    }
                    else
                    {
                        LED_ON_OFF(0,0,0);
    //                  IO_RD6_LED2_SetLow();
                        if(OP_Flash_Cntr >= 20)
                            OP_Flash_Cntr = 0;
                    }

                }
                else if(AllChannel.CurWeight >= Application_Setting.Amber_Glow && AllChannel.CurWeight < Application_Setting.Amber_Flash)
                {
                    LED_ON_OFF(0,1,0);                   
    //              IO_RD7_LED1_SetLow();
    //              IO_RB0_LED3_SetLow();
    //				IO_RD6_LED2_SetHigh();
                }
                else if(AllChannel.CurWeight >= Application_Setting.MinWt)
                {
                    LED_ON_OFF(1,0,0);  
    //              IO_RD7_LED1_SetHigh();
    //              IO_RD6_LED2_SetLow();
    //              IO_RB0_LED3_SetLow();
                }
                else
                {
                    LED_ON_OFF(0,0,0);                
    //              IO_RD7_LED1_SetLow();
    //              IO_RD6_LED2_SetLow();
    //              IO_RB0_LED3_SetLow();
                }
            }
        }
        else
        {
            LED_ON_OFF(0,0,0);           
    //      IO_RD6_LED2_SetLow();
    //      IO_RD7_LED1_SetLow();
    //      IO_RB0_LED3_SetLow();
        }
    }
	Bits.ERRFlag = 0; // Each time Error flag should be updated...
}


void TareFunction(void)
{
    signed char Tempvariable = 1;   
    LcdDisplay(TOP,(char *)"      TARE      ");    
    while(1)
	{
        if(Tempvariable == 1)
            memcpy(LcdDispBuff+16,"   AXLE-1(3&4)  ",16);
        else if(Tempvariable == 2)
            memcpy(LcdDispBuff+16,"   AXLE-2(5&6)  ",16);
        else if(Tempvariable == 3)
            memcpy(LcdDispBuff+16,"  ANGLE SENSOR  ",16);
        else if(Tempvariable == 4)
            memcpy(LcdDispBuff+16,"  ALL CHANNELS  ",16);
        LcdDisplay(BOTTOM,LcdDispBuff+16);
		KeySense();
		if(MenuKey)
		{
            ClearLcdDisplay(TOP_BOTTOM);
			break;
		}
		else if(UpKey)
		{
			if(Tempvariable++ >= 4)
				Tempvariable = 1;
		}        
		else if(EnterKey)
		{
            CalcADC();            
            if((Tempvariable == 1)||(Tempvariable == 4))
            {
                if(Application_Setting.LCED[CH3] == 1)
                {
                    Loadcell[CH3].Zero  = Loadcell[CH3].RawADC;
                    Write_E2(ELC3Zero ,(char*)&Loadcell[CH3].Zero ,sizeof(Loadcell[CH3].Zero));
                }
                if(Application_Setting.LCED[CH4] == 1)
                {
                    Loadcell[CH4].Zero  = Loadcell[CH4].RawADC;
                    Write_E2(ELC4Zero ,(char*)&Loadcell[CH4].Zero ,sizeof(Loadcell[CH4].Zero));
                }
            }
            if((Tempvariable == 2)||(Tempvariable == 4))
            {
                if(Application_Setting.LCED[CH5] == 1)
                {
                    Loadcell[CH5].Zero  = Loadcell[CH5].RawADC;
                    Write_E2(ELC5Zero ,(char*)&Loadcell[CH5].Zero ,sizeof(Loadcell[CH5].Zero));
                }
                if(Application_Setting.LCED[CH6] == 1)
                {
                    Loadcell[CH6].Zero  = Loadcell[CH6].RawADC;
                    Write_E2(ELC6Zero ,(char*)&Loadcell[CH6].Zero ,sizeof(Loadcell[CH6].Zero));
                }
            }
            if((Tempvariable == 3)||(Tempvariable == 4))
            {
                if(Application_Setting.LCED[CH1] == 1)
                {
                    Loadcell[CH1].Zero  = Loadcell[CH1].RawADC;
                    Write_E2(ELC1Zero ,(char*)&Loadcell[CH1].Zero ,sizeof(Loadcell[CH1].Zero));
                }
                if(Application_Setting.LCED[CH2] == 1)
                {
                    Loadcell[CH2].Zero  = Loadcell[CH2].RawADC;
                    Write_E2(ELC2Zero ,(char*)&Loadcell[CH2].Zero ,sizeof(Loadcell[CH2].Zero));			
                }
            }
            LasttareStore();
        //    done_Display(); 
            memset(LcdDispBuff,' ',32);
            break;            
        }
	}	 
}

void Voltage_Calib_init(void)
{
    VoltageCalib.Ch1Ref1 = 770;
    VoltageCalib.Ch2Ref1 = 770;
    VoltageCalib.Ch3Ref1 = 770;
    VoltageCalib.Ch4Ref1 = 770;
    VoltageCalib.Ch5Ref1 = 770;
    VoltageCalib.Offset_Volt = 200;
    VoltageCalib.Ch1Ref2 = 3040;
    VoltageCalib.Ch2Ref2 = 3040;
    VoltageCalib.Ch3Ref2 = 3040;
    VoltageCalib.Ch4Ref2 = 3040;	
    VoltageCalib.Ch5Ref2 = 3040;	
    VoltageCalib.REF2Cnt = 400;    
    VoltageCalib.REF1Cnt = 400;    
    VoltageCalib.REFvolt2 = 4000;    
    VoltageCalib.REFvolt1 = 1000;    
//  VoltageCalib.ChxRef_Volt = 200;	
    voltage_Calib_write();
//  Write_E2(EE_ChxRef_Volt,(char*)&VoltageCalib.ChxRef_Volt,sizeof(VoltageCalib.ChxRef_Volt));
}

void voltage_Calib_write(void)
{
//    sprintf(LcdDispBuff,"2:%lu:%lu",VoltageCalib.REFvolt1,VoltageCalib.REFvolt2);
//    LcdDisplay(TOP,LcdDispBuff);
//    Delay_1sec(15);   

    Write_E2(EE_REF1Cnt,(char*)&VoltageCalib.REF1Cnt,sizeof(VoltageCalib.REF1Cnt));
    Write_E2(EE_REF2Cnt,(char*)&VoltageCalib.REF2Cnt,sizeof(VoltageCalib.REF2Cnt));
    
    Write_E2(EE_REFvolt1,(char*)&VoltageCalib.REFvolt1,sizeof(VoltageCalib.REFvolt1));
    Write_E2(EE_REFvolt2,(char*)&VoltageCalib.REFvolt2,sizeof(VoltageCalib.REFvolt2));
//    VoltageCalib.REFvolt1 = 0;
//    VoltageCalib.REFvolt2 = 0;
//    Read_E2((char *)&VoltageCalib.REFvolt1,EE_REFvolt1,sizeof(VoltageCalib.REFvolt1));
//    Read_E2((char *)&VoltageCalib.REFvolt2,EE_REFvolt2,sizeof(VoltageCalib.REFvolt2));        
//    sprintf(LcdDispBuff,"3:%lu:%lu",VoltageCalib.REFvolt1,VoltageCalib.REFvolt2);
//    LcdDisplay(TOP,LcdDispBuff);
//    Delay_1sec(15);   
    Write_E2(EE_Ch1Ref1,(char*)&VoltageCalib.Ch1Ref1,sizeof(VoltageCalib.Ch1Ref1));	
    Write_E2(EE_Ch2Ref1,(char*)&VoltageCalib.Ch2Ref1,sizeof(VoltageCalib.Ch2Ref1));	
    Write_E2(EE_Ch3Ref1,(char*)&VoltageCalib.Ch3Ref1,sizeof(VoltageCalib.Ch3Ref1));	
    Write_E2(EE_Ch4Ref1,(char*)&VoltageCalib.Ch4Ref1,sizeof(VoltageCalib.Ch4Ref1));	
    Write_E2(EE_Ch5Ref1,(char*)&VoltageCalib.Ch5Ref1,sizeof(VoltageCalib.Ch5Ref1));	
    Write_E2(EE_Ch6Ref1,(char*)&VoltageCalib.Ch6Ref1,sizeof(VoltageCalib.Ch6Ref1));	
    Write_E2(EE_Ch1Ref2,(char*)&VoltageCalib.Ch1Ref2,sizeof(VoltageCalib.Ch1Ref2));	
    Write_E2(EE_Ch2Ref2,(char*)&VoltageCalib.Ch2Ref2,sizeof(VoltageCalib.Ch2Ref2));	
    Write_E2(EE_Ch3Ref2,(char*)&VoltageCalib.Ch3Ref2,sizeof(VoltageCalib.Ch3Ref2));	
    Write_E2(EE_Ch4Ref2,(char*)&VoltageCalib.Ch4Ref2,sizeof(VoltageCalib.Ch4Ref2));	    
    Write_E2(EE_Ch5Ref2,(char*)&VoltageCalib.Ch5Ref2,sizeof(VoltageCalib.Ch5Ref2));	    
    Write_E2(EE_Ch6Ref2,(char*)&VoltageCalib.Ch6Ref2,sizeof(VoltageCalib.Ch6Ref2));	    
    Write_E2(EE_Offset_Volt,(char*)&VoltageCalib.Offset_Volt,sizeof(VoltageCalib.Offset_Volt));    
}

void Weight_Calculation(char Indi_Axle_Calc)
{
//    char Axle_Counter = 0;    
//    for(Axle_Counter = Axle1;Axle_Counter < Axle_NOs;Axle_Counter++)
//    {
//        CalcADC();
//        if(Axle_Counter == Axle1)
//            Weight_Calc(Axle_Counter,Loadcell[CH1].RawADC,Loadcell[CH2].RawADC,Indi_Axle_Calc);
//        else
//            Weight_Calc(Axle_Counter,Loadcell[CH3].RawADC,Loadcell[CH4].RawADC,Indi_Axle_Calc);
//    }
}

void assigning_values(void)
{
        Latching_Time = (Application_Setting.LatchDelay * ((unsigned long)100));                                                  //used to convert in sec   
        Periodic_Time = (unsigned long)(((Application_Setting.Perioric_Timer.Minute *60)+Application_Setting.Perioric_Timer.Second) * ((unsigned long)1000));//used to convert in min        
//        sprintf(LcdDispBuff,"%lu:%02u:%02u",Periodic_Time,Application_Setting.Perioric_Timer.Minute,Application_Setting.Perioric_Timer.Second);	
//        LcdDisplay(TOP,LcdDispBuff);
//        Delay1_1s(5);    
        //    Axle[Axle1].Zero1 = Axle[Axle2].Zero1 = 0;
//    Axle[Axle1].Span = Axle[Axle2].Span = 0;
//    /*  If 1 & 2 channels enabled Zero1,Span is taken as counts for the calculation
//     *  If 1 or 2 channels enabled Zero1,Span is taken as voltage.and the voltage converted to counts for the calculation
//     *  above operation same for 3 & 4 Channel also.
//    */
//    if((Setup.LCED[CH1] == 1) && (Setup.LCED[CH2] == 1))//Axle _calib enabled
//    {
////      Loadcell[CH1].Zero = Loadcell[CH1].Zero1;
////      Loadcell[CH2].Zero = Loadcell[CH2].Zero1;
//        Axle[Axle1].CH1_Difference = abs(Loadcell[CH1].Span  - Loadcell[CH1].Zero1);//Here it is Count
//        Axle[Axle1].CH2_Difference = abs(Loadcell[CH2].Span - Loadcell[CH2].Zero1);//Here it is Count
//    }
//    else    //Individual calib
//    {
//        if(Setup.LCED[CH1] == 1)
//        {
////          Loadcell[CH1].Zero = (Volt_to_count_Conv(Loadcell[CH1].Zero1,VoltageCalib.Ch1Ref2,VoltageCalib.Ch1Ref1));//Here it is voltage
//            Axle[Axle1].Zero1 = Axle[Axle1].Zero1 + Loadcell[CH1].Zero1;
//            Axle[Axle1].Span = Axle[Axle1].Span + Loadcell[CH1].Span;
////            Axle[Axle1].Zero1 = Axle[Axle1].Zero1  + (Volt_to_count_Conv(Loadcell[CH1].Zero1,VoltageCalib.Ch1Ref2,VoltageCalib.Ch1Ref1));
////            Axle[Axle1].Span = Axle[Axle1].Span + (Volt_to_count_Conv(Loadcell[CH1].Span,VoltageCalib.Ch1Ref2,VoltageCalib.Ch1Ref1));//Here it is voltage
//        }
//        if(Setup.LCED[CH2] == 1)
//        {
////          Loadcell[CH2].Zero = (Volt_to_count_Conv(Loadcell[CH2].Zero1,VoltageCalib.Ch2Ref2,VoltageCalib.Ch2Ref1));//Here it is voltage
//            Axle[Axle1].Zero1 = Axle[Axle1].Zero1 + Loadcell[CH2].Zero1;
//            Axle[Axle1].Span = Axle[Axle1].Span + Loadcell[CH2].Span;
//        }
//    }
//    
//    if((Setup.LCED[CH3] == 1) && (Setup.LCED[CH4] == 1))
//    {    
////      Loadcell[CH3].Zero = Loadcell[CH3].Zero1;
////      Loadcell[CH4].Zero = Loadcell[CH4].Zero1;        
//        Axle[Axle2].CH1_Difference = abs(Loadcell[CH3].Span  - Loadcell[CH3].Zero1);//Here it is Count
//        Axle[Axle2].CH2_Difference = abs(Loadcell[CH4].Span - Loadcell[CH4].Zero1);//Here it is Count
//    }
//    else
//    {
//        if(Setup.LCED[CH3] == 1)
//        {
////            Loadcell[CH3].Zero = (Volt_to_count_Conv(Loadcell[CH3].Zero1,VoltageCalib.Ch3Ref2,VoltageCalib.Ch3Ref1));//Here it is voltage            
//            Axle[Axle2].Zero1 = Axle[Axle2].Zero1 + Loadcell[CH3].Zero1;
//            Axle[Axle2].Span = Axle[Axle2].Span + Loadcell[CH3].Span;
//        }
//        if(Setup.LCED[CH4] == 1)
//        {
////            Loadcell[CH4].Zero = (Volt_to_count_Conv(Loadcell[CH4].Zero1,VoltageCalib.Ch4Ref2,VoltageCalib.Ch4Ref1));//Here it is voltage            
//            Axle[Axle2].Zero1 = Axle[Axle2].Zero1 + Loadcell[CH4].Zero1;
//            Axle[Axle2].Span = Axle[Axle2].Span + Loadcell[CH4].Span;
//        }            
//    }
//    Latching_Time = ((Setup.TareDelay * ((unsigned long)62500))/10);//used to convert in sec
//    voltage = (VoltageCalib.REFvolt2-VoltageCalib.REFvolt1);    
}

unsigned int Volt_to_count_Conv(unsigned int Entered_Volt,unsigned int Ref2Count,unsigned int Ref1Count)
{
    Difference_Volt= abs(VoltageCalib.REFvolt2 - VoltageCalib.REFvolt1);
    Difference_Count = abs(Ref2Count - Ref1Count);
    final_count = (unsigned int)(((float)((float)Difference_Count/(float)Difference_Volt)) * Entered_Volt);  
//    sprintf(LcdDispBuff+16,"1:%u:%u",VoltageCalib.REFvolt2,VoltageCalib.REFvolt1);    
//    LcdDisplay(BOTTOM,LcdDispBuff+16);
//    Delay_1sec(10);
//    sprintf(LcdDispBuff+16,"2:%u:%u:%u",Difference_Volt,Difference_Count,Entered_Volt);    
//    LcdDisplay(BOTTOM,LcdDispBuff+16);
//    Delay_1sec(10);
    return(final_count);
}
/*
void Print_parameters(void)
{
    sprintf(PrintBuff,"\r\n1:%ld:%ld:%ld",Loadcell[CH1].Zero,Loadcell[CH1].Zero1,Loadcell[CH1].Span);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\n2:%ld:%ld:%ld",Loadcell[CH2].Zero,Loadcell[CH2].Zero1,Loadcell[CH2].Span);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\n3:%ld:%ld:%ld",Loadcell[CH3].Zero,Loadcell[CH3].Zero1,Loadcell[CH3].Span);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\n4:%ld:%ld:%ld",Loadcell[CH4].Zero,Loadcell[CH4].Zero1,Loadcell[CH4].Span);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nAD1:%d:%d",Axle[Axle1].CH1_Difference,Axle[Axle1].CH2_Difference);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nAD2:%d:%d",Axle[Axle2].CH1_Difference,Axle[Axle2].CH2_Difference);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nR1R2:%lu:%lu",VoltageCalib.REFvolt1,VoltageCalib.REFvolt2);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);    
    sprintf(PrintBuff,"\r\nV1:%d:%d",VoltageCalib.Ch1Ref2,VoltageCalib.Ch1Ref1);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nV2:%d:%d",VoltageCalib.Ch2Ref2,VoltageCalib.Ch2Ref1);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nV3:%d:%d",VoltageCalib.Ch3Ref2,VoltageCalib.Ch3Ref1);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nV4:%d:%d",VoltageCalib.Ch4Ref2,VoltageCalib.Ch4Ref1);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nAZ1:%d:%d",Axle[Axle1].Zero1,Axle[Axle1].Span);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
    sprintf(PrintBuff,"\r\nAZ2:%d:%d",Axle[Axle2].Zero1,Axle[Axle2].Span);
    Com1StringSend(PrintBuff,40);
    memset(PrintBuff,' ',40);
//    Print_CRLF(1);        
}
*/
