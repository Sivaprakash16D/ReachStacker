#include "p30Fxxxx.h"
//#include "Includes.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#define	RTSCHECK_10_VINC_RTS    		      _RB4//(From Payload-Tripper counter RTS pin changed to RB4 from RB9 - 13-04-2018) //same as prev // (input)		To check the writing or not		
#define	RTSCHECK_10_VINC_RTS    		      _RB9//(From Payload-Tripper counter RTS pin changed to RB9 from RB4 - 08-05-2019) //same as prev // (input)		To check the writing or not		
#define	USBAVAIL_11_LD1_VINC_IO8              _RD10 //(Prev-RB14)  // (input)		To check the usb available or not (input)
#define	RESET_9_VINC_RESET                    _RD0  //(Prev-RB15)  // (Output)    To set the vinculum controller after USB detected
#define TRANSFERCOMPLETE_19_VINC_IO11         _RD11 //             // (input)     To check the data transfer complete or not

#define COM_1       0
#define COM_2       1

#define TOP 1
#define BOTTOM 2

#define EEPROM_STORING_ADD  5000

#define DATE_WISE       1
#define SERIAL_NO_WISE  2

#define MS_EXCEL        1
#define TEXT            2
#define MS_WORD         3

char Dateserial_Entry(char date_serialNo);
void PrintReport_USB(char date_serialNo);
unsigned char selectformat=0;
int usbconnected=0;
int usbremoved=0;
int datatransfercomplete=0;
unsigned int CumTrips=0;
char Transfer_Complete_Check(void);
void WaitDisplay(void);
unsigned long CurDate = 0;
void Header_unitprint(char uartselect,char date_serialNo,char ReportType);
void USB_RTS_Check(void);
long TotalRecords=0;
char StartDate[7] = {0},EndDate[7] = {0};
unsigned long End = 0;
unsigned long From = 0;
char USBBuff[130]={0};
extern void PrintCRLF(char UartNo,unsigned int count);
extern void PrintChar(char UartNo,unsigned char Schar, unsigned int count);
void CurDate_assign(void);
unsigned long Record_Send_Function(char ComNo,unsigned int TripNo,char date_serialNo,char excel_other);
void Record_Read_Function(char TripNo,char date_serialNo);
long Cum_Wt=0;
unsigned long Ser_no = 0;
extern void DelayMicro(unsigned long value);
extern unsigned char Unit;
//Project related Variables
extern struct
{
	unsigned long DeviceID;	 					
	unsigned long Password;
	unsigned long BuadRate;
    unsigned long CAN_BuadRate;
}System_setting;

#define NO_OF_CH            6

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

extern unsigned char MenuKey;
extern unsigned char EnterKey;
extern unsigned char UpKey;
extern unsigned char LeftKey;		
extern unsigned char RecOverWrite;
#define MAXRECORDS 3000
extern unsigned char RecordCount;
extern char LcdDispBuff[40];
 char TempDate[8]={0};
extern unsigned int DecimalPoint;
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
	int RAWADC;
	long CurWeight;	
	long LatchedWt;    
	int TripCount;	
	unsigned long CumulativeWeight;   
    char Polarity; 
}AllChannel;
unsigned char Selection_Fun(char * FirstLineData,char * SecondLineData);
extern char GetData(char* buff,char x,char mode,char flag,long minval,long maxval,char dept);

#define COM_GPRS_MODULE     1
#define COM_PRINTER         2
#define COM_DUMP            3
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

void USB_Print_Check(void)
{
    char PrintBuff[50] = {0};	
    memcpy(PrintBuff,"%TESTF",6);		
    sprintf(PrintBuff+6,"%02lu",System_setting.DeviceID);
    PrintBuff[8] = '2';    //1-excel,2-text,3-word		
    Com1StringSend(PrintBuff,10);//(PrintBuff,10);
    PrintCRLF(COM_1,1) ;
    PrintChar(COM_1,'-',80);		
//		Header_unitprint(1,1);
    memcpy(PrintBuff,"\r\n Test File created\r\n",22);			
    Com1StringSend(PrintBuff,22);//(PrintBuff,10);		
    PrintChar(COM_1,'-',80);				
    LcdDisplay(BOTTOM," Transfer over  ");
//    LedDisplay(" done ",0,0);
    Delay1_1s(2);
}

char Transfer_Complete_Check(void) 
{
    datatransfercomplete=1;
    WaitDisplay();
    if(USBAVAIL_11_LD1_VINC_IO8==0)
    {
        //LedDisplay(" Fail ",0,0);
        LcdDisplay(TOP,"                ");        
        LcdDisplay(BOTTOM," COPYING FAILED ");
        Delay1_1s(1);//
        return(0);
    }
    else
        return(1);
}


