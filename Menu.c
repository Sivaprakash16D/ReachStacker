#include "Menu.h"
//#include "Includes.h" - 10/08/2018
//#include "Includes.h" - 02/08/2018
//#include "Includes.h" - 01/08/2018
//#include "Includes.h" - 31/07/2018
//#include "Includes.h" -   
//#include "Includes.h"
//#include "Application.h"
void KeypadTest();
void Set_Date();
void Nop()
{}
unsigned char Axle_or_LC_calib=0;
#define Axlecalib       0
#define LCcalib         1
#define Loadcalib       1
#define Anglecalib      2
unsigned char Calib_Flag = 0;
void DoMenu(void)
{
	long lngval;
    long testing;
	int intval;
    unsigned char Selected_Option = 0;
    memset(LcdDispBuff,' ',32);
    LcdDisplay(TOP,(char *)"     *MENU*     ");
    LcdDisplay(BOTTOM,(char *)"                ");
	while(1)
	{
        strncpy(LcdDispBuff,MenuData[curindex].Name,16);		

		LcdDisplay(TOP,LcdDispBuff);
		LcdDisplay(BOTTOM,LcdDispBuff+16);
		KeySense();
        if(curindex == 6)   //Load calibration
            Calib_Flag = Loadcalib;
        if(curindex == 63)  //Angle calibration
            Calib_Flag = Anglecalib;
		if(UpKey)
		{
           Read_E2((char *)&VoltageCalib.REFvolt1,EE_REFvolt1,sizeof(VoltageCalib.REFvolt1));Delay_ms(10);   
            curindex = (unsigned char)MenuData[curindex].Next;

            if(Calib_Flag == Loadcalib)
            {
                if(curindex == 44)   //Cal Zero
                    curindex = 46;            
                if(curindex == 48)   //Cal span
                    curindex = 50;            
                if(curindex == 54)   //Calcapacity
                    curindex = 52;
            }
            else if(Calib_Flag == Anglecalib)
            {
                if(curindex == 46)   //Cal Zero
                    curindex = 44;            
                if(curindex == 50)   //Cal span
                    curindex = 48;            
                if(curindex == 52)   //Calcapacity
                    curindex = 54;   
                if(curindex == 7)   
                    curindex = 10;   
            }            
 		}
		else if(MenuKey)
		{
			curindex = (unsigned char)MenuData[curindex].Esc;
            if((curindex == 6) && (Calib_Flag == Anglecalib))
                curindex = 63;              
			if(curindex == 0)
			{
                memset(LcdDispBuff,' ',32);                                  
				break;
			}
		}
		else if(EnterKey)
		{
			if(MenuData[curindex].mnuFlags.flags.Numeric)
			{
				switch(MenuData[curindex].mnuType)
				{
					case PtoLong:
					{
                        sprintf(menutmp,"%06lu",MenuData[curindex].Value->Val.ulData);
                        memset(menutmp,' ',6-MenuData[curindex].NumDigit);                            
                        
GET_VALUE:              if(GetData(menutmp,MenuData[curindex].NumDigit,MenuData[curindex].mnuFlags.flags.isPassword,MenuData[curindex].mnuFlags.flags.Validation,*MenuData[curindex].Min,*MenuData[curindex].Max,*MenuData[curindex].NumDecimal))
                        {
							lngval = atol(menutmp);
                            
                            if(curindex == 2)   //Cal Zero
                            {
                                if(!Getvalues_mA(1,lngval))
                                    goto GET_VALUE;	
                            }
                            else if(curindex == 3)   //Cal Span
                            {
                                if(!Getvalues_mA(2,lngval))
                                    goto GET_VALUE;	
                            }
                   
                            memcpy(MenuData[curindex].Value,&lngval,4);Delay_ms(100);
                            
                            if(MenuData[curindex].mnuFlags.flags.WriteE2)		
                            {
                                Write_E2(MenuData[curindex].E2Index,(char*)&lngval,4);	
                                if(curindex == 44)
                                {
                                    Loadcell[CH1].Zero = lngval;
                                    Write_E2(ELC1Zero,(char *)&Loadcell[CH1].Zero,sizeof(Loadcell[CH1].Zero));
                                }
                                if(curindex == 45)
                                {
                                    Loadcell[CH2].Zero = lngval;
                                    Write_E2(ELC2Zero,(char *)&Loadcell[CH2].Zero,sizeof(Loadcell[CH2].Zero));
                                }
                                if(curindex == 46)
                                {
                                    Loadcell[CH3].Zero = lngval;
                                    Write_E2(ELC3Zero,(char *)&Loadcell[CH3].Zero,sizeof(Loadcell[CH3].Zero));
                                }
                                if(curindex == 47)
                                {
                                    Loadcell[CH4].Zero = lngval;
                                    Write_E2(ELC4Zero,(char *)&Loadcell[CH4].Zero,sizeof(Loadcell[CH4].Zero));                                    
                                }
                                if(curindex == 67)
                                {
                                    Loadcell[CH5].Zero = lngval;
                                    Write_E2(ELC5Zero,(char *)&Loadcell[CH5].Zero,sizeof(Loadcell[CH5].Zero));                                    
                                }
                                if(curindex == 71)
                                {
                                    Loadcell[CH6].Zero = lngval;
                                    Write_E2(ELC6Zero,(char *)&Loadcell[CH6].Zero,sizeof(Loadcell[CH6].Zero));                                    
                                }
                            }
                            if((curindex == 8) || ((curindex >= 67) && (curindex <= 73)) || ((curindex >= 44) && (curindex <= 55)))//Max capacity
                                LastCalibStore();	
                            curindex = (unsigned char)MenuData[curindex].Enter;					
                        }
						else
							curindex = (unsigned char)MenuData[curindex].Enter;
                        break;
					}
					case PtoInt:
					{
                        if(curindex == 19)
                        {
                            Selected_Option = Application_Setting.AngleEnDis;
                            if(Selected_Option > 2)
                                Selected_Option = 1;
                            Selected_Option = OptionChooseFunction("          Enable         Disable",2,Selected_Option);
                            if(Selected_Option)
                            {
                                Application_Setting.AngleEnDis = Selected_Option;
                                Write_E2(EAngleEnDis,(char*)&Application_Setting.AngleEnDis,sizeof(Application_Setting.AngleEnDis));	
                            }
                        }
                        if(((curindex == 19) && (Application_Setting.AngleEnDis == 1))||(curindex != 19))
                        {
                            sprintf(menutmp,"%06u",(MenuData[curindex].Value->Val.uiData));
                            memset(menutmp,' ',6-MenuData[curindex].NumDigit);
                            if(GetData(menutmp,MenuData[curindex].NumDigit,MenuData[curindex].mnuFlags.flags.isPassword,MenuData[curindex].mnuFlags.flags.Validation,*MenuData[curindex].Min,*MenuData[curindex].Max,*MenuData[curindex].NumDecimal))
                            {
                                intval = atoi(menutmp);
                                memcpy(MenuData[curindex].Value,(char*)&intval,2); 
                                if(MenuData[curindex].mnuFlags.flags.WriteE2)
                                    Write_E2(MenuData[curindex].E2Index,(char*)&intval,2);
                                if(curindex == 7)   //Decimal POint 
                                    LastCalibStore();
                                curindex = (unsigned char)MenuData[curindex].Enter;
                            }
                            else
                                curindex = (unsigned char)MenuData[curindex].Enter;
                        }
                        else
                            curindex = (unsigned char)MenuData[curindex].Enter;
						break;
					}
					case Combo:
					{
                        menutmp[0] = GetCombo(curindex);
						if(menutmp[0])
						{
                            if(curindex == 7)                            
                                MenuData[curindex].Value->Val.ucData = (int)menutmp[0];
							else
                                MenuData[curindex].Value->Val.ucData = menutmp[0];
			
							if(MenuData[curindex].mnuFlags.flags.WriteE2)
							{
                                Write_E2(MenuData[curindex].E2Index,(char*)menutmp,1);
                            }
                            if(curindex == 80)
                                Can_Init();
                            else if(curindex == 29)
                            {
                                Com2Init(System_setting.BuadRate);Delay_ms(300);
                            }
                            else if(curindex == 9)   //Resolution
                                LastCalibStore(); 
                            
							curindex = (unsigned char)MenuData[curindex].Enter;
						}
						else
							curindex = (unsigned char)MenuData[curindex].Enter;					

						break;
					}
					case SplFunction:
					{
						switch((int)(MenuData[curindex].Value->Val.uiData))
						{
							case 0:
							{
                                DiagnosticCounts();                                
								break;
							}
							case 1:
							{
                                if(curindex == 4)
                                    Confirmation_Function(TakeVoltBackup);                                
                                else if((curindex == 13) && (Calib_Flag == Loadcalib))
                                    Confirmation_Function(TakeLoadCalBackup);                                
                                else if((curindex == 13) && (Calib_Flag == Anglecalib)) 
                                    Confirmation_Function(TakeAngleCalBackup);                                
								break;
							}
							case 2:
							{
                                if(curindex == 58)
                                    Confirmation_Function(RestoreLoadCalBackup);                                
                                else if(curindex == 59)
                                    Confirmation_Function(RestoreVoltBackup);
								break;
							}
							case 3:
							{
                                InputLogic_Multiply_Select(InputSel);
								break;
							}
							case 4:
							{
                                Confirmation_Function(InitGenSettings);  
								break;
							}
							case 5:
							{
								PortCheck();
						 		break;
							}
							case 6:
							{
								KeypadTest();
						 		break;
							}
							case 7:
							{
								DIPCheck();
						 		break;
							}
							case 8:
							{
                                ChannelConfiguration();
						 		break;
							}
							case 9:							// Set Date
							{
								Set_Date();
						 		break;
							}
							case 10:						// Set Time
							{
								Set_Time();
						 		break;
							}
							case 11:						// RTC Initialize
							{
								RTC_Init();
						 		break;
							}
							case 12:						// Load Default
							{
                                Confirmation_Function(Loaddefault);  
 						 		break;
							}
							case 13:
							{
								DOPCheck();
								break;
							}
							case 14:
							{
								DisplayCheck();
								break;
							}
							case 15:
							{
								RTC_Check();
								break;
							}
							case 16:
							{
								Operator_Selector();
								break;
							}
							case 17:
							{
								GPRS_Reset_Function(1);
								break;
							}
							case 18:
							{
								Confirmation_Function(InitVoltValue);
								break;
							}
							case 19:
							{
								Confirmation_Function(InitVoltValue);
								break;
							}                            
							case 20:
							{
								LastCalibRecord();
								break;
							}                            
							case 21:
							{
								LasttareRecord();
								break;
							}                            
							case 22:
							{
								Confirmation_Function(InitAppSettings);
								break;
							}                            
							case 23:    //Restore 
							{
								Selected_Option = OptionChooseFunction("      LOAD           ANGLE           VOLTAGE    ",3,1);
								if(Selected_Option)
                                {
                                    if(Selected_Option == 1)    
                                        Confirmation_Function(RestoreLoadCalBackup);
                                    else if(Selected_Option == 2)    
                                        Confirmation_Function(RestoreAngCalBackup);
                                    else if(Selected_Option == 3)    
                                        Confirmation_Function(RestoreVoltBackup);
                                }
                                break;
							}
							case 24:
							{
								Confirmation_Function(InitAppSettings);
								break;
							}                            
							case 25:
							{
								GPRS_Display();
								break;
							}
							case 26:
							{
                                Selected_Option = Application_Setting.ComMode;
                                if(Selected_Option > 3)
                                    Selected_Option = 1;
                                Selected_Option = OptionChooseFunction("     GPRS MODULE         PRINTER        HMI DATA",3,Selected_Option);
                                if(Selected_Option)
                                {
                                    Application_Setting.ComMode = Selected_Option;
                                    Write_E2(EComMode,(char*)&Application_Setting.ComMode,sizeof(Application_Setting.ComMode));	
                                }
								break;
							}
 							case 27:
							{
//                                sprintf(LcdDispBuff,"S:%02u",sizeof(Application_Setting.Perioric_Timer));	
//                                LcdDisplay(TOP,LcdDispBuff);
//                                Delay1_1s(5);                                
                                if(Get_Hr_min_Sec(&Application_Setting.Perioric_Timer.Hour,0,&Application_Setting.Perioric_Timer.Minute,1,&Application_Setting.Perioric_Timer.Second,1))
                                {
                                    Write_E2(EPeriodic_Timer,(char*)&Application_Setting.Perioric_Timer.Hour,sizeof(Application_Setting.Perioric_Timer));	
                                }
//                                sprintf(LcdDispBuff,"%02u:%02u:%02u",Application_Setting.Perioric_Timer.Hour,Application_Setting.Perioric_Timer.Minute,Application_Setting.Perioric_Timer.Second);	
//                                LcdDisplay(TOP,LcdDispBuff);
//                                Delay1_1s(5);
                                
                                break;
                            }   
                            case 28:
							{
								Get_Phone_No();
								break;
							}
                            case 29:
							{
								View_SMS_Count();
								break;
							}
                            case 30:
							{
								View_GPRS_LIMIT();
								break;
							}
                            default:
								break;
						}
						curindex = (unsigned char)MenuData[curindex].Enter;
					}
				}
 			}
			else
			{	
				curindex = (unsigned char)MenuData[curindex].Enter;			
			}
            if(Calib_Flag == Loadcalib)
            {
                if(curindex == 44)   //Cal Zero
                    curindex = 46;            
                if(curindex == 48)   //Cal span
                    curindex = 50;            
                if(curindex == 54)   //Calcapacity
                    curindex = 52;            
            }
            else if(Calib_Flag == Anglecalib)
            {
                if(curindex == 46)   //Cal Zero
                    curindex = 44;            
                if(curindex == 50)   //Cal span
                    curindex = 48;            
                if(curindex == 52)   //Calcapacity
                    curindex = 54;   
            }
		}
		ReadRTC_DateTime();	//To update the Run time
	}		
}


