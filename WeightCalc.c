//#include "WeightCalc.h"
//#include "Menu.h"
//#include "Application.h"
#include<stdio.h>
#include<math.h>
#define NO_OF_CH            6
#define ADC_Average_Count   10

unsigned int AdcBuff[NO_OF_CH][ADC_Average_Count];
unsigned int AdcCounter[NO_OF_CH];
extern long RetAdc;
long GetAdc(char Channel);
extern unsigned long ADCSampleVal;
long AverageADC(char Channel);
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

#define CH1 0
#define CH2 1
#define CH3 2
#define CH4 3
#define CH5 4
#define CH6 5

#define MaxAdcCount	4050
#define MinAdcCount	  20

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

extern unsigned int DecimalPoint;
extern long MaxCapacity;
extern unsigned char Resolution;
extern unsigned char resdata[7];
long GetAdc(char Channel)   // Channels are re-configured as per R&D instruction on 29/05/2019
{
    if(Channel == 0)
        ReadADCCounts(12);
    else if(Channel == 1)
        ReadADCCounts(13);
    else if(Channel == 2)
        ReadADCCounts(8);
    else if(Channel == 3)
        ReadADCCounts(4);   // In Ver2.1 parameter value from 9 to 4
    else if(Channel == 4)
       ReadADCCounts(10);
    else if(Channel == 5)
       ReadADCCounts(11);
//    if(Channel == 0)
//       ReadADCCounts(8);
//    else if(Channel == 1)   // In Ver2.1 parameter value from 9 to 4
//       ReadADCCounts(4);
//    else if(Channel == 2)
//       ReadADCCounts(10);
//    else if(Channel == 3)
//       ReadADCCounts(11);
//    else if(Channel == 4)
//       ReadADCCounts(12);
//    else if(Channel == 5)
//       ReadADCCounts(13);
    AdcBuff[Channel][AdcCounter[Channel]++] = ADCSampleVal;//AN1ADCSampleVal
    if(AdcCounter[Channel] >= ADC_Average_Count)
      AdcCounter[Channel] = 0; 
	RetAdc = AverageADC(Channel);//passing value is dummy variable not used in averageADC fucniton.
//	RetAdc = BandAverage(RetAdc,Channel);
	return(RetAdc);
}

