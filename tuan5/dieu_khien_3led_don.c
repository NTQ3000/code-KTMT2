#include <REG52.H>  // S? d?ng REG52.H cho kit 80C52RC

unsigned char irKey = 0;
unsigned long bitPattern = 0;
unsigned long newKey = 0;
unsigned long pre_newKey = 0;
unsigned char timerValue;
unsigned char msCount = 0;
char pulseCount = 0;
sbit chedo1= P3^2;
sbit chedo2= P3^3;
sbit chedo3= P3^4;
void Timer0_Init(void);
unsigned char Decode_IRKey(unsigned char key);
void ToggleLED(unsigned char ledNum);  // H�m d?i tr?ng th�i LED
unsigned char tat1,tat2,tat3;
void main(){
    P2 = 0xFF;          // Kh?i t?o P2: T?t c? c�c LED ban d?u b?t (1: b?t LED, 0: t?t LED)
    Timer0_Init();      // Kh?i t?o Timer0
    EA = 1;             // K�ch ho?t ng?t to�n c?c (Global Interrupt Enable)
		chedo1=0;		tat1=0;
		chedo2=0;		tat2=0;
		chedo3=0;		tat3=0;
    while (1)
    {
        if ((newKey != 0) && (pre_newKey != newKey))  // Ch? ph�m m?i du?c nh?n
        {
            irKey = Decode_IRKey(newKey);             // Gi?i m� ph�m m?i
            if (irKey >= 1 && irKey <= 3)             // N?u ph�m l� 1, 2, ho?c 3
                ToggleLED(irKey);                     // �?i tr?ng th�i LED tuong ?ng
        }
        pre_newKey = newKey;  				// C?p nh?t gi� tr? ph�m tru?c
				if ((chedo1==1))
				{
					ToggleLED(1);			
				}    
				if ((chedo2==1))
				{
					ToggleLED(2);			
				}    
				if ((chedo3==1))
				{
					ToggleLED(3);			
				}    
		}
}

// H�m gi?i m� m� ph�m t? di?u khi?n t? xa
unsigned char Decode_IRKey(unsigned char key){
    unsigned char returnValue = 0;
    switch (key)
    {
    case 0xFF30CF:  // Ph�m '1'
        returnValue = 1;
        break;
    case 0xFF18E7:  // Ph�m '2'
        returnValue = 2;
        break;
    case 0xFF7A85:  // Ph�m '3'
        returnValue = 3;
        break;
    default:
        returnValue = 0;  // M� kh�ng h?p l?
        break;
    }
    return returnValue;
}

// Ng?t Timer0 d? d?m mili gi�y
void timer0_isr() interrupt 1
{
    if (msCount < 50)
        msCount++;
    
    // C?u h�nh l?i gi� tr? Timer0 d? ti?p t?c d?m 1ms
    TH0 = 0xFC; 
    TL0 = 0x18;  // Gi� tr? du?c t�nh cho t?n s? 12MHz
}

// Ng?t ngo�i d? nh?n t�n hi?u IR
void externalIntr0_ISR() interrupt 0
{
    timerValue = msCount;
    msCount = 0;
    
    TH0 = 0xFC; 
    TL0 = 0x18;  // C?u h�nh l?i Timer0

    pulseCount++;
    
    if ((timerValue >= 50))  // N?u d? r?ng xung l?n hon 50ms, d�nh d?u b?t d?u khung (Start Of Frame)
    {
        pulseCount = -2;     // B? qua 2 xung d?u
        bitPattern = 0;
    }
    else if ((pulseCount >= 0) && (pulseCount < 32)) // T�ch luy gi� tr? bit t? 0 d?n 31
    {
        if (timerValue >= 2) // Xung c� d? r?ng > 2ms l� bit 1
            bitPattern |= (unsigned long)1 << (31 - pulseCount);
    }
    else if (pulseCount >= 32) // K?t th�c khung khi nh?n d? 32 bit
    {
        newKey = bitPattern;  // Luu m� ph�m m?i
        pulseCount = 0;
    }
}

// Kh?i t?o Timer0
void Timer0_Init(){
    TMOD |= 0x01;     // C?u h�nh Timer0 ? ch? d? 16-bit (MODE 1)
    TH0 = 0xFC; 
    TL0 = 0x18;       // Gi� tr? kh?i t?o cho 1ms t?i t?n s? 12 MHz
    TR0 = 1;          // B?t d?u Timer0
    ET0 = 1;          // Cho ph�p ng?t Timer0
    IT0 = 1;          // C?u h�nh ng?t ngo�i INT0 k�ch ho?t ? c?nh xu?ng
    EX0 = 1;          // Cho ph�p ng?t ngo�i INT0
}

// H�m d?i tr?ng th�i LED tuong ?ng
void ToggleLED(unsigned char ledNum){
    // ledNum = 1, 2, 3 tuong ?ng v?i LED t?i P2.0, P2.1, P2.2
    switch (ledNum) {
        case 1: 
            P2 ^= 0x01;  // �?i tr?ng th�i LED1 (P2.0)
            break;
        case 2: 
            P2 ^= 0x02;  // �?i tr?ng th�i LED2 (P2.1)
            break;
        case 3: 
            P2 ^= 0x04;  // �?i tr?ng th�i LED3 (P2.
		}
	}