#define REDLED_OP1      _RG6    //RED
#define GREENLED_OP2    _RG7
#define AMBERLED_OP3    _RG8
#define OUTPUT1         _RB5

void DOPCheck()						//OUTPUT1 - RB5, OUTPUT2 - RB4	//LED - RG6,RG7,RG8
{
	unsigned char DisplayFlag = 0;
	REDLED_OP1 = 0;Nop();
	GREENLED_OP2 = 0;Nop();
	AMBERLED_OP3 = 0;Nop();
	OUTPUT1 = 0;Nop();
    memset(LcdDispBuff+16,' ',16);
	while(1)
	{
		if(DisplayFlag >= 6)
			memcpy(LcdDispBuff+16,"OP1-            ",16); 
		else
			memcpy(LcdDispBuff+16," L1-  L2-  L3-  ",16);  
		KeySense();
		if(MenuKey)
		{
            REDLED_OP1 = 0;Nop();
            GREENLED_OP2 = 0;Nop();
            AMBERLED_OP3 = 0;Nop();
            OUTPUT1 = 0;Nop();
            memset(LcdDispBuff+16,' ',16);            
			break;
		}
		else if(UpKey)
		{
			DisplayFlag++;		
			if(DisplayFlag >= 8)
			{
				DisplayFlag = 0;
			}
		}		

		if(DisplayFlag == 1)
		{
			GREENLED_OP2 = 1;Nop();Nop();
			LcdDispBuff[20] = '1';
			LcdDispBuff[25] = '0';
			LcdDispBuff[30] = '0';		
		}
		else if(DisplayFlag == 3)
		{
			AMBERLED_OP3 = 1;Nop();Nop();
			LcdDispBuff[20] = '0';
			LcdDispBuff[25] = '1';
			LcdDispBuff[30] = '0';			
		}
		else if(DisplayFlag == 5)
		{
			REDLED_OP1 = 1;Nop();Nop();
			LcdDispBuff[20] = '0';
			LcdDispBuff[25] = '0';
			LcdDispBuff[30] = '1';		
		}
		else if(DisplayFlag == 7)
		{
//			memcpy(LcdDispBuff+16,"    LED CHECK   ",16); 
			OUTPUT1 = 1;Nop();	//Green
			LcdDispBuff[20] = '1';
//			LcdDispBuff[25] = '0';
//			LcdDispBuff[30] = '0';
		}
/*		else if(DisplayFlag == 10)
		{
			LcdDispBuff[20] = '0';
			LcdDispBuff[25] = '1';
			LcdDispBuff[30] = '0';
			_RG7 = 1;Nop();		//Amber
		}
		else if(DisplayFlag == 12)
		{
			LcdDispBuff[20] = '0';
			LcdDispBuff[25] = '0';
			LcdDispBuff[30] = '1';
			_RG8 = 1;Nop();		//Red
		} */
		else
		{
            REDLED_OP1 = 0;Nop();
            GREENLED_OP2 = 0;Nop();
            AMBERLED_OP3 = 0;Nop();
            OUTPUT1 = 0;Nop();
			LcdDispBuff[20] = '0';
            if(DisplayFlag < 6)
            {
                LcdDispBuff[25] = '0';
                LcdDispBuff[30] = '0';
            }
		}
		LcdDisplay(BOTTOM,LcdDispBuff+16);
		if(TimeOutFlag.TimeOut)
			return;
	}
}

void DIPCheck()
{
	memcpy(LcdDispBuff+16," I1-     I2-    ",16); 
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			break;
		}
		if(_RC2 != 1)	//Input 1
			memcpy(LcdDispBuff+20,"ON ",3);
		else
			memcpy(LcdDispBuff+20,"OFF",3);
		
//		if(_RC1 != Setup.InputLogic)	//Input 2
		if(_RC1 != 1)	//Input 2
			memcpy(LcdDispBuff+28,"ON ",3);//			memcpy(LcdDispBuff+16,"     TIPPING    ",16);
		else
			memcpy(LcdDispBuff+28,"OFF",3);//			memcpy(LcdDispBuff+16,"     RESTING    ",16);

		LcdDisplay(BOTTOM,LcdDispBuff+16);
//		if(TimeOutFlag.TimeOut)
//			return;
	}
	memset(LcdDispBuff+16,' ',16);
	return;
}

void KeypadTest()
{
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
//			memcpy(LcdDispBuff+16,"    MENU/ESC    ",16);
			LcdDisplay(BOTTOM,(char *)"    MENU/ESC    ");
			Delay1_1s(1);
            LcdDisplay(BOTTOM,(char *)"                ");
			return;
		}
		else if(EnterKey)
		{
//			memcpy(LcdDispBuff+16,"   ENTER/TARE   ",16);
			LcdDisplay(BOTTOM,(char *)"   ENTER/TARE   ");
			Delay1_1s(1);
//            LcdDisplay(BOTTOM,(char *)"                ");
		}
		else if(UpKey)
		{
//			memcpy(LcdDispBuff+16,"       UP       ",16);
			LcdDisplay(BOTTOM,(char *)"       UP       ");
			Delay1_1s(1);
//            LcdDisplay(BOTTOM,(char *)"                ");
		}
		else if(LeftKey)
		{
//			memcpy(LcdDispBuff+16,"      LEFT      ",16);
			LcdDisplay(BOTTOM,(char *)"      LEFT      ");
			Delay1_1s(1);
//            LcdDisplay(BOTTOM,(char *)"                ");
		}
		LcdDisplay(BOTTOM," Press any key  ");
//		if(TimeOutFlag.TimeOut)
//			return;
	}
	return;
}

void DisplayCheck()
{
	unsigned char DispCnt = 0;
	unsigned long DispCh = 0;	
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			memset(LcdDispBuff,' ',32);
			break;
		}
		else if(EnterKey)
		{
			if(DispCnt >= 4)
				DispCnt = 0;
		}
		if(DispCnt < 4)
		{		
			if(DispCh == 0)
			{
				memset(LcdDispBuff,0xFF,32);
			}
			else if(DispCh == 120000)
			{
				memset(LcdDispBuff,' ',32);
//				LcdDisplay(TOP,LcdDispBuff);
//				LcdDisplay(BOTTOM,LcdDispBuff+16);
			}
			if(DispCh++ >= 160000)
			{
				DispCh = 0;
				DispCnt++;
			}
            LcdDisplay(TOP,LcdDispBuff);
            LcdDisplay(BOTTOM,LcdDispBuff+16);            
		}
		else
		{
			LcdDisplay(TOP,(char *)" Do you want to ");
			LcdDisplay(BOTTOM,(char *)"  flash again?  ");
		}	
//		if(TimeOutFlag.TimeOut)
//			return;
	}
}

void DiagnosticCounts(void)
{
	DiagnoseFlag = 1;
	while(1)
	{
        KeySense();
        if(MenuKey)
        {
            memset(LcdDispBuff+16,' ',16);                    
            break;
        }
//        else if(UpKey)
//        {	
//            if(DiagnoseFlag++ >= 4)
//            DiagnoseFlag = 1;
//        }        
		if(DiagnoseFlag == 0)
            break;
        if(DiagnoseFlag == 1)
        {
			ShowAD(3);
        }
		else if(DiagnoseFlag == 5)
        {
			Show_Volt();
        }
		else if(DiagnoseFlag == 8)
        {
			Calc_Volt();
        }
    	memset(LcdDispBuff+16,' ',16);
    }
}

void ShowAD(char val)
{
	char DispFlag = 0;
	long TempADC = 0;
    //val - 2(Voltage calibration display),3(Rawadc count)
	if((val == 3) || (val == 2))
	{
		memset(LcdDispBuff,' ',32);
	}	

	while(1)
	{
        if((DiagnoseFlag == 1) || (val == 2))
        {
			if(Application_Setting.LCED[CH1] == 1)
            {
                CH1ADC = (unsigned int)GetAdc(CH1);
                sprintf(LcdDispBuff+1,"%4d",CH1ADC);
            }
			if(Application_Setting.LCED[CH2] == 1)
			{
                CH2ADC = (unsigned int)GetAdc(CH2);
                sprintf(LcdDispBuff+6,"%4d",CH2ADC);
			}
			if(Application_Setting.LCED[CH3] == 1)
			{
                CH3ADC = (unsigned int)GetAdc(CH3);
                sprintf(LcdDispBuff+11,"%4d",CH3ADC);
			}
			if(Application_Setting.LCED[CH4] == 1)
            {
                CH4ADC = (unsigned int)GetAdc(CH4);
                sprintf(LcdDispBuff+17,"%4d",CH4ADC);
            }
			if(Application_Setting.LCED[CH5] == 1)
			{
                CH5ADC = (unsigned int)GetAdc(CH5);
                sprintf(LcdDispBuff+22,"%4d",CH5ADC);
			}
			if(Application_Setting.LCED[CH6] == 1)
			{
                CH6ADC = (unsigned int)GetAdc(CH6);            
                sprintf(LcdDispBuff+27,"%4d",CH6ADC);
			}
            LcdDispBuff[5] = LcdDispBuff[10] = LcdDispBuff[21] = LcdDispBuff[26] = '/';
            LcdDisplay(TOP,LcdDispBuff);
			LcdDisplay(BOTTOM,LcdDispBuff+16);		
		}
		else
		{
            //Weight_Calc();            
            WeightDisplay(DiagnoseFlag);                
			LcdDisplay(BOTTOM,LcdDispBuff+16);	
		}
        if(val == 2)
        {
            return;    
        }          
		KeySense();
		if(MenuKey)
		{
            DiagnoseFlag = 0;
    		break;
        }
		else if(UpKey)
		{
			if(val == 3)
			{
                DiagnoseFlag++;
                if(DiagnoseFlag >= 5)
                    break;
                else if(DiagnoseFlag == 0)
                {
                    TwoLineDisplay("   ADC COUNTS   ","                ",1);
                }
                else if(DiagnoseFlag == 2)
                {
                    TwoLineDisplay("      Angle     ","                ",0);
                }
                else
                {
                    TwoLineDisplay(" Current Weight ","                ",0);                    
                }
	//			break;
			}
			else
			{
				if(DispFlag++ > 0)
				{
					DispFlag = 0;
				}	

			}	
		}
//		if(TimeOutFlag.TimeOut)
//			return;
	}
	TimeOutFlag.TimeOutEnb = 1;TimeOutFlag.TimeOut = 0;TimeOut_Cnt = 0;
	return;
}

