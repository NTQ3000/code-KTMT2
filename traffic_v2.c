#include <main.h>
int chedo;
int chuyenhuong;
unsigned char Code7seg[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};
int dem;
int tanso;
void delay_ms(unsigned int time)
{	
	/* f = 12M hz*/
	/*f vào timer 1 = 12M/2 = 6M*/
	/*T = 1/6M s*/
	/* 6000 --> 1ms*/
	/*59536--> 65535 -> 0 = 1ms*/
	while(time--){
		TMOD = 0x01;
		TH1 = 0xE8;
		TL1 = 0x90;
		TR1 = 1;
		while(!TF1);
		TF1 = 0;
	}
}
void Traffic_Auto(void)
{
	StatusTraffic = 0x21;

    // delay30s
    for (dem = 30; dem >= 3; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {  
            dvi_DT = 1;
            Display = Code7seg[dem % 10];
            delay_ms(10);
            dvi_DT = 0;
            dvi_BN = 1;
            Display = Code7seg[(dem - 3)%10];
            delay_ms(10);
            dvi_BN = 0;
            chuc_DT = 1;
            Display = Code7seg[dem / 10];
            delay_ms(10);
            chuc_DT = 0;
            chuc_BN = 1;
            Display = Code7seg[(dem - 3)/10];
            delay_ms(10);
            chuc_BN = 0;
        }
    }
	StatusTraffic = 0x9;
    // delay3s
    for (dem = 2; dem >= 0; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {

            dvi_DT = 1;
            Display = Code7seg[dem % 10];
            delay_ms(10);
            dvi_DT = 0;
            dvi_BN = 1;
            Display = Code7seg[dem % 10];
            delay_ms(10);
            dvi_BN = 0;
            chuc_DT = 1;
            Display = Code7seg[dem / 10];
            delay_ms(10);
            chuc_DT = 0;
            chuc_BN = 1;
            Display = Code7seg[dem / 10];
            delay_ms(10);
            chuc_BN = 0;
        }
    }
	StatusTraffic = 0x12;
    // delay30s

    for (dem = 30; dem >= 3; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {
            dvi_BN = 1;
            Display = Code7seg[dem % 10];
            delay_ms(10);
            dvi_BN = 0;
            dvi_DT = 1;
            Display = Code7seg[(dem - 3)%10];
            delay_ms(10);
            dvi_DT = 0;
            chuc_BN = 1;
            Display = Code7seg[dem / 10];
            delay_ms(10);
            chuc_BN = 0;
            chuc_DT = 1;
            Display = Code7seg[(dem - 3)/10];
            delay_ms(10);
            chuc_DT = 0;
        }
    }
	StatusTraffic = 0x6; 
    // delay3s
    for (dem = 2; dem >= 0; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {
            dvi_BN = 1;
            Display = Code7seg[dem % 10];
            delay_ms(10);
            dvi_BN = 0;
            dvi_DT = 1;
            Display = Code7seg[dem % 10];
            delay_ms(10);
            dvi_DT = 0;
            chuc_BN = 1;
            Display = Code7seg[dem / 10];
            delay_ms(10);
            chuc_BN = 0;
            chuc_DT = 1;
            Display = Code7seg[dem / 10];
            delay_ms(10);
            chuc_DT = 0;
        }
    }
}
void Interrupt_Timer0(void)
{
		/* f = 12M hz*/
		/*f vào timer 0 = 12M/12 = 1M*/
		/*T = 1/6M s*/
		/* 1000 --> 1ms*/
		/*64536--> 65535 -> 0 = 1ms*/
    /* 16 bits timer */
    TMOD = 0x01;
    /* setup 1ms for overforlow */
		
    TH0 = 0xFC;
    TL0 = 0x18;
    /* Enable interrupt */
    ET0 = 1;
    EA = 1;
    /* Enable timer0 */
    TR0 = 1;
}

void ISR_TIMER(void) interrupt 1
{
    if (ET0 == 1)
    {
        if (SW_chedo == 0)
        {
            delay_ms(100);
            if (SW_chedo == 1)
            {
                chedo++;
            }
            if (chedo > 1)
                chedo = 0;
        }
        /*setup 1ms for overforlow*/
        TH0 = 0xFC;
        TL0 = 0x18;
    }
}

void main(void)
{
    Interrupt_Timer0();
    while (1)
    {
        if (chedo == 1)
        {
            if (SW == 0)
            {
                delay_ms(100);
                if (SW == 1)
                {
                    chuyenhuong++;
                }
                if (chuyenhuong > 1)
                    chuyenhuong = 0;
            }
            if (chuyenhuong)
            {
                if (P1_1 == 1)
                {
					StatusTraffic = 0x6;
                    delay_ms(2000);
                }
				StatusTraffic = 0x21;
            }
            else
            {
                if (P1_0 == 1)
                {
					StatusTraffic = 0x9;
                    delay_ms(2000);
                }
				StatusTraffic = 0x12;
            }
        }
        if (chedo == 0)
            Traffic_Auto();
    }
}
