/***********************************************************************************************
 File Name          :   FIFO_Storage.c
 Created date       :   17-08-2018.   
 Last Modified Date :   17-08-2018.   
 Author             :   Iyyappan R.
 Description        :   This File is used to store and read the records in FIFO format.
 
 Available Functions:    
 * FIFO_Rec_Count_read  -   Read record related variables from EEPROM.
 * FIFO_RecordReset     -   To reset the record related variables in EEPROM Memory.
 * FIFO_Rec_Storage     -   To store a record in EEPROM memory.
 * FIFO_Rec_Retrival    -   To get the record number based on available records(This function reads all records).
 * Record_Print         -   To print the record through RS232.
 
************************************************************************************************/

//Change this structure members as per your project requirement
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
}RecordStore;//Dont change this structure name.This name used in some functions of this file

#define MAXRECORDS		 	10 	
#define MAXRECORDS_WARNING 	5
#define MAX_TRIPCOUNT	 	30
#define SIZE_REC            (sizeof(RecordStore))

//EEPROM Storage locations for Record based variables
#define EER_RecordDispCounter   3000
#define EER_RecordStoreCounter  3002
#define EER_RecOverWrite_Flag   3004

#define EEPROM_REC_STORING_ADD  3200        //Records will be stored from this location onwards in EEPROM

static unsigned int RecordDispCounter = 0;  
static unsigned int RecordStoreCounter = 0;
static unsigned char RecOverWrite_Flag = 0; //This flag will set to 1 after RecordStoreCounter above MAXRECORDS
static unsigned int Ser_No = 1;             //To print the serial number in report.

#define TOP      1
#define BOTTOM   2

void Rec_Count_read(void);
void FIFO_RecordReset(void);
void FIFO_Rec_Storage(unsigned int * TotalCount,unsigned char * OverFlow_Flag_status,unsigned int * StoreCount);
void FIFO_Rec_Retrival(void);
void Record_Print(unsigned int Rec_No);

extern void Read_E2(char* Data,const unsigned long Address,const int NoofBytes);
extern void Write_E2(const unsigned long Address,char* Data,const int NoofBytes);

/******************************************************************************
 Function Name      :   void Rec_Count_read(void)
 Created date       :   17-08-2018   
 Last Modified Date :   17-08-2018   
 Author             :   Iyyappan R
 * Arguments        :   None
 * Return           :   None
 * Description      :
 *      This function used to read the required record related variables from EEPROM.
 *      Call this function in power on.
 ******************************************************************************/

void Rec_Count_read(void)
{
    Read_E2((char*)&RecordDispCounter,EER_RecordDispCounter,sizeof(RecordDispCounter));       
    if(RecordDispCounter >= 10000)
    {
        RecordDispCounter = 0;
        Write_E2(EER_RecordDispCounter,(char*)&RecordDispCounter,sizeof(RecordDispCounter));   
    }
    Read_E2((char*)&RecordStoreCounter,EER_RecordStoreCounter,sizeof(RecordStoreCounter));       
    if(RecordStoreCounter >= 3000)
    {
        RecordStoreCounter = 0;
        Write_E2(EER_RecordStoreCounter,(char*)&RecordStoreCounter,sizeof(RecordStoreCounter));   
    }
    Read_E2((char*)&RecOverWrite_Flag,EER_RecOverWrite_Flag,sizeof(RecOverWrite_Flag));       
    if(RecOverWrite_Flag > 1)
    {
        RecOverWrite_Flag = 0;
        Write_E2(EER_RecOverWrite_Flag,(char*)&RecOverWrite_Flag,sizeof(RecOverWrite_Flag));          
    }
}

/******************************************************************************
 Function Name      :   void FIFO_RecordReset(void)
 Created date       :   17-08-2018   
 Last Modified Date :   17-08-2018   
 Author             :   Iyyappan R
 * Arguments        :   None
 * Return           :   None
 * Description      :
 *      This function used to initialize the record related variables in EEPROM as well as RAM.
 *      Call this function where you have the record reset.
 ******************************************************************************/

void FIFO_RecordReset(void)
{
    RecordDispCounter = 0;
    RecOverWrite_Flag = 0;
    RecordStoreCounter = 0;
    Write_E2(EER_RecordStoreCounter,(char*)&RecordStoreCounter,sizeof(RecordStoreCounter));   
    Write_E2(EER_RecordDispCounter,(char*)&RecordDispCounter,sizeof(RecordDispCounter));      
    Write_E2(EER_RecOverWrite_Flag,(char*)&RecOverWrite_Flag,sizeof(RecOverWrite_Flag));      
}

/******************************************************************************
 Function Name      :   void FIFO_Rec_Storage(unsigned int * TotalCount,unsigned char * OverFlow_Flag_status,unsigned int * StoreCount)
 Created date       :   17-08-2018   
 Last Modified Date :   17-08-2018   
 Author             :   Iyyappan R
 Arguments          :   TotalCount              -   Address of the Total record variable
                        OverFlow_Flag_status    -   Address of the Overflow flag variable
 *                      StoreCount              -   Address of the record store variable
 * Return           :   None
 * Description      :
 *      This function used to initialize the record related variables in EEPROM as well as RAM.
 *      Call this function after assigning the data to the Record store structure variable.It will store the data in FIFO format.
 ******************************************************************************/