//This function added from Wheel loader V1.0.0 which is sent to the customer for demo
void InputLogic_Multiply_Select(char InputSel_Minussel)
{
	unsigned char EnDis = 0;
	unsigned char Selected_Option = 0;
	unsigned int Blink = 0;
Selection:
    LcdDisplay(TOP,(char *)"  INPUT LOGIC   ");                    
    Selected_Option = OptionChooseFunction("   INPUT - 1       INPUT - 2    ",2,1);
    if(Selected_Option)
    {
        if(Selected_Option == 1)
            EnDis = Application_Setting.Input1Logic;
        else if(Selected_Option == 2)
            EnDis = Application_Setting.Input2Logic;
    }
    else
        return;
    
    if(EnDis > 1)
        EnDis = 0;
    if(Selected_Option == 1) 
        LcdDisplay(TOP,(char *)" INPUT - 1 LOGIC");
    else if(Selected_Option == 2)       
        LcdDisplay(TOP,(char *)" INPUT - 2 LOGIC");    
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			break;
		}
		else if(UpKey)
		{
			if(EnDis++ >= 1)
				EnDis = 0;
		}
		else if(EnterKey)
		{
            if(Selected_Option == 1)          
            {
                Application_Setting.Input1Logic = EnDis;
                Write_E2(EInput1Logic,(char*)&Application_Setting.Input1Logic,sizeof(Application_Setting.Input1Logic));	
//                Selected_Option = 2;
//                EnDis = Application_Setting.Input2Logic;                
            }
            else if(Selected_Option == 2)          
            {
                Application_Setting.Input2Logic = EnDis;
                Write_E2(EInput2Logic,(char*)&Application_Setting.Input2Logic,sizeof(Application_Setting.Input2Logic));	
//                Selected_Option = 1;
//                EnDis = Application_Setting.Input1Logic;                
            }
            goto Selection;
		}
        if(EnDis == 0)
            LcdDisplay(BOTTOM,(char *)"             Low");
        else
            LcdDisplay(BOTTOM,(char *)"            High");
    }
	memcpy(LcdDispBuff+16,"                ",16);
	goto Selection;		
}

char Getvalues_mA(char Ref,long mA_val)
{
	int Ch1Cnt = 0,Ch2Cnt = 0,Ch3Cnt = 0,Ch4Cnt = 0;
	unsigned int TotalADC = 0;
	unsigned char ErrFlag = 0;
    memset(LcdDispBuff,' ',32);        
	while(1)
	{
        ShowAD(2);//WeightDisplay(Count_Display);
		KeySense();
		if(MenuKey)
		{
			return(0);
		}
		else if(EnterKey)
		{
			if(ErrFlag == 0)
			{
				if(Ref == 1)
				{		
					VoltageCalib.Ch1Ref1 = (int)CH1ADC;//Loadcell[CH1].RawADC;
					VoltageCalib.Ch2Ref1 = (int)CH2ADC;//Loadcell[CH2].RawADC;
					VoltageCalib.Ch3Ref1 = (int)CH3ADC;//Loadcell[CH3].RawADC;
					VoltageCalib.Ch4Ref1 = (int)CH4ADC;//Loadcell[CH4].RawADC;
					VoltageCalib.Ch5Ref1 = (int)CH5ADC;//Loadcell[CH5].RawADC;
					VoltageCalib.Ch6Ref1 = (int)CH6ADC;//Loadcell[CH6].RawADC;
					
					VoltageCalib.REF1Cnt = VoltageCalib.Ch1Ref1 + VoltageCalib.Ch2Ref1 + VoltageCalib.Ch3Ref1 + VoltageCalib.Ch4Ref1+ VoltageCalib.Ch5Ref1+VoltageCalib.Ch6Ref1;
					VoltageCalib.Offset_Volt = mA_val;
                    
					Write_E2(EE_REF1Cnt,(char*)&VoltageCalib.REF1Cnt,sizeof(VoltageCalib.REF1Cnt));
					Write_E2(EE_Ch1Ref1,(char*)&VoltageCalib.Ch1Ref1,sizeof(VoltageCalib.Ch1Ref1));	
					Write_E2(EE_Ch2Ref1,(char*)&VoltageCalib.Ch2Ref1,sizeof(VoltageCalib.Ch2Ref1));	
					Write_E2(EE_Ch3Ref1,(char*)&VoltageCalib.Ch3Ref1,sizeof(VoltageCalib.Ch3Ref1));	
					Write_E2(EE_Ch4Ref1,(char*)&VoltageCalib.Ch4Ref1,sizeof(VoltageCalib.Ch4Ref1));	
					Write_E2(EE_Ch5Ref1,(char*)&VoltageCalib.Ch5Ref1,sizeof(VoltageCalib.Ch5Ref1));	
					Write_E2(EE_Ch6Ref1,(char*)&VoltageCalib.Ch6Ref1,sizeof(VoltageCalib.Ch6Ref1));	
					Write_E2(EE_Offset_Volt,(char*)&VoltageCalib.Offset_Volt,sizeof(VoltageCalib.Offset_Volt));	
				}
				else if(Ref == 2)
				{
					VoltageCalib.Ch1Ref2 = (int)CH1ADC;//Loadcell[CH1].RawADC;
					VoltageCalib.Ch2Ref2 = (int)CH2ADC;//Loadcell[CH2].RawADC;
					VoltageCalib.Ch3Ref2 = (int)CH3ADC;//Loadcell[CH3].RawADC;
					VoltageCalib.Ch4Ref2 = (int)CH4ADC;//Loadcell[CH4].RawADC;	
					VoltageCalib.Ch5Ref2 = (int)CH5ADC;//Loadcell[CH5].RawADC;	
					VoltageCalib.Ch6Ref2 = (int)CH6ADC;//Loadcell[CH6].RawADC;	
					
					VoltageCalib.REF2Cnt = VoltageCalib.Ch1Ref2 + VoltageCalib.Ch2Ref2 + VoltageCalib.Ch3Ref2 + VoltageCalib.Ch4Ref2+ VoltageCalib.Ch5Ref2+VoltageCalib.Ch6Ref2;
    				Write_E2(EE_REF2Cnt,(char*)&VoltageCalib.REF2Cnt,sizeof(VoltageCalib.REF2Cnt));
					Write_E2(EE_Ch1Ref2,(char*)&VoltageCalib.Ch1Ref2,sizeof(VoltageCalib.Ch1Ref2));	
					Write_E2(EE_Ch2Ref2,(char*)&VoltageCalib.Ch2Ref2,sizeof(VoltageCalib.Ch2Ref2));	
					Write_E2(EE_Ch3Ref2,(char*)&VoltageCalib.Ch3Ref2,sizeof(VoltageCalib.Ch3Ref2));	
					Write_E2(EE_Ch4Ref2,(char*)&VoltageCalib.Ch4Ref2,sizeof(VoltageCalib.Ch4Ref2));	
					Write_E2(EE_Ch5Ref2,(char*)&VoltageCalib.Ch5Ref2,sizeof(VoltageCalib.Ch5Ref2));	
					Write_E2(EE_Ch6Ref2,(char*)&VoltageCalib.Ch6Ref2,sizeof(VoltageCalib.Ch6Ref2));	
				}
				memset(LcdDispBuff+16,' ',16);
				return(1);
			}
			else
			{
				LcdDisplay(BOTTOM,(char *)"     VALUE ERROR");
				Delay1_1s(3);
			}
		}
		//if(TimeOutFlag.TimeOut)
		//	return(0);
		TotalADC = Ch1Cnt = Ch2Cnt = Ch3Cnt = Ch4Cnt = 0;
		ErrFlag = 0;
	}	
	return(0);

}

void PutDp(unsigned char*Tmp,unsigned long deci,char x,char displaymode)
{
	char i;
	unsigned char buff[20];

	memcpy(buff,Tmp,(int)x);

	if(deci != 0)
	{
		memmove((buff+(x-(char)deci+1)),(buff+(x-(char)deci)),(int)deci);
		*(buff+(x-(char)deci)) = '.';
	}
	else 
		*(buff+x)=' ';

	if(displaymode == 1)
	{
		for(i=0;i<(x-deci-1);i++)
		{
			if(*(buff+i) == '0')
				*(buff+i) = ' ';
			else if(*(buff+i) != '-') 
				break;
		}
	}
	if(deci != 0)
		memcpy(Tmp,buff,x+1);
	else
	{
		memcpy((Tmp+1),buff,x);
		*Tmp=' ';
	}
	return;
}

char GetData(char* buff,char x,char mode,char flag,long minval,long maxval,char dept)
{
	char scanbuff[7]={0};
	unsigned int blink = 0;
	int digit_1;
	int scanval;
	unsigned char TempBuff_1[10];
	memcpy(LcdDispBuff+16,"                ",16);
	digit_1=5;
	buff[6]='\0';
	scanbuff[6]='\0';
    if(mode == 0)
	{
		for(scanval=0;scanval<6;scanval++)
			scanbuff[scanval]=buff[scanval];
	}
	else
	{
		for(scanval=0;scanval<6;scanval++)
		{
			scanbuff[scanval] = '0';
			buff[scanval] = '0';
		}
	}
//    sprintf(LcdDispBuff+16,"%ld:%ld:%d",minval,maxval,flag);
//    LcdDisplay(TOP,LcdDispBuff+16);
//    Delay_1sec(5);
	while(1)
	{
		blink++;
        if((curindex >= 44 && curindex <= 51) || (curindex == 67) || (curindex == 68) || (curindex == 71) || (curindex == 72))
        {
            CalcADC(); 
            if((curindex == 44) || (curindex == 48))    
                sprintf(LcdDispBuff+16,"%4ld",Loadcell[CH1].RawADC);
            else if((curindex == 45) || (curindex == 49))    
                sprintf(LcdDispBuff+16,"%4ld",Loadcell[CH2].RawADC);
            else if((curindex == 46) || (curindex == 50))    
                sprintf(LcdDispBuff+16,"%4ld",Loadcell[CH3].RawADC);
            else if((curindex == 47) || (curindex == 51))    
                sprintf(LcdDispBuff+16,"%4ld",Loadcell[CH4].RawADC);
            else if((curindex == 67) || (curindex == 68))    
                sprintf(LcdDispBuff+16,"%4ld",Loadcell[CH5].RawADC);
            else if((curindex == 71) || (curindex == 72))    
                sprintf(LcdDispBuff+16,"%4ld",Loadcell[CH6].RawADC);
        }
        else if( (curindex == 18) || (curindex == 81) || (curindex == 82) )
            memcpy(LcdDispBuff+16,"(sec)",5);
        if(PasswordEntry == 1)
		{
			strncpy(LcdDispBuff+29,scanbuff+3,3);
		}
		else
			strncpy(LcdDispBuff+26,scanbuff,6);	
		if(blink>50) //90
			(LcdDispBuff+26)[digit_1] = '_';
		if(dept != 0)
		{
			memcpy(TempBuff_1,LcdDispBuff+26,6);
			PutDp(TempBuff_1,(long)dept,6,1);	
			memcpy(LcdDispBuff+25,TempBuff_1,7);	
		}
		LcdDisplay(BOTTOM,LcdDispBuff+16);
		if(blink>=100)//160
			blink=0;
		KeySense();       
		if(EnterKey)
		{   
			if(mode==1)
			{
				return(1);
			}
			else
			{
				if(flag==1)
				{
                    if(((((atol(scanbuff)) == minval) && (curindex >= 48 && curindex <= 51)) ||(((atol(scanbuff)) < minval) && (curindex < 48 && curindex > 51)) || ((atol(scanbuff)) > maxval)))
                    {
                        LcdDisplay(BOTTOM,(char *)"     VALUE ERROR");
                        Delay1_1s(5);
                        memset(LcdDispBuff+16,' ',16);
                    }
                    else
                    {
                        memset(LcdDispBuff+16,' ',16);   
                        return(1);
                    }
				}
				else
				{
                    memset(LcdDispBuff+16,' ',16);
					return(1);
				}
			}
		}
		else if(MenuKey)
		{
			memset(LcdDispBuff+16,' ',16);
			return(0);
		}
		else if(UpKey)
		{
			if(curindex != 7) // 12,22 --> decimalpoint // && (curindex != 22)
			{
				if((digit_1 != 0) || (mode == 1)) //&& (curindex != 7))
				{
					if(buff[digit_1]=='9')
						buff[digit_1]='0';
					else
						buff[digit_1]+= 1;
				}
				else 
				{
					if(buff[digit_1]=='1')
						buff[digit_1]='0';
					else
						buff[digit_1]+= 1;
				}
				scanbuff[digit_1]=buff[digit_1];
				blink=0;			
			}
			else 
			{
				if(buff[digit_1]=='3')
					buff[digit_1]='0';
				else
					buff[digit_1]+= 1;
				scanbuff[digit_1]=buff[digit_1];
				blink=0;
			}
		}
		else if(LeftKey)
		{
			if(mode==1)
				scanbuff[digit_1]='-';
			if((6-x)<digit_1)
				digit_1--;
			else
				digit_1=5;
			blink=0;
		}

//		if(curindex >= 5 && curindex <= 14)
//		{
////			TimeOutFlag.TimeOutEnb = 0;TimeOutFlag.TimeOut = 0;TimeOut_Cnt = 0;			
//		}	
//		if(TimeOutFlag.TimeOut)
//		{
//			return(0);
//		}
//        sprintf(LcdDispBuff,"IN10:%d:%d:%d",digit,PasswordEntry,blink);
//        LcdDisplay(TOP,LcdDispBuff);
	}
//	TimeOutFlag.TimeOutEnb = 1;TimeOutFlag.TimeOut = 0;TimeOut_Cnt = 0;
//#endif
	return(0);
}

