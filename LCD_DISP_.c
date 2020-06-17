#include <p30f6012A.h>
#include <string.h>
#include "LCD_DISP.h"

void InitLCDPortIO(void)
{
  TRISD	 &= 0xff00;
}
/*******************As per in MW5004***********************/

void LcdDisplay(char Mode,char *Buff)
{
	unsigned char i;

	LCD_RW=0;Nop();
	LCD_RS=0;Nop();
//    LCD_RS=1;Nop();//Need to set this pin always high for OLED display
    
	if(Mode==TOP)
	LCDDATA.all = 0x80;
	else if(Mode==BOTTOM)
	LCDDATA.all = 0xC0;
	SendDataToLCD();
//	LCDDATA.all = 0x00;//To display NULL character before sending original data as per datasheet
//	SendDataToLCD();
    
	for(i=0;i<16;i++)
	{
		LCD_RS=1;Nop();
		LCD_RW=0;Nop();

		if(Buff[i]=='\0')
			Buff[i]=' ';			
		
		LCDDATA.all = Buff[i];
		SendDataToLCD();
		LCD_RS=0;Nop();
		LCD_RW=1;Nop();
	//	LCDDelay(25);
	}
}

void ClearLcdDisplay(char Mode)
{
    memcpy(LcdDispBuff,"                ",16);
	memcpy(LcdDispBuff+16,"                ",16);
	if(Mode==TOP || Mode == TOP_BOTTOM)
		LcdDisplay(TOP,LcdDispBuff);
	if(Mode == BOTTOM || Mode == TOP_BOTTOM)
		LcdDisplay(BOTTOM,LcdDispBuff+16);
}	

void SendDataToLCD()
{
	LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
	LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
	LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
	LD4 = LCDDATA.LCDbit.LcdBit7;Nop();

	LCD_CS=1;
	LCDDelay(LCD_DISPLAY_DLY);
	LCD_CS=0;
	LCDDelay(LCD_DISPLAY_DLY);
	
	LD1 = LCDDATA.LCDbit.LcdBit0;Nop();
	LD2 = LCDDATA.LCDbit.LcdBit1;Nop();
	LD3 = LCDDATA.LCDbit.LcdBit2;Nop();
	LD4 = LCDDATA.LCDbit.LcdBit3;Nop();

	LCD_CS=1;
	LCDDelay(LCD_DISPLAY_DLY);
	LCD_CS=0;
	LCDDelay(LCD_DISPLAY_DLY);
}

void LCDDelay(unsigned int DVal)
{
	while(DVal-- > 0)
	{
		//Nop();
	}
}

void Busy(void)
{
    unsigned char flag=0xf0,flag1,flag2;
    LCD_RS=0;LCD_CS=0;LCD_RW=1;
	while ((flag&0x80)==0x80)
	{	
        LCDDATA.all =0xf0;
        LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
        LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
        LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
        LD4 = LCDDATA.LCDbit.LcdBit7;Nop();
            LCD_CS=1;
            Nop();
            flag1=PORTD;
            LCD_CS=0;
        LCDDATA.all =0xf0;
        LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
        LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
        LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
        LD4 = LCDDATA.LCDbit.LcdBit7;Nop();
            LCD_CS=1;
            Nop();
            flag2=PORTD;
            LCD_CS=0;
            flag = 	((flag1&0xF0)+(flag2>>4));	//	//***
	}
}

void InitLCDDisplay(void)
{
/*  
 * Following is the reset sequence of LCD.
  
 *  Send the first init value (0x30)
 *  Send second init value (0x30)
 *  Send third init value (0x30)
 *  Select bus width (0x30 - for 8-bit and 0x20 for 4-bit)
 */
    InitLCDPortIO();
//initialises the display
	LCD_RW	=0;	Nop();
	LCD_RS	=0;	Nop();
    LCD_CS 	=0;	Nop();
	LCDDelay(LCD_INIT_DLY1);
 
	LCDDATA.all =0x83;
	LCDDATA.all =0x30;
	LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
	LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
	LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
	LD4 = LCDDATA.LCDbit.LcdBit7;Nop();
	LCDDelay(LCD_INIT_DLY1);

	LCD_CS=1;
	LCDDelay(LCD_INIT_DLY2);
	LCD_CS=0;
	LCDDelay(LCD_INIT_DLY2);

	LCDDATA.all =0x83;
	LCDDATA.all =0x30;
	LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
	LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
	LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
	LD4 = LCDDATA.LCDbit.LcdBit7;Nop();
	LCDDelay(LCD_INIT_DLY1);
    
	LCD_CS=1;
	LCDDelay(LCD_INIT_DLY2);
	LCD_CS=0;
	LCDDelay(LCD_INIT_DLY2);
  
	LCDDATA.all =0x83;
	LCDDATA.all =0x30;
	LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
	LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
	LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
	LD4 = LCDDATA.LCDbit.LcdBit7;Nop();
	LCDDelay(LCD_INIT_DLY1);    
    
	LCD_CS=1;
	LCDDelay(LCD_INIT_DLY2);
	LCD_CS=0;
	LCDDelay(LCD_INIT_DLY2);

	LCDDATA.all =0x82;
	LCDDATA.all =0x20;
	LD1 = LCDDATA.LCDbit.LcdBit4;Nop();
	LD2 = LCDDATA.LCDbit.LcdBit5;Nop();
	LD3 = LCDDATA.LCDbit.LcdBit6;Nop();
	LD4 = LCDDATA.LCDbit.LcdBit7;Nop();
	LCDDelay(LCD_INIT_DLY1);    
	LCDDelay(LCD_INIT_DLY1);    
    
	LCD_CS=1;
	LCDDelay(LCD_INIT_DLY2);
	LCD_CS=0;
	LCDDelay(LCD_INIT_DLY2);
    
	LCDDATA.all =0x28;  //Function Set: 4-bit, 2 Line, 5x7 Dots
	SendDataToLCD();
	LCDDelay(LCD_INIT_DLY2);

	LCDDATA.all =0x14;  //Move cursor right by one character
	SendDataToLCD();
	LCDDelay(LCD_INIT_DLY2);
    
	LCDDATA.all =0x0c;  //	Display on Cursor off
	SendDataToLCD();
	LCDDelay(LCD_INIT_DLY2);

	LCDDATA.all = 0x06; //	Entry Mode
	SendDataToLCD();
	LCDDelay(LCD_INIT_DLY2);

	LCDDATA.all =0x01;  //	Clear Display (also clear DDRAM content)
	SendDataToLCD();
	LCDDelay(LCD_INIT_DLY2);

	LCDDATA.all =0x80;  //	Set DDRAM address or cursor position on display
	SendDataToLCD();
	LCDDelay(LCD_INIT_DLY2);
    LCDDelay(LCD_INIT_DLY1);LCDDelay(LCD_INIT_DLY1);
	LCDDelay(LCD_INIT_DLY1);
	return;

}
void TwoLineDisplay(char * FirstLineData,char * SecondLineData,unsigned char DelayTime)
{
    LcdDisplay(TOP,FirstLineData);
    LcdDisplay(BOTTOM,SecondLineData);
    if(DelayTime != 0)
        Delay1_1s(DelayTime);
}

