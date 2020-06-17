#include <p30f6012A.h>
#include <string.h>
#include <stdlib.h>
/*
#define	KEY1				_RB12
#define KEY2				_RB13
#define	KEY3				_RB14
#define KEY4				_RB15
*/

#define ON 1
#define OFF 0

#define LOW 0
#define HIGH 1

#define	PORT1_2_INPUTROW 				_RG0
#define PORT1_3_INPUTROW 				_RG1
#define	PORT1_4_OUTPUTCOL				_RG12
#define PORT1_5_OUTPUTCOL				_RG14

#define	KEY1_Direction		_TRISG0
#define	KEY2_Direction		_TRISG1
#define	KEY3_Direction		_TRISG12
#define	KEY4_Direction		_TRISG14



unsigned char MenuKey = 0;
unsigned char EnterKey = 0;
unsigned char TareKey = 0;
unsigned char UpKey = 0;
unsigned char LeftKey = 0;		
unsigned char EscKey = 0;
unsigned char KeyRead = 1;
unsigned char KeyReadClear = 1;

void KeySense(void);
unsigned long TimeOut_Cnt = 0;
unsigned long TimeOut_Cnt1 = 0;
/*
void KeySense(void)
{
	if(KeyRead == 1) 
	{
		TareKey=0;UpKey=0;LeftKey=0;EscKey=0;MenuKey=0;EnterKey = 0;

		if(KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1)
		{
			KeyReadClear=1;
		}	

	}
	if((KEY1 == 0 || KEY2 == 0 || KEY3 == 0 || KEY4 == 0) && KeyReadClear == 1)
	{
		
		KeyReadClear =0;
		if(!KEY1)
		{
			MenuKey=1; 
			EscKey=1;
		}

		if(!KEY2)
		{
			TareKey=1;
			EnterKey = 1;
		}

		if(!KEY3)	
		{	
			UpKey=1;
		}
  
		if(!KEY4)
		{			
		  	LeftKey=1;
		}
  }
}
*/

/* Matrix Keypad */

void KeySense(void)
{
	static char Cnt; 
	Cnt=0;
	while(1)
	{
		EnterKey = MenuKey = LeftKey = UpKey = EscKey = TareKey = 0;
		PORT1_4_OUTPUTCOL = OFF;Nop();Nop();Nop();
		PORT1_5_OUTPUTCOL = ON;Nop();Nop();Nop();					
		if(PORT1_2_INPUTROW == LOW || PORT1_3_INPUTROW == LOW)
		{
			if(PORT1_2_INPUTROW == HIGH && PORT1_3_INPUTROW == LOW && KeyRead ==1)
			{
				PORT1_4_OUTPUTCOL = OFF;Nop();Nop();Nop();
				PORT1_5_OUTPUTCOL = OFF;Nop();Nop();Nop();
				UpKey=1;					
				KeyRead = 0;	
				TimeOut_Cnt = 0;			
				break;	
			}
			else if(PORT1_2_INPUTROW == LOW && PORT1_3_INPUTROW == HIGH && KeyRead ==1)	
			{
				PORT1_4_OUTPUTCOL = OFF;Nop();Nop();Nop();
				PORT1_5_OUTPUTCOL = OFF;Nop();Nop();Nop();
				MenuKey=1;
				EscKey=1;
				KeyRead = 0;
				TimeOut_Cnt = 0;				
				break;
			}
		}	
		if(PORT1_2_INPUTROW == HIGH && PORT1_3_INPUTROW == HIGH ) Cnt++;
		PORT1_4_OUTPUTCOL = ON;Nop();Nop();Nop();
		PORT1_5_OUTPUTCOL = OFF;Nop();Nop();Nop();	
		if(PORT1_2_INPUTROW == HIGH && PORT1_3_INPUTROW == LOW && KeyRead ==1) 
        {
			PORT1_4_OUTPUTCOL = OFF;Nop();Nop();Nop();
			PORT1_5_OUTPUTCOL = OFF;Nop();Nop();Nop();
			LeftKey=1;		
			KeyRead = 0;
			TimeOut_Cnt = 0;		
			break;	
		}
		else if(PORT1_2_INPUTROW == LOW && PORT1_3_INPUTROW == HIGH && KeyRead ==1) {			

			PORT1_4_OUTPUTCOL = OFF;Nop();Nop();Nop();
			PORT1_5_OUTPUTCOL = OFF;Nop();Nop();Nop();
			EnterKey=1;
			TareKey=1;		
			KeyRead = 0;
			TimeOut_Cnt = 0;			
			break;
		}		
		if(PORT1_2_INPUTROW == HIGH && PORT1_3_INPUTROW == HIGH ) Cnt++;
		PORT1_4_OUTPUTCOL = OFF;Nop();Nop();Nop();
		PORT1_5_OUTPUTCOL = OFF;Nop();Nop();Nop();
		if (Cnt==2) {
			KeyRead =1;}
		break;
	}
}