char GetCombo(char x)
{
	char digit_2=0;
    digit_2 = (char)(MenuData[x].Value->Val.ucData-1);
//    sprintf(LcdDispBuff,"2:%d:%d:%d",x,digit_2,(char)(MenuData[x].Value->Val.ucData));
//    LcdDisplay(TOP,LcdDispBuff);
//    Delay1_1s(1);                
//    sprintf(LcdDispBuff+16,"1:%6ld:%d:%6ld",(*(MenuData[x].Max)),digit_2,(*(MenuData[x].Min)));   
//	LcdDisplay(BOTTOM,LcdDispBuff+16);	
//    Delay_1sec(5);    
//    sprintf(LcdDispBuff+16,"2:        %6ld",(*(MenuData[x].Min)));    
//		LcdDisplay(BOTTOM,LcdDispBuff+16);	    
//    Delay_1sec(5);
    //	digit = (char)(MenuData[x].Value->Val.ucData);
    memset(LcdDispBuff+16,' ',16);
	while(1)
	{
        if(curindex == 29)
            sprintf(LcdDispBuff+16,"          %6ld",BaudRateBuff[digit_2]);
        else if(curindex == 80)
            sprintf(LcdDispBuff+16,"          %6ld",CAN_BaudRateBuff[digit_2]);
        else if(curindex == 61)
        {
            if(digit_2 == 0)
                memcpy(LcdDispBuff+30,"Kg",2);
            else
                memcpy(LcdDispBuff+30," T",2);
        }
//        else if(curindex == 7)
//        {
//            if(digit_2 == 0)
//                memset(LcdDispBuff+31,'0',1);
//            else
//                memset(LcdDispBuff+31,'1',1);
//        }        
        else 
        {
            sprintf(LcdDispBuff+16,"          %6u",resdata[digit_2]);
            Delay_ms(200);
        }
 //           sprintf(LcdDispBuff,"%6u",digit_2);
		LcdDisplay(BOTTOM,LcdDispBuff+16);	
//		LcdDisplay(TOP,LcdDispBuff);	

		KeySense();
		if(EnterKey)
		{
			memset(LcdDispBuff+16,' ',16);
//            if(curindex == 7)            
//            {
//                sprintf(LcdDispBuff,"3:%d:%d",DecimalPoint,digit_2);
//                LcdDisplay(TOP,LcdDispBuff);
//                Delay_1sec(5);        
//                return((unsigned char)digit_2);
//            }
//            else
                return((unsigned char)(digit_2+1));
		}
		else if(MenuKey)
		{
			memset(LcdDispBuff+16,' ',16);
			return(0);
		}
		else if(UpKey)
		{
			if(digit_2 == *(MenuData[x].Max))
				digit_2 = (char)(*(MenuData[x].Min));
			else
				digit_2++;			
		}
	}
	return(0);
}

char GetPassword()
{
	char EntryPassword[7] = {0};
	LcdDisplay(TOP,(char *)" ENTER PASSWORD ");
	PasswordEntry = 1;
	if(GetData(EntryPassword,3,1,1,0,0,0))
	{
        memcpy(LcdDispBuff,EntryPassword,16);
        EntryPassword[6]='\0';
		
		if(atol(EntryPassword)== 795) // && RunTimeClr == 0)
        {
            PasswordEntry = 0;
            return(1);
        }
        if(atol(EntryPassword) == System_setting.Password)
        {
            PasswordEntry = 0;
            return(2);
        }
        else
		{
			LcdDisplay(BOTTOM,(char *)"      ERROR     ");
			Delay1_1s(1);
            PasswordEntry = 0;
			return(0);
		}
	}
	return(0);
}


void PowerOn_Read(void)
{
	//Weight Calibration Read

	Read_E2((char *)&Resolution,EResolution,sizeof(Resolution));
	Read_E2((char *)&MaxCapacity,EMaxCapacity,sizeof(MaxCapacity));
	Read_E2((char *)&DecimalPoint,EDecimalPoint,sizeof(DecimalPoint));
	Read_E2((char *)&Unit,EUnit,sizeof(Unit));
    if(DecimalPoint > 3)
    DecimalPoint = 1;
    if(Unit > 2)
    Unit = 1;    

    for(Current_CH_Axle=CH1;Current_CH_Axle< NO_OF_CH;Current_CH_Axle++)
    {
        if(Current_CH_Axle != CH6)
        {
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero,(ELC1Zero+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero));Delay_ms(100);     
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero1,(ELC1Zero1+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero1));Delay_ms(100);
            Read_E2((char *)&Loadcell[Current_CH_Axle].Span,(ELC1Span+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Span));Delay_ms(100);
            Read_E2((char *)&Loadcell[Current_CH_Axle].CalCapacity,(ELC1CalCapacity+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].CalCapacity));Delay_ms(100);        
            Read_E2((char *)&Application_Setting.LCED[Current_CH_Axle],(ELC1ED+Current_CH_Axle),sizeof(Application_Setting.LCED[Current_CH_Axle]));Delay_ms(100);
        }
        else
        {
            Read_E2((char *)&Loadcell[CH6].Zero,ELC6Zero,sizeof(Loadcell[CH6].Zero));Delay_ms(100);     
            Read_E2((char *)&Loadcell[CH6].Zero1,ELC6Zero1,sizeof(Loadcell[CH6].Zero1));Delay_ms(100);
            Read_E2((char *)&Loadcell[CH6].Span,ELC6Span,sizeof(Loadcell[CH6].Span));Delay_ms(100);
            Read_E2((char *)&Loadcell[CH6].CalCapacity,ELC6CalCapacity,sizeof(Loadcell[CH6].CalCapacity));Delay_ms(100);        
            Read_E2((char *)&Application_Setting.LCED[CH6],ELC6ED,sizeof(Application_Setting.LCED[CH6]));Delay_ms(100);
        }
    }
    Application_Setting.LCED[CH3] = 1;  //Ver2.1
            
    Read_E2((char *)&System_setting.DeviceID,EDeviceID,sizeof(System_setting.DeviceID));
    Read_E2((char *)&Application_Setting.ComMode,EComMode,sizeof(Application_Setting.ComMode));
    if(Application_Setting.ComMode > COM_DUMP)  //COM_PRINTER
        Application_Setting.ComMode= COM_GPRS_MODULE;      
    Read_E2((char *)&System_setting.Password,EPassword,sizeof(System_setting.Password));
    Read_E2((char *)&System_setting.BuadRate,EBuadRate,sizeof(System_setting.BuadRate));
    Read_E2((char *)&System_setting.CAN_BuadRate,ECAN_BuadRate,sizeof(System_setting.CAN_BuadRate));
    if(System_setting.CAN_BuadRate > 3)
        System_setting.CAN_BuadRate = 1;
    Read_E2((char *)&Application_Setting.MinWt,E_MinWt,sizeof(Application_Setting.MinWt));
    Read_E2((char *)&Application_Setting.PLMS_Status,E_PLMS_Status,sizeof(Application_Setting.PLMS_Status));
    Read_E2((char *)&Application_Setting.AngleConfig,EAngleConfig,sizeof(Application_Setting.AngleConfig));
    if(Application_Setting.AngleConfig > 9)
        Application_Setting.AngleConfig= 0;    
    Read_E2((char *)&Application_Setting.Red_Glow,E_Red_Glow,sizeof(Application_Setting.Red_Glow));    
    Read_E2((char *)&Application_Setting.Amber_Flash,E_Amber_Flash,sizeof(Application_Setting.Amber_Flash));
    Read_E2((char *)&Application_Setting.Amber_Glow,E_Amber_Glow,sizeof(Application_Setting.Amber_Glow));
    Read_E2((char *)&Application_Setting.Input1Logic,EInput1Logic,sizeof(Application_Setting.Input1Logic));
    Read_E2((char *)&Application_Setting.Input2Logic,EInput2Logic,sizeof(Application_Setting.Input2Logic));
    Read_E2((char *)&Application_Setting.Corr_Factor,ECorr_Factor,sizeof(Application_Setting.Corr_Factor));
    Read_E2((char *)&Application_Setting.Perioric_Timer.Hour,EPeriodic_Timer,sizeof(Application_Setting.Perioric_Timer));
//  sprintf(LcdDispBuff,"%02u:%02u:%02u",Application_Setting.Perioric_Timer.Hour,Application_Setting.Perioric_Timer.Minute,Application_Setting.Perioric_Timer.Second);	
//  LcdDisplay(TOP,LcdDispBuff);
//  Delay1_1s(5);    
//  Periodic_Time = Application_Setting.Perioric_Timer.Minute + Application_Setting.Perioric_Timer.Second;
    
    Voltage_calib_read();

    Read_E2((char *)&AllChannel.CumulativeWeight,EE_CumulativeWeight,sizeof(AllChannel.CumulativeWeight));
    Read_E2((char *)&AllChannel.TripCount,EE_TripCount,sizeof(AllChannel.TripCount));
    Read_E2((char *)&Application_Setting.OverWeight,EE_OverWeight,sizeof(Application_Setting.OverWeight));
    Read_E2((char *)&Application_Setting.LatchedWt,E_LatchedWt,sizeof(Application_Setting.LatchedWt));
    Read_E2((char *)&Application_Setting.LatchDelay,E_LatchDelay,sizeof(Application_Setting.LatchDelay));
    Read_E2((char *)&Application_Setting.CAN_Broadcast_Time,ECAN_Broadcast_Time,sizeof(Application_Setting.CAN_Broadcast_Time));
    if(Application_Setting.CAN_Broadcast_Time > 9999)
        Application_Setting.CAN_Broadcast_Time = 10;
    
    Read_E2((char *)&Application_Setting.RS232_Dump_Time,EERS232_Dump_Time,sizeof(Application_Setting.RS232_Dump_Time));
    if(Application_Setting.RS232_Dump_Time > 9999)
        Application_Setting.RS232_Dump_Time = 10;
    
    Read_E2((char *)&Application_Setting.Mobile_Operator,EES_Mobile_Operator,sizeof(Application_Setting.Mobile_Operator));
    if(Application_Setting.OverWeight > MaxCapacity)
    Application_Setting.OverWeight = 350;
    if(Application_Setting.LatchedWt > MaxCapacity)
    Application_Setting.LatchedWt = 200;      
    Read_E2((char *)&Application_Setting.OperationLogic,EE_OperationLogic,sizeof(Application_Setting.OperationLogic));
    if((Application_Setting.OperationLogic > BodyUp) || (Application_Setting.OperationLogic <= ParkingBreak))
    {
        Application_Setting.OperationLogic = ParkingBreak;  
        Write_E2(EE_OperationLogic,(char*)&Application_Setting.OperationLogic,sizeof(Application_Setting.OperationLogic));//Delay_ms(2);	
    }
    Read_E2((char *)&Application_Setting.AngleEnDis,EAngleEnDis,sizeof(Application_Setting.AngleEnDis));
    if((Application_Setting.AngleEnDis > 2) || (Application_Setting.AngleEnDis < 1))    //Enable - 1,Disable - 2 
    {
        Application_Setting.AngleEnDis = 1;  //Enable - 1
        Write_E2(EAngleEnDis,(char*)&Application_Setting.AngleEnDis,sizeof(Application_Setting.AngleEnDis));//Delay_ms(2);	
    }
    Read_E2((char *)&Application_Setting.No_of_Samples,ENo_of_Samples,sizeof(Application_Setting.No_of_Samples));

    if((Application_Setting.No_of_Samples > 400) || (Application_Setting.No_of_Samples < 10))
    {
        Application_Setting.No_of_Samples = 10;  
        Write_E2(ENo_of_Samples,(char*)&Application_Setting.No_of_Samples,sizeof(Application_Setting.No_of_Samples));//Delay_ms(2);	
    }
    Read_E2((char *)&CounterOverflowFlg,ECounterOverflowFlg,sizeof(CounterOverflowFlg));
    Read_E2((char *)&Average_Count,EAverage_Count,sizeof(Average_Count));
    if((Average_Count > 400) ||(Average_Count < 0))
    {
        Average_Count = 0;
        Write_E2(EAverage_Count,(char*)&Average_Count,sizeof(Average_Count));//Delay(2);	                            
    }
    Read_E2((char *)&MinWt_Check_Counter,EMinWt_Check_Counter,sizeof(MinWt_Check_Counter));
    Read_E2((char *)&LastTare.Date,E_LastTare,sizeof(LastTare));
    Read_E2((char *)&LastCalib.Date,E_LastCalib,sizeof(LastCalib));
    if((MinWt_Check_Counter > 1) ||(MinWt_Check_Counter <= 0))
    {
        MinWt_Check_Counter = 0;
        Write_E2(EMinWt_Check_Counter,(char*)&MinWt_Check_Counter,sizeof(MinWt_Check_Counter));//Delay(2);	                            
    }
    if(Average_Count != 0)
    {
        for(Current_CH_Axle = 0;Current_CH_Axle < Array_Counter;Current_CH_Axle++)
        {
            Read_E2((char*)&No_of_Samplesarray[Current_CH_Axle],(ENo_of_SamplesArray+(Current_CH_Axle * 2)),2);//Delay(2);	
        }
    }
    

}

