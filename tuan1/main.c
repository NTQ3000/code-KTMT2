#include <reg51.h>

void delay_ms(unsigned int time){
	unsigned int x;
	for(x=1;x<=time;x++){
		TMOD=0x11;
		TH0=55536/256;
		TL0=55536%256;
		TR0=1;
		while(!TF0);
		TF0=TR0=0;
	}
}	

void main()
{
  unsigned char i;

  while(1)
  {
		//SANGDAN
    P0=0x00;
    for(i = 0; i < 8; i++)
    {
      P0 = (P0<<1) | 1;
      delay_ms(100);
    }

    //TATDAN
    for(i=0;i<8;i++)
    {
      P0 = P0<<1;
      delay_ms(100);
    }
  }
}