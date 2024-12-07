#include <regx52.h>
#include <intrins.h>
#include <stdlib.h>

sbit SRCLK=P3^6;
sbit CLK=P3^5;
sbit SER=P3^4;

#define COMMONPORTS     P0 // chan dieu khien led ma tran
#define GPIO_KEY P1	// chân di?u khi?n các nút nh?n
unsigned char code TAB[8]  = {0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};// du lieu can de dieu khien ma tran
unsigned char map[8]={0,0,0,0,0,0,0,0};
unsigned char buff[8]={0,0,0,0,0,0,0,0};
unsigned char xTail[16],yTail[16];
unsigned char KeyValue,xf,yf,x,y,gameOver,tailLen,buffkey;

void delay(unsigned int time)
{
  unsigned int i,j;
  for(i=0;i<time;i++)
    for(j=0;j<121;j++);
}
//keydown
void KeyDown(void)
{
    GPIO_KEY=0x0f;
    if(GPIO_KEY!=0x0f)
    {
        if(GPIO_KEY!=0x0f)
        {
            
            buffkey=KeyValue;
            GPIO_KEY=0X0F;
            switch(GPIO_KEY)
            {
                case(0X07): KeyValue=0;break;
                case(0X0b): KeyValue=1;break;
                case(0X0d): KeyValue=2;break;
                case(0X0e): KeyValue=3;break;
            }
        
            GPIO_KEY=0XF0;
            switch(GPIO_KEY)
            {
                case(0X70): KeyValue=KeyValue;break;
                case(0Xb0): KeyValue=KeyValue+4;break;
                case(0Xd0): KeyValue=KeyValue+8;break;
                case(0Xe0): KeyValue=KeyValue+12;break;
            }
            switch(KeyValue)
                {
                    case 4: KeyValue=(buffkey!=6)?4:buffkey;break;
                    case 6: KeyValue=(buffkey!=4)?6:buffkey;break;
                    case 1: KeyValue=(buffkey!=9)?1:buffkey;break;
                    case 9: KeyValue=(buffkey!=1)?9:buffkey;break;
                }
        }
    }
}

    //send
    void send(unsigned char dat)
    {
        unsigned char a;
        SRCLK=0;
        CLK=0;
        for(a=0;a<8;a++)
        {
            SER=dat>>7;
            dat<<=1;
    
            SRCLK=1;
            SRCLK=0;    
        }

        CLK=1;
        CLK=0;
    }

//init
    void init()
    {   TMOD=0x01;
        EA=1;
        TR0=1;
        TH0=0xef;
        TL0=0xff;
        ET0=1;
        gameOver = 0;
        x = 2;
        y = 5;
        xf = ((unsigned char) rand()) % 8;
        yf = ((unsigned char) rand()) % 8;
        while (xf == 0 || xf == 7)  xf = ((unsigned char) rand()) % 8;
        while (yf == 0 || yf == 7)  yf = ((unsigned char) rand()) % 8;
    }
//change
    unsigned char change(unsigned char n)
    {
        switch(n){
            case 0: return 0x80;
            case 1: return 0x40;
            case 2: return 0x20;
            case 3: return 0x10;
            case 4: return 0x08;
            case 5: return 0x04;
            case 6: return 0x02;
            case 7: return 0x01;
    }
//      return -1;
}
// premap
    void premap()
{   unsigned int  i;
        for (i=0;i<8;i++)
        { 
            buff[i]=0;
        }
        
        buff[xf] |= change(yf);
        buff[x] |= change(y);
            
        for (i=0;i<tailLen;i++) 
        {
            buff[xTail[i]] |= change(yTail[i]);

        }   
    }
//draw
        void draw()
        {
            unsigned int  i,j;
            for(j=0;j<50;j++)
            for(i=0;i<8;i++)
            {
                send(0x00);
                map[i] = buff[i];
                if (i==0) map[i] = 0xff;
                if (i==7) map[i] = 0xff;
                COMMONPORTS = TAB[i];       
                send(map[i] | 0x81);
                delay(2);
            }
        }
        

//logic
        void logic()
        {
            //update snake movement
        unsigned int  i;
        unsigned char xPrev = xTail[0];
        unsigned char yPrev = yTail[0];
        unsigned char x2Prev, y2Prev;
                xTail[0] = x;
                yTail[0] = y;
        for (i=1;i<tailLen;i++)
            {
                x2Prev = xTail[i];
                y2Prev = yTail[i];
                xTail[i] = xPrev;
                yTail[i] = yPrev;
                xPrev = x2Prev;
                yPrev = y2Prev;
            }
            //check movement direction
                switch(KeyValue)
                {
                    case 4: x--;break;
                    case 6: x++;break;
                    case 1: y--;break;
                    case 9: y++;break;
                }
                //check collision
                if (x <= 0 || x >= 7 || y <= 0 || y >= 7) gameOver = 1;
                //check self-collision
                for (i = 0; i < tailLen; i++) 
                {                   
                    if (xTail[i] == x && yTail[i] == y)
                    gameOver = 1;
                }
                // once hits the fruit
                if (x == xf && y == yf) {
                    xf = ((unsigned char) rand()) % 8;
                    yf = ((unsigned char) rand()) % 8;
                    while (xf == 0 || xf == 7)  xf = ((unsigned char) rand()) % 8;
                    while (yf == 0 || yf == 7)  yf = ((unsigned char) rand()) % 8;
                    tailLen+=1;
    }
            }
            void main()
        {   init(); 
            tailLen=0;
            while (!gameOver)
            {
                premap();
                draw();
                KeyDown();      
                logic();
            }
        }
        void timerInterrupt(void) interrupt 1
        {
            TF0=0;
            TH0=0xef;
            TL0=0xff;
            KeyDown();
        }