void PortCheck(void)
{
    if(System_setting.BuadRate == 1)    // Baud Rate = 1200; //   13332 = 3414
        LcdDisplay(BOTTOM,(char *)"   Baud: 1200   ");
    else if(System_setting.BuadRate == 2)    // Baud Rate = 2400; //   6666 = 1A0A
        LcdDisplay(BOTTOM,(char *)"   Baud: 2400   ");
    else if(System_setting.BuadRate == 3)    // Baud Rate = 9600; //   1666 = 682
        LcdDisplay(BOTTOM,(char *)"   Baud: 9600   ");
    else if(System_setting.BuadRate == 4)    // Baud Rate = 19200; //  832 = 340
        LcdDisplay(BOTTOM,(char *)"   Baud: 19200  "); 
    else if(System_setting.BuadRate == 5)    // Baud Rate = 19200; //  832 = 340
        LcdDisplay(BOTTOM,(char *)"   Baud: 38400  "); 
    else if(System_setting.BuadRate == 6)    // Baud Rate = 57600; //  278 = 116
        LcdDisplay(BOTTOM,(char *)"   Baud: 57600  ");      
    else if(System_setting.BuadRate == 7)    // Baud Rate = 11500; //138
        LcdDisplay(BOTTOM,(char *)"   Baud: 115200 ");  
    else
        LcdDisplay(BOTTOM,(char *)"Check Baud Rate ");  
	Delay1_1s(5);
	while(1)
	{
		Com2Bytesend('U');
//		sprintf(LcdDispBuff,"%d:%c",Bits.Com2Check,U2RXREG);
//        LcdDisplay(TOP,LcdDispBuff);
		KeySense();
		if(MenuKey)
		{
			break;
		}

		if(Bits.Com2Check)
		{
			Bits.Com2Check = 0;
            LcdDisplay(BOTTOM,(char *)"  Status: PASS  ");
			Delay1_1s(1);
		}
		else
		{
            LcdDisplay(BOTTOM,(char *)"  Status: FAIL  ");	
		}
	}
	memset(LcdDispBuff+16,' ',16);
	return;
}
#define LCD_LINE_COLUMN 16
unsigned char OptionChooseFunction(char * DataOrigin,unsigned char AvailData,unsigned char InitData)
{
    unsigned char BotDisp = 1;
    BotDisp = InitData;
    while(1)
	{
        LcdDisplay(BOTTOM,(DataOrigin+((BotDisp-1)*LCD_LINE_COLUMN)));
        KeySense();
		if(MenuKey)
		{
        //  ClearLcdDisplay(TOP_BOTTOM);
			return(0);
		}
        else if(UpKey)
        {
            BotDisp++;
            if(BotDisp > AvailData)
                BotDisp = 1;
            memset(LcdDispBuff,' ',32);     
        }        
		else if(EnterKey)
		{
           // ClearLcdDisplay(TOP_BOTTOM);            
            return(BotDisp);
		}
 	}
}

void Confirmation_Function(char selectedparameter)
{
    LcdDisplay(TOP,(char *)"Are you sure to ");
    if(selectedparameter == Loaddefault)
        LcdDisplay(BOTTOM,(char *)"  Load default? ");
    else if((selectedparameter == TakeLoadCalBackup) || (selectedparameter == TakeAngleCalBackup))
        LcdDisplay(BOTTOM,(char *)"Take Cal Backup?");
    else if((selectedparameter == RestoreLoadCalBackup) || (selectedparameter == RestoreAngCalBackup))
        LcdDisplay(BOTTOM,(char *)" Res Cal Backup?");
    else if(selectedparameter == TakeVoltBackup)
        LcdDisplay(BOTTOM,(char *)"Take VoltBackup?");
    else if(selectedparameter == RestoreVoltBackup)
        LcdDisplay(BOTTOM,(char *)" Res VoltBackup?");
    else if(selectedparameter == InitGenSettings)
        LcdDisplay(BOTTOM,(char *)" Init Gen Sett ?");
    else if(selectedparameter == InitVoltValue)
        LcdDisplay(BOTTOM,(char *)"Init Volt value?");
    else if(selectedparameter == InitAppSettings)
        LcdDisplay(BOTTOM,(char *)" Init App Sett ?");
    while(1)
	{
		KeySense();
		if(MenuKey)
		{
            ClearLcdDisplay(TOP_BOTTOM);
			break;
		}
		else if(EnterKey)
		{
            ClearLcdDisplay(TOP_BOTTOM);            
            LcdDisplay(BOTTOM,(char *)" Please Wait... ");
            Delay1_1s(2);               
            if(selectedparameter == Loaddefault)
                LoadDefault();
            else if(selectedparameter == TakeLoadCalBackup)
                Calibration_Backup(loadcells);
            else if(selectedparameter == RestoreLoadCalBackup)           
                Restore_Calib_Backup(loadcells);
            else if(selectedparameter == TakeVoltBackup)
                Calibration_Backup(Voltage);
            else if(selectedparameter == RestoreVoltBackup)
                Restore_Calib_Backup(Voltage);
            else if(selectedparameter == InitGenSettings)
                Init_GenSetting();
            else if(selectedparameter == InitAppSettings)
                Init_AppSetting();
            else if(selectedparameter == InitVoltValue)
                Voltage_Calib_init();
            else if(selectedparameter == TakeAngleCalBackup)
                Calibration_Backup(Angle);
            else if(selectedparameter == RestoreAngCalBackup) 
                Restore_Calib_Backup(Angle);
            done_Display();
			break;
		}
 	}
}

void done_Display(void)
{
    ClearLcdDisplay(TOP_BOTTOM);            
    LcdDisplay(BOTTOM,(char *)"   Completed    ");
    Delay1_1s(2);                            
    ClearLcdDisplay(TOP_BOTTOM);            
}

void Calibration_Backup(char loadcells_or_Voltage)
{
    if((loadcells_or_Voltage == Angle)||(loadcells_or_Voltage == loadcells_Voltage))
    {
        for(Current_CH_Axle=CH1;Current_CH_Axle< 2;Current_CH_Axle++)
        {
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero,(ELC1Zero+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero));     
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero1,(ELC1Zero1+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero1));
            Read_E2((char *)&Loadcell[Current_CH_Axle].Span,(ELC1Span+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Span));
            Read_E2((char *)&Loadcell[Current_CH_Axle].CalCapacity,(ELC1CalCapacity+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].CalCapacity));        

            Write_E2((EBLC1Zero+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero,sizeof(Loadcell[Current_CH_Axle].Zero));     
            Write_E2((EBLC1Zero1+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero1,sizeof(Loadcell[Current_CH_Axle].Zero1));
            Write_E2((EBLC1Span+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Span,sizeof(Loadcell[Current_CH_Axle].Span));
            Write_E2((EBLC1CalCapacity+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].CalCapacity,sizeof(Loadcell[Current_CH_Axle].CalCapacity)); 
        }        
    }
    if((loadcells_or_Voltage == loadcells)||(loadcells_or_Voltage == loadcells_Voltage))
    {
        Read_E2((char *)&Resolution,EResolution,sizeof(Resolution));
        Read_E2((char *)&MaxCapacity,EMaxCapacity,sizeof(MaxCapacity));
        Read_E2((char *)&Unit,EUnit,sizeof(Unit));
        Read_E2((char *)&DecimalPoint,EDecimalPoint,sizeof(DecimalPoint));
        
        Write_E2(EBResolution,(char *)&Resolution,sizeof(Resolution));				  
        Write_E2(EBMaxCapacity,(char *)&MaxCapacity,sizeof(MaxCapacity));
        Write_E2(EBUnit,(char *)&Unit,sizeof(Unit));				  
        Write_E2(EBDecimalPoint,(char *)&DecimalPoint,sizeof(DecimalPoint));
        
        for(Current_CH_Axle=CH3;Current_CH_Axle < CH6;Current_CH_Axle++)
        {
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero,(ELC1Zero+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero));     
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero1,(ELC1Zero1+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero1));
            Read_E2((char *)&Loadcell[Current_CH_Axle].Span,(ELC1Span+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Span));
            Read_E2((char *)&Loadcell[Current_CH_Axle].CalCapacity,(ELC1CalCapacity+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].CalCapacity));        

            Write_E2((EBLC1Zero+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero,sizeof(Loadcell[Current_CH_Axle].Zero));     
            Write_E2((EBLC1Zero1+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero1,sizeof(Loadcell[Current_CH_Axle].Zero1));
            Write_E2((EBLC1Span+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Span,sizeof(Loadcell[Current_CH_Axle].Span));
            Write_E2((EBLC1CalCapacity+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].CalCapacity,sizeof(Loadcell[Current_CH_Axle].CalCapacity)); 
        }        

        Read_E2((char *)&Loadcell[CH6].Zero,ELC6Zero,sizeof(Loadcell[CH6].Zero));     
        Read_E2((char *)&Loadcell[CH6].Zero1,ELC6Zero1,sizeof(Loadcell[CH6].Zero1));
        Read_E2((char *)&Loadcell[CH6].Span,ELC6Span,sizeof(Loadcell[CH6].Span));
        Read_E2((char *)&Loadcell[CH6].CalCapacity,ELC6CalCapacity,sizeof(Loadcell[CH6].CalCapacity));        

        Write_E2(EBLC6Zero,(char *)&Loadcell[CH6].Zero,sizeof(Loadcell[CH6].Zero));     
        Write_E2(EBLC6Zero1,(char *)&Loadcell[CH6].Zero1,sizeof(Loadcell[CH6].Zero1));
        Write_E2(EBLC6Span,(char *)&Loadcell[CH6].Span,sizeof(Loadcell[CH6].Span));
        Write_E2(EBLC6CalCapacity,(char *)&Loadcell[CH6].CalCapacity,sizeof(Loadcell[CH6].CalCapacity)); 
    }
    if((loadcells_or_Voltage == Voltage)||(loadcells_or_Voltage == loadcells_Voltage))
    {    
        Voltage_calib_read();
        Write_E2(EE_BREFvolt1,(char*)&VoltageCalib.REFvolt1,sizeof(VoltageCalib.REFvolt1));
        Write_E2(EE_BREFvolt2,(char*)&VoltageCalib.REFvolt2,sizeof(VoltageCalib.REFvolt2));
        Write_E2(EE_BREF1Cnt,(char*)&VoltageCalib.REF1Cnt,sizeof(VoltageCalib.REF1Cnt));
        Write_E2(EE_BREF2Cnt,(char*)&VoltageCalib.REF2Cnt,sizeof(VoltageCalib.REF2Cnt));
        Write_E2(EE_BCh1Ref1,(char*)&VoltageCalib.Ch1Ref1,sizeof(VoltageCalib.Ch1Ref1));	
        Write_E2(EE_BCh1Ref2,(char*)&VoltageCalib.Ch1Ref2,sizeof(VoltageCalib.Ch1Ref2));	
        Write_E2(EE_BCh2Ref1,(char*)&VoltageCalib.Ch2Ref1,sizeof(VoltageCalib.Ch2Ref1));	
        Write_E2(EE_BCh2Ref2,(char*)&VoltageCalib.Ch2Ref2,sizeof(VoltageCalib.Ch2Ref2));	
        Write_E2(EE_BCh3Ref1,(char*)&VoltageCalib.Ch3Ref1,sizeof(VoltageCalib.Ch3Ref1));	
        Write_E2(EE_BCh3Ref2,(char*)&VoltageCalib.Ch3Ref2,sizeof(VoltageCalib.Ch3Ref2));	
        Write_E2(EE_BCh4Ref1,(char*)&VoltageCalib.Ch4Ref1,sizeof(VoltageCalib.Ch4Ref1));	
        Write_E2(EE_BCh4Ref2,(char*)&VoltageCalib.Ch4Ref2,sizeof(VoltageCalib.Ch4Ref2));	   
        Write_E2(EE_BCh5Ref1,(char*)&VoltageCalib.Ch5Ref1,sizeof(VoltageCalib.Ch5Ref1));	
        Write_E2(EE_BCh5Ref2,(char*)&VoltageCalib.Ch5Ref2,sizeof(VoltageCalib.Ch5Ref2));	   
        Write_E2(EE_BCh6Ref1,(char*)&VoltageCalib.Ch6Ref1,sizeof(VoltageCalib.Ch6Ref1));	
        Write_E2(EE_BCh6Ref2,(char*)&VoltageCalib.Ch6Ref2,sizeof(VoltageCalib.Ch6Ref2));	   
        Write_E2(EE_BOffset_Volt,(char*)&VoltageCalib.Offset_Volt,sizeof(VoltageCalib.Offset_Volt));
    }
}

