#include <REGX52.h>
sbit DOUT = P3^7;	
sbit CLK  = P3^6;	
sbit DIN  = P3^4;	
sbit CS   = P3^5;	

sbit button = P3^3;
#define Display P0
unsigned char code Code7segCatot[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d,
                                     0x7d, 0x07, 0x7f, 0x6f, 0x77};
//thu tu: giay phut gio nam thang ngay									
unsigned char Time[6] = {0x10, 0x01, 0x16, 0x24, 0x10, 0x07};

unsigned char irKey = 0;
unsigned long bitPattern = 0;
unsigned long newKey = 0;
unsigned long pre_newKey = 0;
unsigned char timerValue;
unsigned char msCount = 0;
char pulseCount = 0;
void HenGio(int time);
void DieuKhien();
void CamBienAS();
void delay(int time);
void SPI_Start(void);
void SPI_Write(unsigned char dat);
unsigned int SPI_Read(void);
unsigned int Read_AD_Data(unsigned char cmd);
void Timer0_Init(void);
unsigned char Decode_IRKey(unsigned char key);
void HienThiSoChay(int demA, int demB);
int main(void)
{
	P2 = 0xff;
		Timer0_Init();
	EA = 1; // Enable the Global Interrupt bit
	while(1)
	{
		
		CamBienAS();
		DieuKhien();
		if(button == 1)
			delay(200);
			if(button == 0)
				HienThiSoChay(5,0);
			

	}
}

void HienThiSoChay(int demA, int demB) {
  int tanso;
  int dem;
  for (dem = demA; dem >= demB; dem--) {
 
    
      P2_2 = 0; P2_3 = 0; P2_4 = 0;
      Display = Code7segCatot[dem];
      delay(100000);			


  }
	P2_5 = ~P2_5;
}

void DieuKhien()
{
	irKey = Decode_IRKey(newKey);
			if (irKey == 1) 
				P2_7 = 0;
			if(irKey==4)
				P2_7 = 1;
}
void CamBienAS()
{
	unsigned int datAS = Read_AD_Data(0xA4);
		if(datAS>50)
			P2_0 = 1;
		else
			P2_0 = 0;
		
		P0 = 0;
}
void delay(int time)
{
	while(--time);
}

void SPI_Start(void)
{
	CLK = 0;
	CS  = 1;
	DIN = 1;
	CLK = 1;
	CS  = 0;		
}

void SPI_Write(unsigned char dat)
{
	unsigned char i;
	CLK = 0;
	for(i=0; i<8; i++)
	{
		DIN = dat >> 7;  
		dat <<= 1;
		CLK = 0;			

		CLK = 1;

	}
}

unsigned int SPI_Read(void)
{
	unsigned int i, dat=0;
	CLK = 0;
	for(i=0; i<12; i++)		
	{
		dat <<= 1;

		CLK = 1;
		CLK = 0;

		dat |= DOUT;

	}
	return dat;	
}

unsigned int Read_AD_Data(unsigned char cmd)
{
	unsigned char i;
	unsigned int AD_Value;
	CLK = 0;
	CS  = 0;
	SPI_Write(cmd);
	for(i=6; i>0; i--); 	
	CLK = 1;	 
	delay(10);
	delay(10);
	CLK = 0;
	delay(10);
	delay(10);
	AD_Value=SPI_Read();
	CS = 1;
	return AD_Value;	
}

unsigned char Decode_IRKey(unsigned char key){
	unsigned char returnValue = 0;
	switch (key)
	{
	case 0xFF30CF:
		returnValue = 1;
		break;
	case 0xFF10EF:
		returnValue = 4;
		break;
	}
	return returnValue;
}
void timer0_isr() interrupt 1
{
	if (msCount < 50)
		msCount++;
	TH0 = 0xFC; // Reload the timer value for 1ms Delay
	TL0 = 0x67;
}
void externalIntr0_ISR() interrupt 0
{
	timerValue = msCount;
	msCount = 0;
	TH0 = 0xFC; // Reload the timer value for 1ms Delay
	TL0 = 0x67;
	pulseCount++;
	if ((timerValue >= 50)) // If the pulse width is greater than 50ms, this will mark the SOF
	{

	pulseCount = -2; // First 2 counts needs to be skipped hence pulse count is set to -2
	bitPattern = 0;
	}
	else if ((pulseCount >= 0) && (pulseCount < 32)) //Accumulate the bit values between 0-31.
	{
	if (timerValue >= 2) // pulse width greater than 2ms is considered as LOGIC1
		bitPattern |= (unsigned long)1 << (31 - pulseCount);
	}
	else if (pulseCount >= 32) // This will mark the End of frame as 32 pulses are received
	{
		newKey = bitPattern; // Copy the newKey(patter) and set the pulse count to 0;
		pulseCount = 0;
	}
}

void Timer0_Init(){
	TMOD |= 0x01; // Timer0 MODE1(16-bit timer)
	TH0 = 0xFC;
	TL0 = 0x67;
	// Timer value for 1ms at 11.0592Mhz clock
	TR0 = 1; // Start the Timer
	ET0 = 1; // Enable the Timer0 Interrupt
	IT0 = 1; // Configure INT0 falling edge interrupt
	EX0 = 1; // Enable the INT0 External Interrupt
}