void WaitDisplay(void)
{
    //LedDisplay(" Wait ",0,0);//
    LcdDisplay(BOTTOM,"    WAIT.       ");								
    Delay1_1s(1);
    //LedDisplay("      ",0,0);//
    LcdDisplay(BOTTOM,"    WAIT...     ");								
    Delay1_1s(1);
}

void USB_RTS_Check(void)
{
	if(USBAVAIL_11_LD1_VINC_IO8==0)
	{
		//LedDisplay(" Fail ",0,0);//
        LcdDisplay(TOP,"                ");        
        LcdDisplay(BOTTOM," COPYING FAILED ");		
        Delay1_1s(1);	
		return;
	}
	while(RTSCHECK_10_VINC_RTS==1)
	{
//		ReadRTC_DateTime();	//To update the Run time
//		RunTimeUpdate();			
		WaitDisplay();
		if(USBAVAIL_11_LD1_VINC_IO8==0)
		{
			//LedDisplay(" Fail ",0,0);
            LcdDisplay(TOP,"                ");            
            LcdDisplay(BOTTOM," COPYING FAILED ");								//									LcdDisplay(TOP,LcdDispBuff);
            Delay1_1s(1);//				//									LcdDisplay(TOP,LcdDispBuff);
			return;
		}
	}
}

void USB_Check(void)
{
    if((USBAVAIL_11_LD1_VINC_IO8==1) && (usbconnected==0))
    {
        usbconnected=1;
        usbremoved=0;
        //LedDisplay("USBcon",0,0);//
        LcdDisplay(TOP,"                ");        
        LcdDisplay(BOTTOM,"  USB DETECTED  ");	
        RESET_9_VINC_RESET = 0; Nop();
        Delay1_1s(1);
        RESET_9_VINC_RESET = 1; Nop();
        Delay1_1s(1);
    }
    if((USBAVAIL_11_LD1_VINC_IO8==0) && (usbremoved==0) && (usbconnected==1))	
    {
        usbconnected=0;
        usbremoved=1;
        //LedDisplay("USBDis",0,0);//
        LcdDisplay(TOP,"                ");        
        LcdDisplay(BOTTOM,"  USB REMOVED   ");							
        Delay1_1s(2);
	}
}

/*
 * Function Name    :   Header_unitprint
 * Argument         :   
 *                      uartselect      -   either com-1 or com-2
 *                      date_serialNo   -   TO print date or serial from to(THis argument is not used)
 *                      ReportType      -   Excel(0),other(word,Txt,RS232 protocol dump)
 * Return           :   None
 * Explanation      :
 *       This function used to send the header,from to date,category of report to the selected COM port.
 */