void Voltage_calib_read(void)
{
    Read_E2((char *)&VoltageCalib.REFvolt1,EE_REFvolt1,sizeof(VoltageCalib.REFvolt1));
    Read_E2((char *)&VoltageCalib.REFvolt2,EE_REFvolt2,sizeof(VoltageCalib.REFvolt2));    
    Read_E2((char *)&VoltageCalib.REF1Cnt,EE_REF1Cnt,sizeof(VoltageCalib.REF1Cnt));
    Read_E2((char *)&VoltageCalib.REF2Cnt,EE_REF2Cnt,sizeof(VoltageCalib.REF2Cnt));
    Read_E2((char *)&VoltageCalib.Ch1Ref1,EE_Ch1Ref1,sizeof(VoltageCalib.Ch1Ref1));
    Read_E2((char *)&VoltageCalib.Ch1Ref2,EE_Ch1Ref2,sizeof(VoltageCalib.Ch1Ref2));
    Read_E2((char *)&VoltageCalib.Ch2Ref1,EE_Ch2Ref1,sizeof(VoltageCalib.Ch2Ref1));
    Read_E2((char *)&VoltageCalib.Ch2Ref2,EE_Ch2Ref2,sizeof(VoltageCalib.Ch2Ref2));
    Read_E2((char *)&VoltageCalib.Ch3Ref1,EE_Ch3Ref1,sizeof(VoltageCalib.Ch3Ref1));
    Read_E2((char *)&VoltageCalib.Ch3Ref2,EE_Ch3Ref2,sizeof(VoltageCalib.Ch3Ref2));
    Read_E2((char *)&VoltageCalib.Ch4Ref1,EE_Ch4Ref1,sizeof(VoltageCalib.Ch4Ref1));
    Read_E2((char *)&VoltageCalib.Ch4Ref2,EE_Ch4Ref2,sizeof(VoltageCalib.Ch4Ref2));
    Read_E2((char *)&VoltageCalib.Ch5Ref1,EE_Ch5Ref1,sizeof(VoltageCalib.Ch5Ref1));
    Read_E2((char *)&VoltageCalib.Ch5Ref2,EE_Ch5Ref2,sizeof(VoltageCalib.Ch5Ref2));
    Read_E2((char *)&VoltageCalib.Ch6Ref1,EE_Ch6Ref1,sizeof(VoltageCalib.Ch6Ref1));
    Read_E2((char *)&VoltageCalib.Ch6Ref2,EE_Ch6Ref2,sizeof(VoltageCalib.Ch6Ref2));
    Read_E2((char *)&VoltageCalib.Offset_Volt,EE_Offset_Volt,sizeof(VoltageCalib.Offset_Volt)); 
}

