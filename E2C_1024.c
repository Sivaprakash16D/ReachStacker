#include "p30fxxxx.h"
//#include <string.h>

void Send_FRAM(char);
void SerialClock(void);
void SendAddress_FRAM(long);
unsigned char Read_FRAM(void);

//void Write_E2(unsigned long,char*,int);
//void Read_E2(char*,unsigned long,int);

void Write_E2(const unsigned long Address,char* Data,const int NoofBytes);
void Read_E2(char* Data,const unsigned long Address,const int NoofBytes);

void Write_FRAM(long,char*,unsigned char);
void Read_FRAM1(char*,long,unsigned char);
void FRAMInit(void);
void FRAMDelay(void);
void WriteEnable_Cmd(void);

#define SCLK	_RG2			//FRAM_SCK
#define SDI		_RF6			//FRAM_MOSI//write
#define SDO		_RG3			//FRAM_MISO//Read
#define CS		_RD9			//FRAM_SEL
#define WriteProtect	_RD8	//FRAM_WP

/* UPGRADE ADD-ON CARD LCD I/O PINS inter change 19/02/2014 */
/*#define SCLK	_RD7
#define SDI		_RD5
#define SDO		_RD6
#define CS		_RD3
#define WriteProtect	_RD4
*/

#define WriteEnable		0x06
#define WriteDisable	0x04
#define ReadStatusReg	0x05
#define WriteStatusReg	0x01
#define Read			0x03
#define FastRead		0x0B
#define Write			0x02
#define Sleep_Code		0xB9
#define ReadDeviceID	0x9F
	

void FRAMInit(void)
{
	SCLK = 0;
	CS = 1;
	CS = 0;
	Send_FRAM(0x06);
	Send_FRAM(0x01);
	Send_FRAM(0x40);
	CS = 1;
	Nop();
	Nop();
}

void FRAMDelay(void)
{
//	Nop();
//	Nop();
//	Nop();
//	Nop();
//	Nop();
}

void SerialClock(void)
{
	SCLK = 1;
//	E2Delay();
//	FRAMDelay();
	SCLK = 0;
//	E2Delay();
//	FRAMDelay();
}

void Send_FRAM( char data)
{
	unsigned char i =0;
	for(i=0;i<8;i++)
	{		
		if(((data) & 0x80) == 0x80)
			SDI = 1;		
		else
			SDI = 0;
		Nop();
		SerialClock();		
		(data) <<= 1;
	}
}

void WriteEnable_Cmd(void)
{
		CS = 0;
		Send_FRAM(WriteEnable);
		CS = 1;
}

void SendAddress_FRAM(long Address)
{
	unsigned char Byte1=0,Byte2=0,Byte3=0;
	Byte1 = (0xFF & Address);
	Byte2 = (0xFF & (Address >> 8)) ;
	Byte3 = (0xFF & (Address >> 16));
	Send_FRAM(Byte3);
	Send_FRAM(Byte2);
	Send_FRAM(Byte1);
}

unsigned char Read_FRAM(void)
{
	unsigned char i=0,Data = 0x00;
	for(i=0;i<8;i++)
	{
		
		if(SDO)
			Data |= 1;
		else
			Data |= 0;
		SerialClock();
		if(i<7)
			Data <<= 1;
	}
	return(Data);
}

void Write_E2(const unsigned long Address,char* Data,const int NoofBytes)
{
	int i = 0;
//    char * dataptr;
	unsigned char Byte = 0x00;
//    dataptr = &Data;
 	WriteEnable_Cmd();
	CS = 0;
	Send_FRAM(Write);
	SendAddress_FRAM(Address);
	for(i=0;i<NoofBytes;i++)
	{
		Byte =(unsigned char) (0xFF & (*(Data+i))); 		
		Send_FRAM(Byte);
	}
	CS = 1;
}

void Read_E2(char* Data,const unsigned long Address,const int NoofBytes)
{
	int i =0 ;
//    char * dataptr;
    int orig_data = 0;
//    dataptr = &Data;
	CS = 0;
	Send_FRAM(Read);
	SendAddress_FRAM(Address);
	for(i=0;i<NoofBytes;i++)
	{
		*(Data+i) = Read_FRAM();
	}
	CS = 1;
}