long AverageADC(char Channel)
{
	unsigned int TempVar = 0;
	unsigned long AdcVal = 0;
	for(TempVar=0;TempVar<ADC_Average_Count;TempVar++)
	{
        AdcVal += AdcBuff[Channel][TempVar];
        
	}
    AdcVal /= ADC_Average_Count;
	return(AdcVal);
}
/*
long BandAverage(long AdcVal,char Channel)
{
	if(labs(AdcVal - TempAdc[Channel]) <= 10)   //don't remove
	{
		if(AvgCnt[Channel]==1)
			AvgAdc51[Channel]=AdcVal;
		if(AvgCnt[Channel]==2)
			AvgAdc52[Channel]=AdcVal;
		if(AvgCnt[Channel]==3)
			AvgAdc53[Channel]=AdcVal;
		if(AvgCnt[Channel]==4)
			AvgAdc54[Channel]=AdcVal;
		if(AvgCnt[Channel]==5)
			AvgAdc55[Channel]=AdcVal;
		if(AvgCnt[Channel]==6)
			AvgAdc56[Channel]=AdcVal;
		if(AvgCnt[Channel]==7)
			AvgAdc57[Channel]=AdcVal;
		if(AvgCnt[Channel]==8)
			AvgAdc58[Channel]=AdcVal;
		if(AvgCnt[Channel]==9)
			AvgAdc59[Channel]=AdcVal;
		if(AvgCnt[Channel]==10)
			AvgAdc60[Channel]=AdcVal;
	
		AvgCnt[Channel]++;
		if(AvgCnt[Channel]>=11)
			AvgCnt[Channel]=1;
	
		AdcVal=AvgAdc51[Channel]+AvgAdc52[Channel]+AvgAdc53[Channel]+AvgAdc54[Channel]+AvgAdc55[Channel]+AvgAdc56[Channel]+AvgAdc57[Channel]+AvgAdc58[Channel]+AvgAdc59[Channel]+AvgAdc60[Channel];
		AdcVal /= 10;
	}
	else
	{
		AvgCnt[Channel]=1;
		AvgAdc51[Channel]=AvgAdc52[Channel]=AvgAdc53[Channel]=AvgAdc54[Channel]=AvgAdc55[Channel]=AvgAdc56[Channel]=AvgAdc57[Channel]=AvgAdc58[Channel]=AvgAdc59[Channel]=AvgAdc60[Channel]=AdcVal;
	}
	
	TempAdc[Channel] = AdcVal; 
	return(AdcVal);

}
*/
void CalcADC()
{
    long ReadCounts = 0;
    AllChannel.RAWADC = 0;
    if(Application_Setting.LCED[CH1] == 1)
    {
        ReadCounts = 0;        
        ReadCounts = (long)GetAdc(CH1);//ADC_GetConversion(1);
        if(ReadCounts >= MaxAdcCount)
			Bits.ORERRFlag1 = 1;
        else
            Bits.ORERRFlag1 = 0;
		if(ReadCounts <= MinAdcCount)
			Bits.URERRFlag1 = 1;
        else
            Bits.URERRFlag1 = 0;        
        Loadcell[CH1].RawADC = ReadCounts;
        if((Bits.ORERRFlag1 == 0) && (Bits.URERRFlag1 == 0))
        {
//            Loadcell[CH1].RawADC = 2200;
            AllChannel.RAWADC += Loadcell[CH1].RawADC;
        }
    }
    else
    {
        Loadcell[CH1].RawADC = 0;
        Bits.ORERRFlag1 = 0;        
        Bits.URERRFlag1 = 0;              
    }

    if(Application_Setting.LCED[CH2] == 1)
    {
        ReadCounts = 0;
        ReadCounts = (long)GetAdc(CH2);//ADC_GetConversion(0);
//        ReadCounts = ReadCounts - 500;
        if(ReadCounts >= MaxAdcCount)
			Bits.ORERRFlag2 = 1;
        else
            Bits.ORERRFlag2 = 0;        
		if(ReadCounts <= MinAdcCount)
			Bits.URERRFlag2 = 1;
        else
            Bits.URERRFlag2 = 0;      
        Loadcell[CH2].RawADC = ReadCounts;
        if((Bits.ORERRFlag2 == 0) && (Bits.URERRFlag2 == 0))
        {
//            Loadcell[CH2].RawADC = 2800;
            AllChannel.RAWADC += Loadcell[CH2].RawADC;
        }
    }
    else
    {
        Loadcell[CH2].RawADC = 0;
        Bits.ORERRFlag2 = 0;        
        Bits.URERRFlag2 = 0;              
    }    

    if(Application_Setting.LCED[CH3] == 1)
    {
        ReadCounts = 0;        
        //ReadCounts = (long)GetAdc(CH3);//ADC_GetConversion(5);
        ReadCounts = (long)GetAdc(CH3);//ADC_GetConversion(5);
//        ReadCounts = Loadcell[CH3].Span;//GetAdc(CH3);//ADC_GetConversion(5);
        if(ReadCounts >= MaxAdcCount)
			Bits.ORERRFlag3 = 1;
        else
            Bits.ORERRFlag3 = 0;        
		if(ReadCounts <= MinAdcCount)
			Bits.URERRFlag3 = 1;
        else
            Bits.URERRFlag3 = 0;      
            Loadcell[CH3].RawADC = ReadCounts;
        if((Bits.ORERRFlag3 == 0) && (Bits.URERRFlag3 == 0))
        {
            AllChannel.RAWADC += Loadcell[CH3].RawADC;
        }        
    }
    else
    {
        Loadcell[CH3].RawADC = 0;
        Bits.ORERRFlag3 = 0;        
        Bits.URERRFlag3 = 0;              
    }        

    if(Application_Setting.LCED[CH4] == 1)
    {
        ReadCounts = 0;        
        ReadCounts = (long)GetAdc(CH4);//ADC_GetConversion(4);
 //       ReadCounts = Loadcell[CH4].Span;
        if(ReadCounts >= MaxAdcCount)
			Bits.ORERRFlag4 = 1;
        else
            Bits.ORERRFlag4 = 0;        
		if(ReadCounts <= MinAdcCount)
			Bits.URERRFlag4 = 1;
        else
            Bits.URERRFlag4 = 0;      
        Loadcell[CH4].RawADC = ReadCounts;
        if((Bits.ORERRFlag4 == 0) && (Bits.URERRFlag4 == 0))
        {
            AllChannel.RAWADC += Loadcell[CH4].RawADC;
        }        
    }
    else
    {
        Loadcell[CH4].RawADC = 0;
        Bits.ORERRFlag4 = 0;        
        Bits.URERRFlag4 = 0;              
    }
    
    if(Application_Setting.LCED[CH5] == 1)
    {
        ReadCounts = (long)GetAdc(CH5);//ADC_GetConversion(4);
//        ReadCounts = Loadcell[CH5].Span;
        if(ReadCounts >= MaxAdcCount)
			Bits.ORERRFlag5 = 1;
        else
            Bits.ORERRFlag5 = 0;        
		if(ReadCounts <= MinAdcCount)
			Bits.URERRFlag5 = 1;
        else
            Bits.URERRFlag5 = 0;      
        Loadcell[CH5].RawADC = ReadCounts;
        if((Bits.ORERRFlag5 == 0) && (Bits.URERRFlag5 == 0))
        {
            AllChannel.RAWADC += Loadcell[CH5].RawADC;
        }
    }
    else
    {
        Loadcell[CH5].RawADC = 0;
        Bits.ORERRFlag5 = 0;        
        Bits.URERRFlag5 = 0;              
    }
    
    if(Application_Setting.LCED[CH6] == 1)
    {    
        ReadCounts = (long)GetAdc(CH6);//ADC_GetConversion(4);
//      ReadCounts = Loadcell[CH6].Span;
        if(ReadCounts >= MaxAdcCount)
			Bits.ORERRFlag6 = 1;
        else
            Bits.ORERRFlag6 = 0;        
		if(ReadCounts <= MinAdcCount)
			Bits.URERRFlag6 = 1;
        else
            Bits.URERRFlag6 = 0;      
        Loadcell[CH6].RawADC = ReadCounts;
        if((Bits.ORERRFlag6 == 0) && (Bits.URERRFlag6 == 0))
        {
            AllChannel.RAWADC += Loadcell[CH6].RawADC;
        }
    }
    else
    {
        Loadcell[CH6].RawADC = 0;
        Bits.ORERRFlag6 = 0;        
        Bits.URERRFlag6 = 0;              
    }    

	if((Bits.ORERRFlag1 + Bits.ORERRFlag2 + Bits.ORERRFlag3 + Bits.ORERRFlag4 + Bits.ORERRFlag5 + Bits.ORERRFlag6 +
        Bits.URERRFlag1 + Bits.URERRFlag2 + Bits.URERRFlag3 + Bits.URERRFlag4 + Bits.URERRFlag5 + Bits.URERRFlag6)  > 0)  
    {
        Bits.ERRFlag = 1;
    }
    else
        Bits.ERRFlag = 0;        
}