void Header_unitprint(char uartselect,char date_serialNo,char ReportType)
{
    char wastebuff[30]={0};    
    if(ReportType == 0)
    {
        sprintf(USBBuff,"%c",0x09);
        memcpy(USBBuff+1,"FIXED BODY TRUCK WEIGHING",25);	
        Com1StringSend(USBBuff,30);//UART1_send(USBBuff,25);
        PrintCRLF(COM_1,1);
        memset(USBBuff,' ',130);				
        memcpy(USBBuff,"DATE:",5);
        sprintf(USBBuff+5,"%c",0x09);
        sprintf(USBBuff+6,"%02d/%02d/%04d ",RTC.Date,RTC.Month,RTC.Year);
        sprintf(USBBuff+16,"%c",0x09);
        memcpy(USBBuff+17,"TIME:",5);
        sprintf(USBBuff+22,"%c",0x09);
        sprintf(USBBuff+23,"%02d:%02d",RTC.Hour,RTC.Minute);
        sprintf(USBBuff+28,"%c",0x09);		
        Com1StringSend(USBBuff,29);	
        PrintCRLF(COM_1,1);
        memset(USBBuff,' ',130);
        if(date_serialNo!=2)	//date wise	
        {
            memcpy(USBBuff,"REPORT FROM:",12);
            sprintf(USBBuff+12,"%c",0x09);
            memcpy(wastebuff,StartDate,2);	
            memset(wastebuff+2,'/',1);
            memcpy(wastebuff+3,StartDate+2,2);
            memset(wastebuff+5,'/',1);
            memcpy(wastebuff+6,"20",2);
            memcpy(wastebuff+8,StartDate+4,2);
            memcpy(USBBuff+13,wastebuff,10);	
            sprintf(USBBuff+23,"%c",0x09);
            memcpy(USBBuff+24,"TO:",3);
            sprintf(USBBuff+27,"%c",0x09);
            memcpy(wastebuff,EndDate,2);	
            memset(wastebuff+2,'/',1);
            memcpy(wastebuff+3,EndDate+2,2);
            memset(wastebuff+5,'/',1);
            memcpy(wastebuff+6,"20",2);
            memcpy(wastebuff+8,EndDate+4,2);				
            memcpy(USBBuff+28,wastebuff,10);
        }
        else 
        {
            memcpy(USBBuff,"SERIAL NUMBER FROM:",19);
            sprintf(USBBuff+19,"%1c",0x09);						
            sprintf(USBBuff+20,"%4ld",From);
            sprintf(USBBuff+24,"%c",0x09);
            memcpy(USBBuff+25,"TO:",3);
            sprintf(USBBuff+28,"%c",0x09);
            sprintf(USBBuff+29,"%4ld",End);	
        }
        Com1StringSend(USBBuff,40);		
        PrintCRLF(COM_1,1);
        PrintChar(COM_1,'-',60);           
        memset(USBBuff,' ',130);
        memcpy(USBBuff,"S.NO",4);
        sprintf(USBBuff+4,"%c",0x09);                
        memcpy(USBBuff+5,"DATE",4);
        sprintf(USBBuff+9,"%c",0x09);
        memcpy(USBBuff+10,"TIME",4);
        sprintf(USBBuff+14,"%c",0x09);
        memcpy(USBBuff+15,"T.Wt",4);
        if((Unit-1) == 0)
            memcpy(USBBuff+19,"(Kg)",4);
        else
            memcpy(USBBuff+19,"( T)",4);   
        PrintCRLF(COM_1,1);        
        Com1StringSend(USBBuff,60);
        PrintCRLF(COM_1,1);
        PrintChar(COM_1,'-',60);                
        PrintCRLF(COM_1,1);        
    }
    else
    {
        PrintCRLF(uartselect,1);
        if(uartselect == 0)
            Com1StringSend((char *)"       FIXED BODY TRUCK WEIGHING        ",40);    
        else
            Com2StringSend((char *)"       FIXED BODY TRUCK WEIGHING        ",40);    
        PrintCRLF(uartselect,1);
        ReadRTC_DateTime();
        sprintf(USBBuff,"  Date: %02u/%02u/%4d",RTC.Date,RTC.Month,RTC.Year);
        memcpy(USBBuff+18,"      ",6);
        sprintf(USBBuff+24,"Time: %02u:%02u:%02u  ",RTC.Hour,RTC.Minute,RTC.Second);
        if(uartselect == 0)
            Com1StringSend(USBBuff,40);
        else
            Com2StringSend(USBBuff,40);
        PrintCRLF(uartselect,1); 
 		memset(USBBuff,' ',130);        
        PrintCRLF(uartselect,1); 
  		if(date_serialNo!=2)	//date wise	
		{
            PrintChar(uartselect,' ',2);
			memcpy(USBBuff,"REPORT FROM:",15);
			memcpy(wastebuff,StartDate,2);	
			memset(wastebuff+2,'/',1);
			memcpy(wastebuff+3,StartDate+2,2);
			memset(wastebuff+5,'/',1);
            memcpy(wastebuff+6,"20",2);
            memcpy(wastebuff+8,StartDate+4,2);
			memcpy(USBBuff+12,wastebuff,10);	
			memcpy(USBBuff+22," TO:",4);
			memcpy(wastebuff,EndDate,2);	
			memset(wastebuff+2,'/',1);
			memcpy(wastebuff+3,EndDate+2,2);
			memset(wastebuff+5,'/',1);
            memcpy(wastebuff+6,"20",2);
            memcpy(wastebuff+8,EndDate+4,2);						
			memcpy(USBBuff+26,wastebuff,10);
		}
		else
		{
            PrintChar(uartselect,' ',2);
			memcpy(USBBuff,"SERIAL NUMBER FROM:",19);	
			sprintf(USBBuff+19,"%4ld",From);
			memcpy(USBBuff+25," TO:",4);
			sprintf(USBBuff+29,"%4ld",End);
		}
        if(uartselect == 0)    
            Com1StringSend(USBBuff,40);
        else
            Com2StringSend(USBBuff,40);
        PrintCRLF(uartselect,1); 
		PrintChar(uartselect,'-',40);        
        PrintCRLF(uartselect,1); 
        if(uartselect == 0)
        {
            Com1StringSend((char *)"   SNO     DATE      TIME     T.Wt",34);
            if((Unit-1) == 0)
                Com1StringSend((char *)"(Kg)",4);
            else
                Com1StringSend((char *)"( T)",4);                  
        }            
        else
        {
            Com2StringSend((char *)"   SNO     DATE      TIME     T.Wt",34);
            if((Unit-1) == 0)
                Com2StringSend((char *)"(Kg)",4);
            else
                Com2StringSend((char *)"( T)",4);                  
        }
        PrintCRLF(uartselect,1);    
		PrintChar(uartselect,'-',40);    
		PrintCRLF(uartselect,1);			
    }
}

