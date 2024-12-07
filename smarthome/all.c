#include <REGX52.h>
sbit DOUT = P3^7;	
sbit CLK  = P3^6;	
sbit DIN  = P3^4;	
sbit CS   = P3^5;	
void HenGio(int time);
void DieuKhien();
void CamBienAS();
void delay(int time);
void remote();
void SPI_Start(void);
void SPI_Write(unsigned char dat);
unsigned int SPI_Read(void);
unsigned int Read_AD_Data(unsigned char cmd);
int main(void)
{   P2 = 0xFF;         
    Timer0_Init();     
    EA = 1; 
	while(1)
	{
		unsigned int datAS = Read_AD_Data(0xA4);
		if(datAS>20)
			P2_0 = 1;
		else
			P2_0 = 0;
		remote();
	}
}

//void HenGio(int time)
//{
//	
//}

//void DieuKhien()
//{
//	
//}
//void CamBienAS()
//{
//	
//}
void remote(){          
    {
			irKey=Decode_IRKey(newKey);
			if(irKey==1)
				P2_0=0;
			if(irKey==2)
				P2_1=0;
			if(irKey==3)
				P2_2=0;
			if(irKey==4)
				P2_0=1;
			if(irKey==5)
				P2_1=1;
			if(irKey==6)
				P2_2=1;
			if(P3_2==0)
				P2_4=0;
			if(P3_1==0)
				P2_4=1;
		}    
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
