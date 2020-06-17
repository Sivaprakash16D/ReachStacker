
/*
sbit WP= P1^0;
sbit SDA = P1^7; //used in E2Prom writing
sbit SCL = P1^6;
sbit Carry = PSW^7;
*/

extern void StartCmd();
//void Write_E2(const unsigned long Address,const char* Data,const int NoofBytes);
//void Read_E2(const char* Data,const unsigned long Address,const int NoofBytes);

extern void Send_E2(char);
extern void SerClock();
extern void SerAck();
extern void SerNoAck();
extern void StopCmd();
extern unsigned char Get_E2();
extern void E2Delay();
extern void E2SDelay();