void CurDate_assign(void)
{
    CurDate = 0;
	CurDate += (TripRecord.Year % 100);
	CurDate *= 10000;
	CurDate += TripRecord.Date;	
	CurDate += (TripRecord.Month * 100);	
}

void Record_Read_Function(char TripNo,char date_serialNo)
{
    Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
    if(date_serialNo!=2)//date wise
    {
        CurDate_assign();
        if(CurDate >= From && CurDate <= End)
        {
            CumTrips += 1;	
        }
        CurDate = 0;
    }
    else
    {
        if(TripNo >= From && TripNo <= End)
        {
            CumTrips += 1;	
        }						
    }    
}

unsigned long Record_Send_Function(char ComNo,unsigned int TripNo,char date_serialNo,char excel_other)
{
    unsigned char Tempchar = 0;
    Cum_Wt = 0;    
    Read_E2((char*)&TripRecord.Date,(unsigned long)(EEPROM_STORING_ADD+((TripNo) * (sizeof(TripRecord)))),sizeof(TripRecord));Delay_ms(10);
    if(date_serialNo!=2)
    {
            CurDate_assign();
    }
    if((((CurDate >= From) && (CurDate <= End))&& (date_serialNo!=2))||(((TripNo+1) >= From && (TripNo+1) <= End)&& (date_serialNo==2)))
    {
//        if(date_serialNo==2)	
//        {
//            sprintf(USBBuff,"%3d",TripNo);
//            ++Ser_no;
//        }
//        else
            sprintf(USBBuff+Tempchar,"%3ld",++Ser_no);		
        if(excel_other == 1)    //Excel Format
        {
            sprintf(USBBuff+(Tempchar+=3),"%c",0x09);
            sprintf(USBBuff+(Tempchar+=1),"%02d/%02d/%04d ",TripRecord.Date,TripRecord.Month,TripRecord.Year);
            sprintf(USBBuff+(Tempchar+=10),"%c",0x09);//            memset(USBBuff+(Tempchar+=8),' ',5);
            sprintf(USBBuff+(Tempchar+=1),"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
            sprintf(USBBuff+(Tempchar+=5),"%c",0x09);
            sprintf(USBBuff+(Tempchar+=1),"%06ld",TripRecord.Weight);                          
            PutDp((unsigned char*)USBBuff+Tempchar,(unsigned long)DecimalPoint,6,1);                        
            sprintf(USBBuff+(Tempchar+=7),"%c",0x09);
            if(TripRecord.OLStatus)
                memcpy(USBBuff+(Tempchar+=1),"*    ",5);
            else
                memcpy(USBBuff+(Tempchar+=1),"     ",5);           
            if(ComNo == COM_1)
            {
                Com1StringSend(USBBuff,(Tempchar+=5));
                PrintCRLF(COM_1,1);  
            }
            else if(ComNo == COM_2)
            {
                Com2StringSend(USBBuff,(Tempchar+=5));	
                PrintCRLF(COM_2,1);
            }            
        }
        else    //  Other Format Like Word,Txt,RS232
        {
            sprintf(USBBuff+1,"%4d",(TripNo+1));
            memcpy(USBBuff+5,"   ",3);
            sprintf(USBBuff+8,"%02u/%02u/%4d",TripRecord.Date,TripRecord.Month,TripRecord.Year);
            memcpy(USBBuff+18,"   ",3);
            sprintf(USBBuff+21,"%02u:%02u",TripRecord.Hour,TripRecord.Minute);
            memcpy(USBBuff+26,"  ",2);
            sprintf(USBBuff+28,"%06ld",TripRecord.Weight);                          
            PutDp((unsigned char*)USBBuff+28,(unsigned long)DecimalPoint,6,1);                        
            if(TripRecord.OLStatus)
                memcpy(USBBuff+35,"*    ",5);
            else
                memcpy(USBBuff+35,"     ",5);                      
            if(ComNo == COM_1)
            {
                Com1StringSend(USBBuff,40);
                PrintCRLF(COM_1,1);  
            }
            else if(ComNo == COM_2)
            {
                Com2StringSend(USBBuff,40);	
                PrintCRLF(COM_2,1);
            }
        }
    }
    return(Ser_no);
}

void PrintReport_USB(char date_serialNo)
{
//    char TempDate_1[7]={0};//TempDate[7]={0},
//	unsigned long ser_no = 0;
//  float Cum_Wt=0;
	unsigned int TripNo = 0; 
	char PrintBuff[40] = {0};
	unsigned char Mode = 1;
	CumTrips=0;
	CurDate = 0;
	TotalRecords=0;
    Ser_no = 0;
//	ClearLcdDisplay();	
	memset(StartDate,'0',7);
	memset(EndDate,'0',7);
	if(USBAVAIL_11_LD1_VINC_IO8==1)//To check USB available or not
    {
    //    if(date_serialNo != SERIAL_NO_WISE)
        {
            if(Dateserial_Entry(date_serialNo))
            {
                //continue the process
            }
            else
                return;
        }
		//LcdDisplay(BOTTOM,"                ");
        //LedDisplay("Format",0,0);Delay1_1s(1);//
        LcdDisplay(TOP," Select Format  ");	
		while(1)
		{
			KeySense();
			if(MenuKey)
			{
				return;
			}
			else if(UpKey)
			{
				if(Mode++ >= 3)
					Mode = 1;
			}
			else if(EnterKey)
			{
				selectformat=Mode;
				break;
			}
				if(Mode == 1)
				  //LedDisplay(" Excel",0,0);//
                    LcdDisplay(BOTTOM,"        MS-EXCEL");
				else if(Mode == 2)
				  //LedDisplay("  Text",0,0);//
                    LcdDisplay(BOTTOM,"            TEXT");
				else if(Mode == 3)
				  //LedDisplay("  Word",0,0);//
                    LcdDisplay(BOTTOM,"         MS-WORD");
		}
//		ClearLcdDisplay();            
		//LedDisplay(" Wait ",0,0);//
        LcdDisplay(BOTTOM,"       Wait...      ");

        if(date_serialNo != SERIAL_NO_WISE)
        {
            if(Bits.RecOverWrite)
            {
                for(TripNo =1;TripNo <= MAXRECORDS;TripNo++)
                {
                    Record_Read_Function(TripNo,date_serialNo);
                }
            }
            else
            {
                for(TripNo =1;TripNo <= AllChannel.TripCount;TripNo++)
                {
                    Record_Read_Function(TripNo,date_serialNo);
                }
            }
            if(CumTrips == 0)
            {
                //LedDisplay("No Rec",0,0);//
                LcdDisplay(TOP,"                ");            
                LcdDisplay(BOTTOM,"  * NO RECORD * ");				
                Delay1_1s(1);
                return;
            }
        }
//        TotalRecords = CumTrips;

    	LcdDisplay(TOP,"Copying to USB..");
    	LcdDisplay(BOTTOM,"                ");

        PrintChar(COM_1,'%',1);//Chip_UART_SendByte(LPC_USART1,'%');
        if(date_serialNo!=2)//date wise
            memcpy(PrintBuff,"DA",2);
        else
            memcpy(PrintBuff,"SE",2);					
        sprintf(PrintBuff+2,"%03lu",System_setting.DeviceID);
        sprintf(PrintBuff+5,"%02u%c",RTC.Minute,selectformat);
        Com1StringSend(PrintBuff,10);//(PrintBuff,10);
        PrintCRLF(COM_1,1);
        memset(LcdDispBuff,' ',16);
//		Chip_EEPROM_Read(EES_Header1_Name,(unsigned char *)&Header1_Name,20);//DelayMicro(2);
//		Chip_EEPROM_Read(EES_Header2_Name,(unsigned char *)&Header2_Name,20);//DelayMicro(2);

        if((selectformat==TEXT)||(selectformat==MS_WORD))	//Word or Text print
        {
            if(date_serialNo!=2)	//date wise	
                Header_unitprint(0,1,1);
            else 
                Header_unitprint(0,2,1);
            if(Bits.RecOverWrite)
            {
                for(TripNo=0;TripNo < AllChannel.TripCount;TripNo--)
                {
                    memset(LcdDispBuff,' ',16);
                    sprintf(LcdDispBuff,"     %4ld      " ,(Record_Send_Function(COM_1,TripNo,date_serialNo,0)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
                    //LedDisplay(LcdDispBuff,0,0);			    
                    LcdDisplay(BOTTOM,LcdDispBuff);
                    USB_RTS_Check();
                    CurDate = 0;						
                }                
                for(TripNo=MAXRECORDS;TripNo > AllChannel.TripCount;TripNo--)
                {
                    memset(LcdDispBuff,' ',16);
                    sprintf(LcdDispBuff,"     %4ld      " ,(Record_Send_Function(COM_1,TripNo,date_serialNo,0)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
                    //LedDisplay(LcdDispBuff,0,0);			    
                    LcdDisplay(BOTTOM,LcdDispBuff);
                    USB_RTS_Check();
                    CurDate = 0;						
                }     
            }
            else
            {
                for(TripNo=0;TripNo < AllChannel.TripCount;TripNo++)
                {
                    memset(LcdDispBuff,' ',16);
                    sprintf(LcdDispBuff,"     %4ld      " ,(Record_Send_Function(COM_1,TripNo,date_serialNo,0)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
                //    LedDisplay(LcdDispBuff,0,0);			    //
                    LcdDisplay(BOTTOM,LcdDispBuff);
                    USB_RTS_Check();
                    CurDate = 0;						
                }                           
            }
            PrintChar(COM_1,'-',40);	
            memset(USBBuff,' ',130);//assigning NULL
            PrintCRLF(COM_1,1);
        }
        else if(selectformat==MS_EXCEL)//excel format
        {
                if(date_serialNo!=2)	//date wise	
                    Header_unitprint(0,1,0);
                else 
                    Header_unitprint(0,2,0);	
                memset(USBBuff,' ',130);
                if(Bits.RecOverWrite)
                {
                    for(TripNo=AllChannel.TripCount;TripNo!=0;TripNo--)
                    {
                        sprintf(LcdDispBuff,"     %4ld      " ,(Record_Send_Function(COM_1,TripNo,date_serialNo,1)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
                        //LedDisplay(LcdDispBuff,0,0);			    //
                        LcdDisplay(BOTTOM,LcdDispBuff);
                        USB_RTS_Check();
                        CurDate = 0;						
                    }                
                    for(TripNo=MAXRECORDS;TripNo > AllChannel.TripCount;TripNo--)
                    {
                        sprintf(LcdDispBuff,"     %4ld      " ,(Record_Send_Function(COM_1,TripNo,date_serialNo,1)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
                        //LedDisplay(LcdDispBuff,0,0);			    //
                        LcdDisplay(BOTTOM,LcdDispBuff);
                        USB_RTS_Check();
                        CurDate = 0;						
                    }     
                }
                else
                {
                    for(TripNo=0;TripNo<=AllChannel.TripCount;TripNo++)
                    {
                        sprintf(LcdDispBuff,"     %4ld      " ,(Record_Send_Function(COM_1,TripNo,date_serialNo,1)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
                        //LedDisplay(LcdDispBuff,0,0);			    //
                        LcdDisplay(BOTTOM,LcdDispBuff);
                        USB_RTS_Check();
                        CurDate = 0;						
                    }
                }
                PrintChar(COM_1,'-',60);
                memset(USBBuff,0x00,130);//assigning NULL
                PrintCRLF(COM_1,1);
        }
        memset(USBBuff,' ',130);
        while(TRANSFERCOMPLETE_19_VINC_IO11==0) 
        {
            if(Transfer_Complete_Check())
            {
            }
            else
                return;
        }
        if(datatransfercomplete==1)
        {
                datatransfercomplete=0;
            	LcdDisplay(TOP,"                ");
            	LcdDisplay(BOTTOM,"COPYING COMPLETE");
     //           LedDisplay(" done ",0,0);
        }
    }
    else
    {
            LcdDisplay(TOP,"                ");
            LcdDisplay(BOTTOM,"USB NOT DETECTED");
  //        LedDisplay("No USB",0,0);
            Delay1_1s(1);
            return;
    }  
	Delay1_1s(1);//used giving delay for avoiding data lose   
}

char Dateserial_Entry(char date_serialNo)
{
    char menutmp[8];
 if(date_serialNo!= SERIAL_NO_WISE)//date wise
 {
  ReadRTC_DateTime();
	
	sprintf(StartDate,"%02u%02u%02u",RTC.Date,RTC.Month,(RTC.Year % 100));
	sprintf(EndDate,"%02u%02u%02u",RTC.Date,RTC.Month,(RTC.Year % 100));
    LcdDisplay(TOP,"   Start Date   ");//	LedDisplay("Start ",0,0);
	if(GetRTCData(3,StartDate,"010100","311299","N3N1N0"))
	{
		memcpy(TempDate,StartDate+4,2);
		memcpy(TempDate+2,StartDate+2,2);
		memcpy(TempDate+4,StartDate,2);
        TempDate[6] = '\0';
		From = atol(TempDate);
//		Temp_From = atol(TempDate);
//      LedDisplay(TempDate,0,0);Delay1_1s(1);
//      sprintf(LcdDispBuff,"%lu",From);LedDisplay(LcdDispBuff,0,0);Delay1_1s(1);
        LcdDisplay(TOP,"    End Date    ");//LedDisplay(" End  ",0,0);
		if(GetRTCData(3,EndDate,"010100","311299","N3N1N0"))
		{
			memcpy(TempDate,EndDate+4,2);
			memcpy(TempDate+2,EndDate+2,2);
			memcpy(TempDate+4,EndDate,2);
//          LedDisplay(TempDate,0,0);Delay1_1s(1);
            TempDate[6] = '\0';
            End = atol(TempDate);		
//          Temp_End = atol(TempDate);		
//          sprintf(LcdDispBuff,"%lu",End);LedDisplay(LcdDispBuff,0,0);Delay1_1s(1);
			return(1);
		}
		else
		{
			return(0);
		}
	}
	else
		return(0);
 }
 else if((AllChannel.TripCount !=0) && (date_serialNo == SERIAL_NO_WISE))
 {
		//LcdDisplay(LCD_LINE_1,"  Start Serial No   ");
		LcdDisplay(TOP,"Start Serial No ");
		From = 1;
        sprintf(menutmp,"%06lu",From);
        memset(menutmp,' ',6-4);
		if(GetData(menutmp,(char)4,(char)0,(char)1,(long)1,(long)3000,(char)0))
        {
            From = atol(menutmp);
            if(From <= AllChannel.TripCount)
            {
                LcdDisplay(TOP," End Serial No  ");
                //LcdDisplay(LCD_LINE_1,"   End Serial No    ");
                End = AllChannel.TripCount;
                sprintf(menutmp,"%06lu",End);
                memset(menutmp,' ',6-4);
                
                //LcdDisplay(LCD_LINE_1,"                    ");
                if(GetData(menutmp,(char)4,(char)0,(char)1,(long)1,(long)AllChannel.TripCount,(char)0))
                {
                    End = atol(menutmp);
                    return(1);
                }
                else
                {
                    return(0);
                }
            }
            else
                return(0); 
        }
        else
            return(0); 
        
 }
 return(0);
}

void PrintReport_Rs232(char date_serialNo)
{
	unsigned int TripNo = 0; 
    CurDate = 0;
	CumTrips=0;
	TotalRecords=0;
    Ser_no = 0;
//	ClearLcdDisplay();
	memset(StartDate,'0',7);
	memset(EndDate,'0',7);	
	
	if(Dateserial_Entry(date_serialNo))
	{
		//continue the process
	}
	else
		return;
//	Clear234LCD();
	//memcpy(LCD_Buff3,"       Wait...      ",20);
	LcdDisplay(BOTTOM,"     Wait...    ");	
    if(date_serialNo != SERIAL_NO_WISE)
    {
        if(Bits.RecOverWrite)
        {
            for(TripNo =0;TripNo < MAXRECORDS;TripNo++)
            {
                Record_Read_Function(TripNo,date_serialNo);
            }
        }
        else
        {
            for(TripNo =0;TripNo < AllChannel.TripCount;TripNo++)
            {
                Record_Read_Function(TripNo,date_serialNo);
            }            
        }

        if(CumTrips == 0)
        {
    //		Clear234LCD();
                LcdDisplay(TOP,"                ");            
                LcdDisplay(BOTTOM,"  * NO RECORD * ");
    //        LedDisplay("No Rec",0,0);
            Delay1_1s(1);
            return;
        }	
    }

	//LedDisplay("Print.",0,0);//
    LcdDisplay(BOTTOM,"  PRINTING...   ");								
	
	if(date_serialNo!=2)	//date wise	
		Header_unitprint(1,1,1);
	else 
		Header_unitprint(1,2,1);
    PrintCRLF(COM_2,1);	
    if(Bits.RecOverWrite)
    {
        for(TripNo=AllChannel.TripCount;TripNo!=0;TripNo--)
        {
                                memset(LcdDispBuff,' ',16);
            sprintf(LcdDispBuff," %3ld  " ,(Record_Send_Function(COM_2,TripNo,date_serialNo,0)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
            //LedDisplay(LcdDispBuff,0,0);			    //
            LcdDisplay(BOTTOM,LcdDispBuff);
            //USB_RTS_Check();
            CurDate = 0;						
        }                
        for(TripNo=MAXRECORDS;TripNo > AllChannel.TripCount;TripNo--)
        {
                                memset(LcdDispBuff,' ',16);
            sprintf(LcdDispBuff," %3ld  " ,(Record_Send_Function(COM_2,TripNo,date_serialNo,0)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
            //LedDisplay(LcdDispBuff,0,0);			    //
            LcdDisplay(BOTTOM,LcdDispBuff);
            //USB_RTS_Check();
            CurDate = 0;						
        }     
    }
    else
    {
        for(TripNo=0;TripNo < AllChannel.TripCount;TripNo++)
        {
                                memset(LcdDispBuff,' ',16);
            sprintf(LcdDispBuff," %3ld  " ,(Record_Send_Function(COM_2,TripNo,date_serialNo,0)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
            //LedDisplay(LcdDispBuff,0,0);			    //
            LcdDisplay(BOTTOM,LcdDispBuff);
            CurDate = 0;						
        }                           
    }
//	for(TripNo=1;TripNo<=AllChannel.TripCount;TripNo++)
//	{
//        sprintf(LcdDispBuff," %3ld  " ,(Record_Send_Function(COM_2,TripNo,date_serialNo)));		//sprintf(LCD_Buff3,"%3ld/%3d",ser_no,TotalRecords);							
//        LedDisplay(LcdDispBuff,0,0);			    //LcdDisplay(LCD_LINE_3,LCD_Buff3);
//    	Cum_Wt=0;
//		CurDate = 0;
//   }
    PrintChar(COM_2,'-',40);
    memset(USBBuff,' ',130);//assigning NULL
    LcdDisplay(BOTTOM," PRINT COMPLETED");
    //LedDisplay(" Done ",0,0);
   // DelayMicro(300);
}

void USB_Log(char *Buff,int No_of_Bytes)
{
	if(USBAVAIL_11_LD1_VINC_IO8==1)//inverse logic
	{
//		Chip_UART_SendByte(LPC_USART1,'%');
		memcpy(USBBuff,"%LOG",4);
		sprintf(USBBuff+4,"%02u%02u%c",RTC.Month,RTC.Date,2);	//In Text Format
		Com1StringSend(USBBuff,9);
//		Delay_1sec(1);		
	// Replace your Log code here
/*		sprintf(USBBuff,"%02u/%02u/%04u",RTC.Date,RTC.Month,RTC.Year);	
		sprintf(USBBuff+10,"  %02u:%02u:%02u",RTC.Hour,RTC.Minute,RTC.Second);	
		sprintf(USBBuff+20,"%c",0x09);
		sprintf(USBBuff+21,"S:%08lu",SysON_Mins);	
		sprintf(USBBuff+31,"%c",0x09);
		sprintf(USBBuff+32,"C:%02u",CurrentMin);	
		sprintf(USBBuff+36,"%c",0x09);
		sprintf(USBBuff+37,"RUN TIME:%7lu:%02u ",(SysON_Mins/60),(unsigned char)(SysON_Mins % 60));		*/
		// below one send the log data to rs232_USB
		Com1StringSend(Buff,No_of_Bytes);
		PrintCRLF(0,1);	
        //		Com1StringSend(0,USBBuff,57);
	}
}

void PrintOption()
{
    unsigned char Return_Data = 0;
	while(1)
	{
        if( (Application_Setting.ComMode == COM_GPRS_MODULE) || (Application_Setting.ComMode == COM_DUMP) )
            goto USB_Print;
        TwoLineDisplay("      PRESS     "," UP-232 LEFT-USB",0);
		KeySense();
		if(MenuKey)
		{
			break;
		}
		else if(UpKey)				// RS232 Print
		{
            Return_Data = Selection_Fun("   UP - Date    ","LEFT -Serial No ");
            if(Return_Data != 0)
                PrintReport_Rs232(Return_Data);
			break;
		}
		else if(LeftKey)			// USB Print
		{
            USB_Print:
            if(USBAVAIL_11_LD1_VINC_IO8==1)//To check USB available or not
            {
                TwoLineDisplay("   USB REPORT   ","                ",3);
                Return_Data = Selection_Fun("   UP - Date    ","LEFT -Serial No ");
                if(Return_Data != 0)
                    PrintReport_USB(Return_Data);
            }
            else
            {
                TwoLineDisplay("                ","USB NOT DETECTED",1);
      //        LedDisplay("No USB",0,0);
                Delay1_1s(1);
            }
			break;
		}
//		if(TimeOutFlag.TimeOut)
//			return;
	}
	return;
}


unsigned char Selection_Fun(char * FirstLineData,char * SecondLineData)
{
    while(1)
    {
        LcdDisplay(TOP,FirstLineData);
        LcdDisplay(BOTTOM,SecondLineData);	

        KeySense();
        if(MenuKey)
        {
            return(0);
        }
        else if(UpKey)				
        {
            return(DATE_WISE);
        }
        else if(LeftKey)			
        {
            return(SERIAL_NO_WISE);
        }		
    }
}