void FIFO_Rec_Storage(unsigned int * TotalCount,unsigned char * OverFlow_Flag_status,unsigned int * StoreCount)
{
    unsigned long TempRecNo = 0;
    TempRecNo = RecordStoreCounter;
    RecordStoreCounter++;
    if(RecordStoreCounter >= MAXRECORDS && RecOverWrite_Flag == 0) 
    {
        RecOverWrite_Flag = 1;
        Write_E2(EER_RecOverWrite_Flag,(char*)&RecOverWrite_Flag,sizeof(RecOverWrite_Flag));             
    }
    if(RecordStoreCounter >= MAXRECORDS)
    {
        RecordStoreCounter = 0;
    }
    RecordDispCounter++;
    if(RecordDispCounter > MAX_TRIPCOUNT)
    {
        RecordDispCounter = 1;
    }    

    Write_E2(EER_RecordStoreCounter,(char*)&RecordStoreCounter,sizeof(RecordStoreCounter));   
    Write_E2(EER_RecordDispCounter,(char*)&RecordDispCounter,sizeof(RecordDispCounter));   
  	Write_E2((unsigned long)(EEPROM_REC_STORING_ADD+((TempRecNo) * (unsigned long)SIZE_REC)),(char*)&RecordStore.Date,(int)SIZE_REC);	
	Delay_ms(10);
    (*(OverFlow_Flag_status)) = RecOverWrite_Flag;
    (*(TotalCount)) = RecordDispCounter;
    (*(StoreCount)) = RecordStoreCounter;
}

/******************************************************************************
 Function Name      :   void FIFO_Rec_Retrival(void)
 Created date       :   17-08-2018   
 Last Modified Date :   17-08-2018   
 Author             :   Iyyappan R
 * Arguments        :   None
 * Return           :   None
 * Description      :
 *      This function used to retrive the records in the FIFO format.
 *      Call this function where you want to print the records.
 ******************************************************************************/

void FIFO_Rec_Retrival(void)
{
    int TripNo = 0;
    Ser_No = 1;
    if((RecordStoreCounter != 0) || (RecOverWrite_Flag == 1))
    {
        if(RecOverWrite_Flag)
        {
            for(TripNo = (RecordStoreCounter-1);TripNo >= (int)0;TripNo--)
            {
                Record_Print(TripNo);
            }
            for(TripNo = (MAXRECORDS-1);TripNo >= (int)RecordStoreCounter;TripNo--)
            {
                Record_Print(TripNo);
            }
        }
        else
        {
            for(TripNo = (RecordStoreCounter-1);TripNo >= (int)0;TripNo--)
            {
                Record_Print(TripNo);
            }
        }
    }
}

/******************************************************************************
 Function Name      :   void Record_Print(unsigned int Rec_No)
 Created date       :   17-08-2018   
 Last Modified Date :   17-08-2018   
 Author             :   Iyyappan R
 * Arguments        :   None
 * Return           :   None
 * Description      :
 *      This function used to read the Specified record from EEPROM and print it in COM(RS232) Port.
 *      Call this function where you want to print the records.
 ******************************************************************************/

void Record_Print(unsigned int Rec_No)
{
    char FIFO_Record_Buff[50]={0};
    PrintCRLF(1,1);           
    Read_E2((char*)&RecordStore.Date,(unsigned long)(EEPROM_REC_STORING_ADD+((Rec_No) * (unsigned long)SIZE_REC)),(int)SIZE_REC);Delay_ms(10);
    sprintf(FIFO_Record_Buff+1,"%4d",Ser_No++);
    memcpy(FIFO_Record_Buff+5,"   ",3);
    sprintf(FIFO_Record_Buff+8,"%02u/%02u/%4d",RecordStore.Date,RecordStore.Month,RecordStore.Year);
    memcpy(FIFO_Record_Buff+18,"   ",3);
    sprintf(FIFO_Record_Buff+21,"%02u:%02u",RecordStore.Hour,RecordStore.Minute);
    memcpy(FIFO_Record_Buff+26,"  ",2);
    sprintf(FIFO_Record_Buff+28,"%06ld",RecordStore.Weight);       
    Com2StringSend(FIFO_Record_Buff,50);
}


void FIFO_Rec_Retrival_Key(int * Current_Count)
{
    if(((*(Current_Count)) != 0) || (RecOverWrite_Flag == 1))
    {
        (*(Current_Count)) = (*(Current_Count))-1;
        Read_E2((char*)&RecordStore.Date,(unsigned long)(EEPROM_REC_STORING_ADD+(((*(Current_Count))) * (unsigned long)SIZE_REC)),(int)SIZE_REC);Delay_ms(10);
        if((*(Current_Count)) == 0)
        {
            if(RecOverWrite_Flag == 1) 
                (*(Current_Count)) = (MAXRECORDS-1);
            else
                (*(Current_Count)) = RecordStoreCounter;
        }
    }
}

/******************************   End of File *********************************/
