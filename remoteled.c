/*
0xFFA25D: CH-
0xFF629D: CH
0xFFE21D: CH+
0xFF22DD: PREV
0xFF02FD: NEXT
0xFFC23D: PLAY/PAUSE
0xFFE01F: VOL-
0xFFA857: VOL+
0xFF906F: EQ
0xFF6897: 0
0xFF9867: 100+
0xFFB04F: 200+
0xFF30CF: 1
0xFF18E7: 2
0xFF7A85: 3
0xFF10EF: 4
0xFF38C7: 5
0xFF5AA5: 6
0xFF42BD: 7
0xFF4AB5: 8
0xFF52AD: 9
*/
#include <REGx51.H>
unsigned char irKey = 0;
unsigned long bitPattern = 0;
unsigned long newKey = 0;
unsigned long pre_newKey = 0;
unsigned char timerValue;
unsigned char msCount = 0;
char pulseCount = 0;
unsigned char Code7segCatot[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 
                                 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7C, 
                                 0x39, 0x5E, 0x79, 0x71};

void delay_us(unsigned int us_count)
{  
	while(us_count!=0)
	{
		us_count--;
  }
}
void Timer0_Init(void);
unsigned char Decode_IRKey(unsigned char key);
char led1 = 1, led2 = 1; led3 = 1;
void main(){
	P2 = 0xff;
	Timer0_Init();
	EA = 1; // Enable the Global Interrupt bit
	while (1)
	{
		P0 = 0;
		
			irKey = Decode_IRKey(newKey);
			if (irKey == 1) 
				P2_0 = 0;
			if(irKey==2)
					P2_2= 0;
      if(irKey==3)
					P2_4=0;
			if (irKey == 4) 
				P2_0 = 1;
			if(irKey==5)
					P2_2= 1;
      if(irKey==6)
					P2_4=1;

		

	}
}


unsigned char Decode_IRKey(unsigned char key){
	unsigned char returnValue = 0;
	switch (key)
	{
	case 0xFF30CF:
		returnValue = 1;
		break;
	case 0xFF18E7:
		returnValue = 2;
		break;
	case 0xFF7A85:
		returnValue = 3;
		break;
	case 0xFF10EF:
		returnValue = 4;
		break;
	case 0xFF38C7:
		returnValue = 5;
		break;
	case 0xFF5AA5:
		returnValue = 6;
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