/*****************************************************************
 * Driver for  LCD Module (16 characters by 2 lines )
 4-Bit Data Line
*****************************************************************/

#define TOP 1
#define BOTTOM 2
#define TOP_BOTTOM 3

#define LCD_INIT_DLY1   15000
#define LCD_INIT_DLY2   1000
#define LCD_DISPLAY_DLY 100

#define LD4  _RD7
#define LD3	 _RD6
#define LD2	 _RD5
#define LD1	 _RD4

#define LCD_RW _RD2
#define LCD_RS _RD1
#define LCD_CS _RD3

union {
	struct {
		unsigned char LcdBit0 : 1 ;
		unsigned char LcdBit1 : 1 ;
		unsigned char LcdBit2 : 1 ;
		unsigned char LcdBit3 : 1 ;
		unsigned char LcdBit4 : 1 ;
		unsigned char LcdBit5 : 1 ;
		unsigned char LcdBit6 : 1 ;
		unsigned char LcdBit7 : 1 ;
	} LCDbit ;
	unsigned char all ;
}LCDDATA ;


void InitLCDDisplay(void);
void LcdDisplay(char Mode,char *Buff);
void SendDataToLCD(void);
void LCDDelay(unsigned int DVal);
void ClearLcdDisplay(char);
void Busy(void);

extern void Delay_ms(unsigned char);
extern char LcdDispBuff[40];				// Used for send the characters to LCD 