long RoundOff(long value,char round)
{	
	long ii;	 	
	ii = round;	
 	value = ((value+(ii/2))/ii)*ii;
	return(value);
}

void Weight_Calc(void)
{
 //   #ifdef testing
	float WeightWithDecimal = 0;
    char i=0;
    long Long_Wt = 0,Rawadc[NO_OF_CH] = {0};
    float loc_weight = 0;    
    CalcADC();
    for(i=CH1;i< NO_OF_CH;i++)	// from wheel loader 1.0.0
    {
        if(((i == CH1) && (Application_Setting.LCED[CH1] == 1)) ||
                ((i == CH2) && (Application_Setting.LCED[CH2] == 1)) ||
                    ((i == CH3) && (Application_Setting.LCED[CH3] == 1)) ||
                        ((i == CH4) && (Application_Setting.LCED[CH4] == 1)) ||
                            ((i == CH5) && (Application_Setting.LCED[CH5] == 1)) ||
                                ((i == CH6) && (Application_Setting.LCED[CH6] == 1)))
        {
            WeightWithDecimal = 0.0;
            WeightWithDecimal = (float)((float)(float)(Loadcell[i].CalCapacity) / ((float)labs(Loadcell[i].Span - Loadcell[i].Zero1)));//(float)(Setup.Channel1+Setup.Channel2+Setup.Channel3+Setup.Channel4)		//NO_OF_LOADCELL
            WeightWithDecimal = (float)((float)(WeightWithDecimal)*(float)labs(Loadcell[i].RawADC - Loadcell[i].Zero));				

            if(((Bits.ORERRFlag1 == 1 || Bits.URERRFlag1 == 1) && (i == CH1)) ||
                    ((Bits.ORERRFlag2 == 1 || Bits.URERRFlag2 == 1) && (i == CH2)) ||
                        ((Bits.ORERRFlag3 == 1 || Bits.URERRFlag3 == 1) && (i == CH3)) ||
                            ((Bits.ORERRFlag4 == 1 || Bits.URERRFlag4 == 1) && (i == CH4)) ||
                                ((Bits.ORERRFlag5 == 1 || Bits.URERRFlag5 == 1) && (i == CH5)) ||
                                    ((Bits.ORERRFlag6 == 1 || Bits.URERRFlag6 == 1) && (i == CH6)))
            {
                Loadcell[i].CurWeight = 65535;//For sending FFFF to the CAN Bus when the loadcell is in error
            }
            else
            {
                if((signed int)(Loadcell[i].Span - Loadcell[i].Zero1) < 0)
                {
                    if(Loadcell[i].RawADC > Loadcell[i].Zero)   
                        Loadcell[i].CurWeight = WeightWithDecimal * (-1);
                    else
                        Loadcell[i].CurWeight = WeightWithDecimal;   
                }
                else if((signed int)(Loadcell[i].Span - Loadcell[i].Zero1) > 0)
                {
                    if(Loadcell[i].RawADC < Loadcell[i].Zero)   
                        Loadcell[i].CurWeight = WeightWithDecimal * (-1);
                    else
                        Loadcell[i].CurWeight = WeightWithDecimal;                  
                }
                //Autozero(i);    

                if(Loadcell[i].CurWeight > (MaxCapacity+(9 * resdata[Resolution-1])))
                {
                    if(i == 0)
                        Bits.OCERRFlag1 = 1;
                    else if(i == 1)
                        Bits.OCERRFlag2 = 1;
                    else if(i == 2)
                        Bits.OCERRFlag3 = 1;
                    else if(i == 3)
                        Bits.OCERRFlag4 = 1;
                    else if(i == 4)
                        Bits.OCERRFlag5 = 1;
                    else if(i == 5)
                        Bits.OCERRFlag6 = 1;
                }
                else
                {
                    if(i == 0)
                        Bits.OCERRFlag1 = 0;
                    else if(i == 1)
                        Bits.OCERRFlag2 = 0;
                    else if(i == 2)
                        Bits.OCERRFlag3 = 0;
                    else if(i == 3)
                        Bits.OCERRFlag4 = 0;     
                    else if(i == 4)
                        Bits.OCERRFlag5 = 0;     
                    else if(i == 5)
                        Bits.OCERRFlag6 = 0;     
                }
            }
        }
    }
    loc_weight = 0;
    if((Application_Setting.LCED[CH1] == 1) || (Application_Setting.LCED[CH2] == 1))
    {
        if(Application_Setting.LCED[CH1] != 1)
            Loadcell[CH1].CurWeight = 0;//assigned for protocol dump
        if(Application_Setting.LCED[CH2] != 1)
            Loadcell[CH2].CurWeight = 0;//assigned for protocol dump
    }
    if((Application_Setting.LCED[CH3] == 1) || (Application_Setting.LCED[CH4] == 1))
    {
        if(Application_Setting.LCED[CH3] == 1)
           loc_weight = loc_weight + Loadcell[CH3].CurWeight;
        else
           Loadcell[CH3].CurWeight = 0;//assigned for protocol dump
        if(Application_Setting.LCED[CH4] == 1)
           loc_weight = loc_weight + Loadcell[CH4].CurWeight;
        else
            Loadcell[CH4].CurWeight = 0;//assigned for protocol dump
    //  AllChannel.CurWeight = AllChannel.CurWeight + Axle[Axle1].CurWeight;
    }
    if((Application_Setting.LCED[CH5] == 1) || (Application_Setting.LCED[CH6] == 1))
    {
        if(Application_Setting.LCED[CH5] == 1)
           loc_weight = loc_weight + Loadcell[CH5].CurWeight;
        else
            Loadcell[CH5].CurWeight = 0;//assigned for protocol dump

        if(Application_Setting.LCED[CH6] == 1)
           loc_weight = loc_weight + Loadcell[CH6].CurWeight;
        else
            Loadcell[CH6].CurWeight = 0;//assigned for protocol dump
    //  AllChannel.CurWeight = AllChannel.CurWeight + Axle[Axle1].CurWeight;
    }

    loc_weight = ((float)(loc_weight * ((float)(Application_Setting.Corr_Factor)/1000.0)));
    Long_Wt = (long)loc_weight * 100;
    Long_Wt = RoundOff(Long_Wt,100);
    Long_Wt = (Long_Wt/100);
    AllChannel.CurWeight = RoundOff(Long_Wt,resdata[Resolution-1]);         
    if(AllChannel.CurWeight > (MaxCapacity+(9 * resdata[Resolution-1])))    
        Bits.OCERRFlag1 = 1;
    else
        Bits.OCERRFlag1 = 0;    
     
//    if(AllChannel.CurWeight < 0)
//    {
//        AllChannel.Polarity = 1;
//        AllChannel.CurWeight = labs(AllChannel.CurWeight);
//    }
//    else
//        AllChannel.Polarity = 0;    

//    if(AllChannel.CurWeight > Setup.OverWeight)
//    {
//        OP1_ON Nop();
//    }
//    else
//    {
//        OP1_OFF Nop();        
//    }                
//#endif
}



