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
void ToggleLED(unsigned char ledNum);  // Hàm d?i tr?ng thái LED
unsigned char tat1,tat2,tat3;
void main(){
    P2 = 0xFF;          // Kh?i t?o P2: T?t c? các LED ban d?u b?t (1: b?t LED, 0: t?t LED)
    Timer0_Init();      // Kh?i t?o Timer0
    EA = 1;             // Kích ho?t ng?t toàn c?c (Global Interrupt Enable)
		chedo1=0;		tat1=0;
		chedo2=0;		tat2=0;
		chedo3=0;		tat3=0;
    while (1)
    {
        if ((newKey != 0) && (pre_newKey != newKey))  // Ch? phím m?i du?c nh?n
        {
            irKey = Decode_IRKey(newKey);             // Gi?i mã phím m?i
            if (irKey >= 1 && irKey <= 3)             // N?u phím là 1, 2, ho?c 3
                ToggleLED(irKey);                     // Ð?i tr?ng thái LED tuong ?ng
        }
        pre_newKey = newKey;  				// C?p nh?t giá tr? phím tru?c
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

// Hàm gi?i mã mã phím t? di?u khi?n t? xa
unsigned char Decode_IRKey(unsigned char key){
    unsigned char returnValue = 0;
    switch (key)
    {
    case 0xFF30CF:  // Phím '1'
        returnValue = 1;
        break;
    case 0xFF18E7:  // Phím '2'
        returnValue = 2;
        break;
    case 0xFF7A85:  // Phím '3'
        returnValue = 3;
        break;
    default:
        returnValue = 0;  // Mã không h?p l?
        break;
    }
    return returnValue;
}

// Ng?t Timer0 d? d?m mili giây
void timer0_isr() interrupt 1
{
    if (msCount < 50)
        msCount++;
    
    // C?u hình l?i giá tr? Timer0 d? ti?p t?c d?m 1ms
    TH0 = 0xFC; 
    TL0 = 0x18;  // Giá tr? du?c tính cho t?n s? 12MHz
}

// Ng?t ngoài d? nh?n tín hi?u IR
void externalIntr0_ISR() interrupt 0
{
    timerValue = msCount;
    msCount = 0;
    
    TH0 = 0xFC; 
    TL0 = 0x18;  // C?u hình l?i Timer0

    pulseCount++;
    
    if ((timerValue >= 50))  // N?u d? r?ng xung l?n hon 50ms, dánh d?u b?t d?u khung (Start Of Frame)
    {
        pulseCount = -2;     // B? qua 2 xung d?u
        bitPattern = 0;
    }
    else if ((pulseCount >= 0) && (pulseCount < 32)) // Tích luy giá tr? bit t? 0 d?n 31
    {
        if (timerValue >= 2) // Xung có d? r?ng > 2ms là bit 1
            bitPattern |= (unsigned long)1 << (31 - pulseCount);
    }
    else if (pulseCount >= 32) // K?t thúc khung khi nh?n d? 32 bit
    {
        newKey = bitPattern;  // Luu mã phím m?i
        pulseCount = 0;
    }
}

// Kh?i t?o Timer0
void Timer0_Init(){
    TMOD |= 0x01;     // C?u hình Timer0 ? ch? d? 16-bit (MODE 1)
    TH0 = 0xFC; 
    TL0 = 0x18;       // Giá tr? kh?i t?o cho 1ms t?i t?n s? 12 MHz
    TR0 = 1;          // B?t d?u Timer0
    ET0 = 1;          // Cho phép ng?t Timer0
    IT0 = 1;          // C?u hình ng?t ngoài INT0 kích ho?t ? c?nh xu?ng
    EX0 = 1;          // Cho phép ng?t ngoài INT0
}

// Hàm d?i tr?ng thái LED tuong ?ng
void ToggleLED(unsigned char ledNum){
    // ledNum = 1, 2, 3 tuong ?ng v?i LED t?i P2.0, P2.1, P2.2
    switch (ledNum) {
        case 1: 
            P2 ^= 0x01;  // Ð?i tr?ng thái LED1 (P2.0)
            break;
        case 2: 
            P2 ^= 0x02;  // Ð?i tr?ng thái LED2 (P2.1)
            break;
        case 3: 
            P2 ^= 0x04;  // Ð?i tr?ng thái LED3 (P2.
		}
	}