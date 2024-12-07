#include <REGX52.H>  

unsigned char irKey = 0;
unsigned long bitPattern = 0;
unsigned long newKey = 0;
unsigned long pre_newKey = 0;
unsigned char timerValue;
unsigned char msCount = 0;
char pulseCount = 0;

void Timer0_Init(void);
unsigned char Decode_IRKey(unsigned char key);
void DisplayNumber(unsigned char num);  // Hàm hi?n th? s? lên LED 7 do?n

// B?ng mã LED 7 do?n (Cathode chung) cho các s? t? 0-9
unsigned char led7SegCode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void main(){
    P0 = 0x00;          // Kh?i t?o P2 làm c?ng xu?t d? li?u cho LED 7 do?n
    Timer0_Init();      // Kh?i t?o Timer0
    EA = 1;             // Kích ho?t ng?t toàn c?c (Global Interrupt Enable)

    while (1)
    {
        if ((newKey != 0) && (pre_newKey != newKey))  // Ch? phím m?i du?c nh?n
        {
            irKey = Decode_IRKey(newKey);             // Gi?i mã phím m?i
            if (irKey < 10)  // N?u là phím s? (0-9)
                DisplayNumber(irKey);  // Hi?n th? s? lên LED 7 do?n
        }
        pre_newKey = newKey;                          // C?p nh?t giá tr? phím tru?c
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
    case 0xFF10EF:  // Phím '4'
        returnValue = 4;
        break;
    case 0xFF38C7:  // Phím '5'
        returnValue = 5;
        break;
    case 0xFF5AA5:  // Phím '6'
        returnValue = 6;
        break;
    case 0xFF42BD:  // Phím '7'
        returnValue = 7;
        break;
    case 0xFF4AB5:  // Phím '8'
        returnValue = 8;
        break;
    case 0xFF52AD:  // Phím '9'
        returnValue = 9;
        break;
    case 0xFF6897:  // Phím '0'
        returnValue = 0;
        break;
    default:
        returnValue = 10;  // Mã không h?p l? (không ph?i phím s?)
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

// Hàm kh?i t?o Timer0
void Timer0_Init(){
    TMOD |= 0x01;     // C?u hình Timer0 ? ch? d? 16-bit (MODE 1)
    TH0 = 0xFC; 
    TL0 = 0x18;       // Giá tr? kh?i t?o cho 1ms t?i t?n s? 12 MHz
    TR0 = 1;          // B?t d?u Timer0
    ET0 = 1;          // Cho phép ng?t Timer0
    IT0 = 1;          // C?u hình ng?t ngoài INT0 kích ho?t ? c?nh xu?ng
    EX0 = 1;          // Cho phép ng?t ngoài INT0
}

// Hàm hi?n th? s? lên LED 7 do?n
void DisplayNumber(unsigned char num){
    if (num < 10) {
        P0 = led7SegCode[num];  // Xu?t mã s? lên LED 7 do?n (cathode chung)
    }
}