void LoadDefault(void)
{
//  #ifdef Execute    
	Resolution = 1;
	MaxCapacity = 500;
    Unit = 1;   //Kg
    DecimalPoint = 1;
	Write_E2(EResolution,(char *)&Resolution,sizeof(Resolution));				  
	Write_E2(EMaxCapacity,(char *)&MaxCapacity,sizeof(MaxCapacity));
	Write_E2(EUnit,(char *)&Unit,sizeof(Unit));    
	Write_E2(EDecimalPoint,(char *)&DecimalPoint,sizeof(DecimalPoint));
    
  
    Loadcell[CH1].Zero = Loadcell[CH1].Zero1 = Loadcell[CH2].Zero = Loadcell[CH2].Zero1 = 1360; //Angle
    Loadcell[CH3].Zero = Loadcell[CH3].Zero1 = 854;
    Loadcell[CH4].Zero = Loadcell[CH4].Zero1 = 854;
    Loadcell[CH5].Zero = Loadcell[CH5].Zero1 = 854;
    Loadcell[CH6].Zero = Loadcell[CH6].Zero1 = 854;
    Loadcell[CH1].Span = Loadcell[CH2].Span = 1212; //Angle
    Loadcell[CH3].Span = 1364;
    Loadcell[CH4].Span = 1364;
    Loadcell[CH5].Span = 1364;
    Loadcell[CH6].Span = 1364;
    Loadcell[CH1].CalCapacity = Loadcell[CH2].CalCapacity = 45;
    Loadcell[CH3].CalCapacity = Loadcell[CH4].CalCapacity = 100;
    Loadcell[CH5].CalCapacity = Loadcell[CH6].CalCapacity = 100;
    
    for(Current_CH_Axle=CH1;Current_CH_Axle<CH6;Current_CH_Axle++)
    {
        Write_E2((ELC1Zero+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero,sizeof(Loadcell[Current_CH_Axle].Zero));     
        Write_E2((ELC1Zero1+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero1,sizeof(Loadcell[Current_CH_Axle].Zero1));
        Write_E2((ELC1Span+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Span,sizeof(Loadcell[Current_CH_Axle].Span));
        Write_E2((ELC1CalCapacity+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].CalCapacity,sizeof(Loadcell[Current_CH_Axle].CalCapacity));        
    }
        Write_E2(ELC6Zero,(char *)&Loadcell[CH6].Zero,sizeof(Loadcell[CH6].Zero));     
        Write_E2(ELC6Zero1,(char *)&Loadcell[CH6].Zero1,sizeof(Loadcell[CH6].Zero1));
        Write_E2(ELC6Span,(char *)&Loadcell[CH6].Span,sizeof(Loadcell[CH6].Span));
        Write_E2(ELC6CalCapacity,(char *)&Loadcell[CH6].CalCapacity,sizeof(Loadcell[CH6].CalCapacity));        
        //  #endif
}

void Init_AppSetting(void)
{
	Application_Setting.MinWt = 10;
	Application_Setting.Amber_Glow = 150;
	Application_Setting.Amber_Flash = 200;
	Application_Setting.Red_Glow = 250;    
	Application_Setting.ComMode = COM_GPRS_MODULE;    //GPRS Module
	Application_Setting.LCED[CH1] = Enable;
	Application_Setting.LCED[CH2] = Enable;
	Application_Setting.LCED[CH3] = Enable;
	Application_Setting.LCED[CH4] = Enable;
	Application_Setting.LCED[CH5] = Enable;
	Application_Setting.LCED[CH6] = Enable;
	Application_Setting.Input1Logic = 0;//Low
	Application_Setting.Input2Logic = 0;//Low
    Application_Setting.OverWeight = 350;
    Application_Setting.LatchedWt = 200; 
    Application_Setting.Corr_Factor = 1000;
    Application_Setting.Perioric_Timer.Hour = 17;
    Application_Setting.Perioric_Timer.Minute = 1;
    Application_Setting.Perioric_Timer.Second = 0;
    Application_Setting.AngleConfig = 0;   
    Application_Setting.No_of_Samples = 10;
    Application_Setting.LatchDelay = 10;
    Application_Setting.CAN_Broadcast_Time = 10;
    Application_Setting.Mobile_Operator = 0;
    Application_Setting.OperationLogic = ParkingBreak;//1
    Application_Setting.AngleEnDis = Enable; //Enable

    Application_Setting.RS232_Dump_Time = 10;
    Write_E2(EERS232_Dump_Time,(char *)&Application_Setting.RS232_Dump_Time,sizeof(Application_Setting.RS232_Dump_Time));
            
    Write_E2(EES_Mobile_Operator,(char *)&Application_Setting.Mobile_Operator,sizeof(Application_Setting.Mobile_Operator));
	Write_E2(ECorr_Factor,(char *)&Application_Setting.Corr_Factor,sizeof(Application_Setting.Corr_Factor));
	Write_E2(E_LatchDelay,(char *)&Application_Setting.LatchDelay,sizeof(Application_Setting.LatchDelay));
    Write_E2(ECAN_Broadcast_Time,(char *)&Application_Setting.CAN_Broadcast_Time,sizeof(Application_Setting.CAN_Broadcast_Time));
	Write_E2(ENo_of_Samples,(char *)&Application_Setting.No_of_Samples,sizeof(Application_Setting.No_of_Samples));
    Write_E2(EPeriodic_Timer,(char*)&Application_Setting.Perioric_Timer.Hour,sizeof(Application_Setting.Perioric_Timer));	 
	Write_E2(E_MinWt,(char *)&Application_Setting.MinWt,sizeof(Application_Setting.MinWt));
	Write_E2(EAngleConfig,(char *)&Application_Setting.AngleConfig,sizeof(Application_Setting.AngleConfig));
	Write_E2(E_Amber_Glow,(char *)&Application_Setting.Amber_Glow,sizeof(Application_Setting.Amber_Glow));
	Write_E2(E_Amber_Flash,(char *)&Application_Setting.Amber_Flash,sizeof(Application_Setting.Amber_Flash));
	Write_E2(E_Red_Glow,(char *)&Application_Setting.Red_Glow,sizeof(Application_Setting.Red_Glow));
	Write_E2(EComMode,(char *)&Application_Setting.ComMode,sizeof(Application_Setting.ComMode));
	Write_E2(EE_OverWeight,(char *)&Application_Setting.OverWeight,sizeof(Application_Setting.OverWeight));
	Write_E2(E_LatchedWt,(char *)&Application_Setting.LatchedWt,sizeof(Application_Setting.LatchedWt));
    Write_E2(EAngleEnDis,(char*)&Application_Setting.AngleEnDis,sizeof(Application_Setting.AngleEnDis));	
    for(Current_CH_Axle=CH1;Current_CH_Axle<NO_OF_CH;Current_CH_Axle++)
    {
        if(Current_CH_Axle == 5)
            Write_E2(ELC6ED,(char *)&Application_Setting.LCED[Current_CH_Axle],sizeof(Application_Setting.LCED[Current_CH_Axle]));
        else
            Write_E2((ELC1ED+Current_CH_Axle),(char *)&Application_Setting.LCED[Current_CH_Axle],sizeof(Application_Setting.LCED[Current_CH_Axle]));
	}
    Write_E2(EInput1Logic,(char *)&Application_Setting.Input1Logic,sizeof(Application_Setting.Input1Logic));   
    Write_E2(EInput2Logic,(char *)&Application_Setting.Input2Logic,sizeof(Application_Setting.Input2Logic));   
    Write_E2(EE_OperationLogic,(char *)&Application_Setting.OperationLogic,sizeof(Application_Setting.OperationLogic));   
}

void Init_GenSetting(void)
{
 //   #ifdef Execute       
	System_setting.Password = 444;
	System_setting.BuadRate = 3;    //9600
    System_setting.CAN_BuadRate = 1; // 250
 	System_setting.DeviceID = 1;
	Write_E2(EPassword,(char *)&System_setting.Password,sizeof(System_setting.Password));
	Write_E2(EBuadRate,(char *)&System_setting.BuadRate,sizeof(System_setting.BuadRate));   
    Write_E2(ECAN_BuadRate,(char *)&System_setting.CAN_BuadRate,sizeof(System_setting.CAN_BuadRate));
	Write_E2(EDeviceID,(char *)&System_setting.DeviceID,sizeof(System_setting.DeviceID));   
    Com2Init(System_setting.BuadRate);
//	Write_E2(E_LatchDelay,(char *)&Setup.LatchDelay,sizeof(Setup.LatchDelay));
    
//    Axle[0].Corr_Factor = Axle[1].Corr_Factor = 1000;
//    for(Current_CH_Axle=Axle1;Current_CH_Axle<Axle_NOs;Current_CH_Axle++)
//        Write_E2((ECorFacor_1+Current_CH_Axle),(char *)&Axle[Current_CH_Axle].Corr_Factor,sizeof(Axle[Current_CH_Axle].Corr_Factor));
//    #endif    
}

void Restore_Calib_Backup(char loadcells_or_Voltage)
{
    if(loadcells_or_Voltage == Angle)
    {
        for(Current_CH_Axle=CH1;Current_CH_Axle< 2 ;Current_CH_Axle++)
        {
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero,(EBLC1Zero+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero));     
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero1,(EBLC1Zero1+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero1));
            Read_E2((char *)&Loadcell[Current_CH_Axle].Span,(EBLC1Span+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Span));
            Read_E2((char *)&Loadcell[Current_CH_Axle].CalCapacity,(EBLC1CalCapacity+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].CalCapacity));        
        }    
        for(Current_CH_Axle=CH1;Current_CH_Axle<2;Current_CH_Axle++)
        {
            Write_E2((ELC1Zero+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero,sizeof(Loadcell[Current_CH_Axle].Zero));     
            Write_E2((ELC1Zero1+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero1,sizeof(Loadcell[Current_CH_Axle].Zero1));
            Write_E2((ELC1Span+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Span,sizeof(Loadcell[Current_CH_Axle].Span));
            Write_E2((ELC1CalCapacity+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].CalCapacity,sizeof(Loadcell[Current_CH_Axle].CalCapacity));        
        } 
    }
   else if(loadcells_or_Voltage == loadcells)
    {
        //Weight Calibration Backup Read
        Read_E2((char *)&Resolution,EBResolution,sizeof(Resolution));
        Read_E2((char *)&MaxCapacity,EBMaxCapacity,sizeof(MaxCapacity));
        Read_E2((char *)&Unit,EBUnit,sizeof(Unit));
        Read_E2((char *)&DecimalPoint,EBDecimalPoint,sizeof(DecimalPoint));

        Write_E2(EResolution,(char *)&Resolution,sizeof(Resolution));				  
        Write_E2(EMaxCapacity,(char *)&MaxCapacity,sizeof(MaxCapacity));
        Write_E2(EUnit,(char *)&Unit,sizeof(Unit));
        Write_E2(EDecimalPoint,(char *)&DecimalPoint,sizeof(DecimalPoint));
    
        for(Current_CH_Axle=CH3;Current_CH_Axle<CH6;Current_CH_Axle++)
        {
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero,(EBLC1Zero+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero));     
            Read_E2((char *)&Loadcell[Current_CH_Axle].Zero1,(EBLC1Zero1+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Zero1));
            Read_E2((char *)&Loadcell[Current_CH_Axle].Span,(EBLC1Span+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].Span));
            Read_E2((char *)&Loadcell[Current_CH_Axle].CalCapacity,(EBLC1CalCapacity+(Current_CH_Axle*4)),sizeof(Loadcell[Current_CH_Axle].CalCapacity));        

            Write_E2((ELC1Zero+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero,sizeof(Loadcell[Current_CH_Axle].Zero));     
            Write_E2((ELC1Zero1+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Zero1,sizeof(Loadcell[Current_CH_Axle].Zero1));
            Write_E2((ELC1Span+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].Span,sizeof(Loadcell[Current_CH_Axle].Span));
            Write_E2((ELC1CalCapacity+(Current_CH_Axle*4)),(char *)&Loadcell[Current_CH_Axle].CalCapacity,sizeof(Loadcell[Current_CH_Axle].CalCapacity));        
        }    
        Read_E2((char *)&Loadcell[CH6].Zero,EBLC6Zero,sizeof(Loadcell[CH6].Zero));     
        Read_E2((char *)&Loadcell[CH6].Zero1,EBLC6Zero1,sizeof(Loadcell[CH6].Zero1));
        Read_E2((char *)&Loadcell[CH6].Span,EBLC6Span,sizeof(Loadcell[CH6].Span));
        Read_E2((char *)&Loadcell[CH6].CalCapacity,EBLC6CalCapacity,sizeof(Loadcell[CH6].CalCapacity));        

        Write_E2(ELC6Zero,(char *)&Loadcell[CH6].Zero,sizeof(Loadcell[CH6].Zero));     
        Write_E2(ELC6Zero1,(char *)&Loadcell[CH6].Zero1,sizeof(Loadcell[CH6].Zero1));
        Write_E2(ELC6Span,(char *)&Loadcell[CH6].Span,sizeof(Loadcell[CH6].Span));
        Write_E2(ELC6CalCapacity,(char *)&Loadcell[CH6].CalCapacity,sizeof(Loadcell[CH6].CalCapacity));        
    }
    else if(loadcells_or_Voltage == Voltage)
    {
        Read_E2((char *)&VoltageCalib.REFvolt1,EE_BREFvolt1,sizeof(VoltageCalib.REFvolt1));
        Read_E2((char *)&VoltageCalib.REFvolt2,EE_BREFvolt2,sizeof(VoltageCalib.REFvolt2));    
        Read_E2((char *)&VoltageCalib.REF1Cnt,EE_BREF1Cnt,sizeof(VoltageCalib.REF1Cnt));
        Read_E2((char *)&VoltageCalib.REF2Cnt,EE_BREF2Cnt,sizeof(VoltageCalib.REF2Cnt));
        Read_E2((char *)&VoltageCalib.Ch1Ref1,EE_BCh1Ref1,sizeof(VoltageCalib.Ch1Ref1));
        Read_E2((char *)&VoltageCalib.Ch1Ref2,EE_BCh1Ref2,sizeof(VoltageCalib.Ch1Ref2));
        Read_E2((char *)&VoltageCalib.Ch2Ref1,EE_BCh2Ref1,sizeof(VoltageCalib.Ch2Ref1));
        Read_E2((char *)&VoltageCalib.Ch2Ref2,EE_BCh2Ref2,sizeof(VoltageCalib.Ch2Ref2));
        Read_E2((char *)&VoltageCalib.Ch3Ref1,EE_BCh3Ref1,sizeof(VoltageCalib.Ch3Ref1));
        Read_E2((char *)&VoltageCalib.Ch3Ref2,EE_BCh3Ref2,sizeof(VoltageCalib.Ch3Ref2));
        Read_E2((char *)&VoltageCalib.Ch4Ref1,EE_BCh4Ref1,sizeof(VoltageCalib.Ch4Ref1));
        Read_E2((char *)&VoltageCalib.Ch4Ref2,EE_BCh4Ref2,sizeof(VoltageCalib.Ch4Ref2));
        Read_E2((char *)&VoltageCalib.Ch5Ref1,EE_BCh5Ref1,sizeof(VoltageCalib.Ch5Ref1));
        Read_E2((char *)&VoltageCalib.Ch5Ref2,EE_BCh5Ref2,sizeof(VoltageCalib.Ch5Ref2));
        Read_E2((char *)&VoltageCalib.Ch6Ref1,EE_BCh6Ref1,sizeof(VoltageCalib.Ch6Ref1));
        Read_E2((char *)&VoltageCalib.Ch6Ref2,EE_BCh6Ref2,sizeof(VoltageCalib.Ch6Ref2));
        Read_E2((char *)&VoltageCalib.Offset_Volt,EE_BOffset_Volt,sizeof(VoltageCalib.Offset_Volt));
        voltage_Calib_write(); 
    }
}

/****************************************************************
Function name 	: void ChannelSelect(unsigned char ChannelNo)
Returns 		: None
Arguments		: unsigned char ChannelNo - Channel to be changed
Created by 		: author ?s name
Date created 	: date
Description 	: To Enable / Disable the given ADC channel.
Notes 			: Only one channel can be disabled.				  
****************************************************************/
void ChannelSelect(unsigned char ChannelNo)
{
	unsigned char EnDis = 0;
	unsigned int Blink = 0;	

//	if(ChannelNo == 1)
//		EnDis = Setup.LCED[CH1];
    if(ChannelNo == 0)  //Ver2.1
		EnDis = Application_Setting.LCED[CH1];
	if(ChannelNo == 1)
		EnDis = Application_Setting.LCED[CH2];
	else if(ChannelNo == 2)
		EnDis = Application_Setting.LCED[CH3];
	else if(ChannelNo == 3)
		EnDis = Application_Setting.LCED[CH4];
	else if(ChannelNo == 4)
		EnDis = Application_Setting.LCED[CH5];	
	else if(ChannelNo == 5)
		EnDis = Application_Setting.LCED[CH6];	
	while(1)
	{
		KeySense();
		if(MenuKey)
		{
			break;
		}
		if(UpKey)
		{
			if(EnDis++ >= 1)
				EnDis = 0;
		}
		if(EnterKey)
		{
//			if((EnDis == 0) && ((Setup.LCED[CH1] + Setup.LCED[CH2] + Setup.LCED[CH3] + Setup.LCED[CH4]) <= 1))	// (4 - one channel disabled) (1 - three channel Disabled)
//			{
//				if(((ChannelNo== 1) && (EnDis != Setup.LCED[CH1])) ||
//					((ChannelNo== 2) && (EnDis != Setup.LCED[CH2])) ||
//						((ChannelNo== 3) && (EnDis != Setup.LCED[CH3])) ||
//							((ChannelNo== 4) && (EnDis != Setup.LCED[CH4])))
//				{
//					LcdDisplay(BOTTOM,(char *)" 3  CH. DISABLED");
//					Delay_1sec(1);
//				}
//				else
//					break;
//			}
//			else
//			{
//				if(ChannelNo == 1)
//				{
//					Setup.LCED[CH1] = EnDis;
//					Write_E2(ELC1ED,(char*)&Setup.LCED[CH1],sizeof(Setup.LCED[CH1]));
//*/
//				}
                if(ChannelNo == 0)  //Ver2.1
				{
					Application_Setting.LCED[CH1] = EnDis;
					Write_E2(ELC1ED,(char*)&Application_Setting.LCED[CH1],sizeof(Application_Setting.LCED[CH1]));//Delay(2000);
				}
				if(ChannelNo == 1)
				{
					Application_Setting.LCED[CH2] = EnDis;
					Write_E2(ELC2ED,(char*)&Application_Setting.LCED[CH2],sizeof(Application_Setting.LCED[CH2]));//Delay(2000);
				}
				else if(ChannelNo == 2)
				{
					Application_Setting.LCED[CH3] = EnDis;
					Write_E2(ELC3ED,(char*)&Application_Setting.LCED[CH3],sizeof(Application_Setting.LCED[CH3]));//Delay(2000);
//                    if(EnDis == 0)
//					{
//                        Application_Setting.LCED[CH4] = EnDis;
//                        Write_E2(ELC4ED,(char*)&Application_Setting.LCED[CH4],sizeof(Application_Setting.LCED[CH4]));//Delay(2000);
//                    }
				}
				if(ChannelNo == 3) 
				{
                    Application_Setting.LCED[CH4] = EnDis;
                    Write_E2(ELC4ED,(char*)&Application_Setting.LCED[CH4],sizeof(Application_Setting.LCED[CH4]));//Delay(2000);
				}
				if(ChannelNo == 4) 
				{
                    Application_Setting.LCED[CH5] = EnDis;
                    Write_E2(ELC5ED,(char*)&Application_Setting.LCED[CH5],sizeof(Application_Setting.LCED[CH5]));//Delay(2000);
				}
				if(ChannelNo == 5) 
				{
                    Application_Setting.LCED[CH6] = EnDis;
                    Write_E2(ELC6ED,(char*)&Application_Setting.LCED[CH6],sizeof(Application_Setting.LCED[CH6]));
				}
				break;
//			}
		}
			if(EnDis == 0)
                LcdDisplay(BOTTOM,(char *)"             Dis");
			else
                LcdDisplay(BOTTOM,(char *)"             Enb");
	}	
	memcpy(LcdDispBuff+16,"                ",16);
	LcdDisplay(BOTTOM,LcdDispBuff+16);
	return;		
}

/****************************************************************
Function name 	: void ChannelConfiguration(void)
Returns 		: None
Arguments		: None
Created by 		: author ?s name
Date created 	: date
Description 	: To Perform the Channel configuration menu.
Notes 			: 				  
****************************************************************/
void ChannelConfiguration(void) //Ver2.1
{
	unsigned char CtNo = 0;
	ClearLcdDisplay(TOP_BOTTOM);
	memset(LcdDispBuff,' ',32);
	while(1)
	{
//		if((Setup.Channel1 + Setup.Channel2+ Setup.Channel3 + Setup.Channel4) == 2)	
//		{
				KeySense();
				if(MenuKey)
				{
					memset(LcdDispBuff+16,' ',16);                    
					break;
				}
				else if(UpKey)
				{
					if(CtNo++ >= 5)
						CtNo = 0;
                    if(CtNo == 2)
                        CtNo++;
				}
				else if(EnterKey)
				{
					ChannelSelect(CtNo);
					memset(LcdDispBuff+16,' ',16);
					if(CtNo++ >= 5)
						CtNo = 0;  
                    if(CtNo == 2)
                        CtNo++;
				}
				if(CtNo == 0)
                    LcdDisplay(TOP,(char *)"*   CHANNEL 1  *");	
				if(CtNo == 1)
                    LcdDisplay(TOP,(char *)"*   CHANNEL 2  *");	
				else if(CtNo == 2)
                    LcdDisplay(TOP,(char *)"*   CHANNEL 3  *");
//				if((Application_Setting.LCED[CH3] == 0) && (CtNo == 3))
//                    CtNo = 4;
                if(CtNo == 3)
                    LcdDisplay(TOP,(char *)"*   CHANNEL 4  *");
                if(CtNo == 4)
                    LcdDisplay(TOP,(char *)"*   CHANNEL 5  *");
                if(CtNo == 5)
                    LcdDisplay(TOP,(char *)"*   CHANNEL 6  *");
	}
}

unsigned int Numeric_Entry(unsigned int SourceNumBuff,unsigned char Length,unsigned char DpPos,unsigned int MinVal,unsigned int MaxVal)
{
	unsigned char EntryBuff[20] = {0};
	unsigned char ParameterBuff[20] = {0};
	unsigned char CopyBuff[20] = {0};
 
	unsigned int blink = CLEAR;	
    unsigned char digit = CLEAR;

//        memcpy(LCD_Buff2,"+(   )          ",16);
    memcpy((char*)&CopyBuff,LCD_Buff2,16);
    digit = Length-1;

    LcdDisplay(BOTTOM,(char *)&CopyBuff);	
    Enter_NxtFlag = CLEAR;
    Esc_NxtFlag = CLEAR;
	
    while(1)
	{
        memcpy(EntryBuff,CopyBuff,Length);

        if(blink < 100)
            EntryBuff[digit] = '_';
        if(blink > 200)
            blink=0;
        blink++;

        if(DpPos == 0)
        {
            memset(ParameterBuff,' ',16-Length);
            memcpy(ParameterBuff+16-Length,EntryBuff,Length);
            LcdDisplay(BOTTOM,(char *)&ParameterBuff);	
        }
	
		KeySense();
		if(MenuKey)
		{
			Esc_NxtFlag = SET; 
			return(0);
		}
		else if(UpKey)
		{
				if(CopyBuff[digit]>='9')
					CopyBuff[digit]=' ';
				else if(CopyBuff[digit] ==' ')
					CopyBuff[digit]='0';
				else
					CopyBuff[digit] = CopyBuff[digit]+ 1;

			blink=110;	
		}
		else if(LeftKey )
		{
            if(digit == 4)
                digit = 2;
            else if(0<digit)
                digit--;
            else
            {
                digit=(Length-1);
            }
		}
		else if(EnterKey)
		{
            memcpy(TempMobEntryBuff,(char *)&CopyBuff,16);
            return(1);
		}
	 }
}

void Get_Phone_No(void)
{
	unsigned int CheckSum_Value = 0;
	unsigned char NumberOrder = 0;
	unsigned char One_Time_Req = 0;  
	char CheckOrder[2]={0};
	
    unsigned char SetAck_Flag = 0;
	while(1)
	{
		Interrupt_Check();
		sprintf(LCD_Buff1,"Mobile No : %02d",NumberOrder+1);
		if(MobileNumRdy == 1)
		{
			memcpy(CheckOrder,MobileNumBuff,2);
			if(NumberOrder+1 == atoi(CheckOrder))
				MobileNumRdy = 2;
		}
		else if(MobileNumRdy == 2)
		{
//			memcpy(LCD_Buff2,"+(   )          ",16);
//			memcpy(LCD_Buff2+2,MobileNumBuff+2,3);
//			memcpy(LCD_Buff2+6,MobileNumBuff+5,10);
            
            memcpy(LCD_Buff2,"   -            ",16);
            memcpy(LCD_Buff2,MobileNumBuff+2,3);
			memcpy(LCD_Buff2+4,MobileNumBuff+5,12);
            
		}
		else
		{
			memcpy(LCD_Buff2," Please wait... ",16);
		}
		
		if(One_Time_Req == 0)
		{
			One_Time_Req = 1;
			sprintf((char*)&Protocol_Buff,"[MRD*%02d*",NumberOrder+1);
			CheckSum_Value = CalcLRC(Protocol_Buff+1,7);
			sprintf((char*)&Protocol_Buff+8,"%02d",CheckSum_Value);
			Protocol_Buff[10] = ']';
			Rx_ClearVar();
			Com2StringSend(Protocol_Buff,11);
		}
        KeySense();
		if(EnterKey)
		{
			if((Numeric_Entry(MobileNumBuff,16,0,0,0) != 0))
			{
//                LcdDisplay(TOP,TempMobEntryBuff);Delay1_1s(2);
				sprintf((char*)&Protocol_Buff,"[MWR*%02d*",NumberOrder+1);
				memcpy(Protocol_Buff+8,TempMobEntryBuff,3);
				Protocol_Buff[11] = '*';
				memcpy(Protocol_Buff+12,TempMobEntryBuff+4,12);
				CheckSum_Value = CheckSum(Protocol_Buff+1,23);
				sprintf((char*)&Protocol_Buff+24,"%02u",CheckSum_Value);
				Protocol_Buff[26] = ']';
				Rx_ClearVar();
				Com2StringSend(Protocol_Buff,27);
				One_Time_Req = 0;
				MobileNumRdy = 0;
				TimerCntr1 = SetAck_Flag = 1;
				while(1)
				{
                    //TimerCntr1++;
					if( (ReceivedFlag == 1) && (ucount == 8) )
					{
						if(!strncmp((char *)ReceiveBuff,"MWR OK",6))
							SetAck_Flag = 2;
						else
							SetAck_Flag = 3;
					}
					if(TimerCntr1 > 297)		// For Timeout after 3  seconds
					{
						TimerCntr1 = 0;
						SetAck_Flag = 3;
					}
					if(SetAck_Flag == 1)
						LcdDisplay(BOTTOM," Please wait... ");
					else if(SetAck_Flag == 2)
					{
						LcdDisplay(BOTTOM,"   Set Success  ");
						TimerCntr1 = 0;
						SetAck_Flag = 0;
						Rx_ClearVar();						
						Delay_ms(200);Delay_ms(200);Delay_ms(200);
						break;
					}
					else if(SetAck_Flag == 3)
					{
						LcdDisplay(BOTTOM,"   Set Failed   ");
						SetAck_Flag = 0;
						Rx_ClearVar();
						Delay_ms(200);Delay_ms(200);Delay_ms(200);
						
						break;
					}
//					if( (TimeOut_Cnt == 0) && (Skip_TimeOut == 0) )
//									{				
//										Clear234LCD();
//										return;
//									}
				}
			}
		}
		else if(MenuKey)
		{
			MobileNumRdy = 0;
			TimerCntr1 = 0;
			SetAck_Flag = 0;
			LcdDisplay(BOTTOM,"                ");
            break;
		}
		else if(UpKey)
		{
			
			MobileNumRdy = 0;
			One_Time_Req = 0;
      NumberOrder++;
			if(NumberOrder > 2)
				NumberOrder = 0;			
		}
//  if( (TimeOut_Cnt == 0) && (Skip_TimeOut == 0) )
//											{				
//												Clear234LCD();
//												return;
//											}  
		LcdDisplay(TOP,LCD_Buff1);
		LcdDisplay(BOTTOM,LCD_Buff2);
		
	}
}

void View_SMS_Count(void)
{
	unsigned int CheckSum_Value = 0,value = 0;
	unsigned char NumberOrder = 0;
	unsigned char One_Time_Req = 0;  
	char CheckOrder[2]={0};
	char valuee[15] = {0};
	
  unsigned char SetAck_Flag = 0;
	
	
	
	while(1)
	{
		Interrupt_Check();
		
		if(MobileNumRdy == 1)
		{
			//valuee[0]=NumberOrder = MobileNumBuff[0];
			memcpy(valuee,MobileNumBuff,10);
			memcpy(LCD_Buff1,"                ",16);
			memcpy(LCD_Buff1+9,valuee,7);
			memcpy(LCD_Buff2,"Skipped Cnt:    ",20);
			memcpy(LCD_Buff2+12,valuee+8,3);
		}
		else
		{
			memcpy(LCD_Buff2,"   Please wait...   ",20);
		//	memcpy(LCD_Buff2,ReceiveBuff,20);
		}
		
		if(One_Time_Req == 0)
		{
			One_Time_Req = 1;

			sprintf((char*)&Protocol_Buff,"[VSCRD*");
			CheckSum_Value = CheckSum(Protocol_Buff+1,6);
			sprintf((char*)&Protocol_Buff+7,"%02u",CheckSum_Value);
			Protocol_Buff[9] = ']';
			Rx_ClearVar();
			Com2StringSend(Protocol_Buff,10);
		}
		
        KeySense();
		 if(MenuKey)
		{
			MobileNumRdy = 0;
			TimerCntr1 = 0;
			SetAck_Flag = 0;
			
      break;
		}
   
		LcdDisplay(TOP,LCD_Buff1);
		LcdDisplay(BOTTOM,LCD_Buff2);
	}

}

void View_GPRS_LIMIT(void)
{
	unsigned int CheckSum_Value = 0,value = 0;
	unsigned char NumberOrder = 0;
	unsigned char One_Time_Req = 0;  
	char CheckOrder[2]={0};
	unsigned char valuee[17] = {0},valuee1[17] = {0};
	
  unsigned char SetAck_Flag = 0;
	
	
	
	while(1)
	{
		Interrupt_Check();
		
		if(MobileNumRdy == 1)
		{
			//memcpy(LCD_Buff1,"   Per Day (MB)     ",20);
			//valuee[0]=NumberOrder = MobileNumBuff[0];
			memcpy(valuee,MobileNumBuff,15);
			memcpy(valuee1,valuee,7);
			Put_DecimalPoint_Display(valuee1,7,3);
			memcpy(LCD_Buff1,"                ",16);
			memcpy(LCD_Buff1+3,valuee1,8);
			memcpy(LCD_Buff1+11,valuee+7,5);
			memcpy(LCD_Buff2,"Skipped Cnt:    ",16);
			memcpy(LCD_Buff2+12,valuee+12,3);
			
		}
		else
		{
			memcpy(LCD_Buff1,"   Per Day (MB)     ",20);
			memcpy(LCD_Buff2,"   Please wait...   ",20);
		//	memcpy(LCD_Buff2,ReceiveBuff,20);
		}
		
		if(One_Time_Req == 0)
		{
			One_Time_Req = 1;

			sprintf((char*)&Protocol_Buff,"[VGLRD*");
			CheckSum_Value = CheckSum(Protocol_Buff+1,6);
			sprintf((char*)&Protocol_Buff+7,"%02u",CheckSum_Value);
			Protocol_Buff[9] = ']';
			Rx_ClearVar();
			Com2StringSend(Protocol_Buff,10);
		}
		KeySense();
	
		if(MenuKey)
		{
			MobileNumRdy = 0;
			TimerCntr1 = 0;
			SetAck_Flag = 0;
			
      break;
		}

		LcdDisplay(TOP,LCD_Buff1);
		LcdDisplay(BOTTOM,LCD_Buff2);
	}
}

void Put_DecimalPoint_Display(unsigned char *SourceBuff,unsigned char Length,unsigned char DpPos)
{
	unsigned char Divide_and_Separate_Buff[20] = {0};
	unsigned char SourceData[20]; 
	unsigned char Clear_Length;
	unsigned int temp = 0;
        
	memcpy(Divide_and_Separate_Buff,SourceBuff,Length);
    
	if(DpPos == 0)
	{
	  	temp = atoi((const char *)&Divide_and_Separate_Buff);
		if(Length == 6)
			sprintf((char *)&SourceBuff," %6d",temp);
		else if(Length == 7)
			sprintf((char *)&SourceBuff," %7d",temp); 
	}
	else
	{   
		memcpy(SourceData,Divide_and_Separate_Buff,Length-DpPos);
	  memcpy(SourceData+(Length-DpPos+1),Divide_and_Separate_Buff+(Length-DpPos),DpPos);
		if(CursorIndex == 42)
	    SourceData[Length-DpPos] = ':';
		else
			SourceData[Length-DpPos] = '.';
       		
		for(Clear_Length=0;Clear_Length<(Length-DpPos-1);Clear_Length++)
		{
			if(*(SourceData+Clear_Length) > '0')
				break;
			else if(*(SourceData+Clear_Length) == '0')
				*(SourceData+Clear_Length) = ' ';
		}
		memcpy(SourceBuff,SourceData,Length+1);
	}    	
}