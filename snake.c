#include <regx52.h>
#include <stdlib.h>

/* Difficulty */
int TIMER0_COUNTER_MAX = 10;
int dao = 0;

#define SNAKE_MAX_LENGTH 64
#define GAME_OVER 1
#define GAME_PLAYING 0

/* Define 74HC595 Pin */
sbit CLOCK = P3^6;
sbit LATCH = P3^5;
sbit DATA = P3^4;

/* Counter variables */
unsigned char timer2_counter = 0;
unsigned char timer0_counter = 0;

/* 8x8 Matrix Buffer */
unsigned char xdata displayBuffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* 4x4 Keypad assigned value */
char code keypad[17] = {
    10, 'u', 10, 10,
    'l', 's', 'r', 10,
    10, 'd', 10, 10,
    10, 10, 10, 10,
    10
};

/* A point on display */
typedef struct {
    unsigned char x;
    unsigned char y;
} Point;

/* Game objects */
Point xdata snake[SNAKE_MAX_LENGTH];
Point apple;
unsigned char snakeLength = 0;
unsigned char snakeDirection = 0;
bit gameState = GAME_OVER;
void Snake_Display();
void Snake_Step();
unsigned char isSnakeHitItself();
void Apple_Create();
void Apple_Display();
void Display_Clear();
void Display_Point(Point);
void Display_Point2(unsigned x, unsigned y);
void Keypad_Read();
char Keypad_Check();
char Keypad_Position(unsigned short value);
void delay(int time);
/*//////////////////////////////////////2////////////////////////////*/

void Matrix_StartDisplay();
void Game_Init();
void Game_Start();
void Game_Stop();
void main () {
/* Variable to handle the keypad */
	


	
	unsigned char counter2 = 0;
	char key_clicked = 0;
	char pre_key_clicked = 0;
	/* Setup */
	P1 = 0xff; //Set to 1 to use as input
	P2 = 0;
	P0 = 0;
	EA = 1;
	LATCH = 0;
	DATA = 0;
	CLOCK = 0;
	/* Start display */
	Matrix_StartDisplay();
	/* Initialize the game */
	Game_Init();
/* While loop. Check clicked key*/
	while(1){
		
//		if(P3_3 == 0){
//		delay(1000);
//			if(P3_3 == 1)
//			{
//				dao++;
//			}
//		}
//		if(dao == 1)
//		{
//			TIMER0_COUNTER_MAX = 1;
//		}
//		if(dao == 0)
//		{
//			TIMER0_COUNTER_MAX = 10;
//		}
//		if(dao > 1)
//		{
//			dao = 0;
//		}
//		
		key_clicked = keypad[Keypad_Check()];
		if((pre_key_clicked != key_clicked) && (key_clicked !=10)){

			if ((gameState == GAME_OVER) && (key_clicked ='s'))
{

				Game_Init();
				Game_Start();
			}
			else if (gameState == GAME_PLAYING){
				snakeDirection = key_clicked;
			
			}
		}
		pre_key_clicked = key_clicked;
	}
}

void Snake_Display(){
	unsigned char counter2 = 0;
	
/*//////////////////////////////////////////3//////////////////////////*/

	Display_Clear();
	Apple_Display();
	for(counter2 = 0; counter2 < snakeLength; counter2++){
		Display_Point(snake[counter2]);
	}
}

void Snake_Step(){
unsigned char counter2 = 0;
/* Remember the position of the tail */
Point snake_tail;
snake_tail.x = snake[snakeLength - 1].x;
snake_tail.y = snake[snakeLength - 1].y;

switch (snakeDirection)
{
/* Turn right */
case 'r':
	for(counter2 = snakeLength - 1; counter2 > 0; counter2--){

		/* counter2 run from last point to 1 */
		snake[counter2].x = snake[counter2 - 1].x;
		snake[counter2].y = snake[counter2- 1].y;
}
snake [0].x = snake[0].x + 1; //Try to go right
if(snake[0].x == 8){
/* If hit the edge */
	snake [0].x = 0;
}
break;
/* Turn left */
case 'l':
	for(counter2 = snakeLength - 1; counter2 > 0; counter2--){
/* counter2 run from last point to 1 */
	snake [counter2].x = snake[counter2 - 1].x;
	snake[counter2].y = snake[counter2 - 1].y;
}
snake [0].x --; //Try to go left
if (snake[0].x == 255){
/* If hit the edge */
	snake [0].x = 7;
	
/*///////////////////////////////////////////4/////////////////////*/
}
break;

case 'u':
for(counter2 = snakeLength - 1; counter2 > 0; counter2--){

/* counter2 run from last point to 1 */
snake [counter2].x = snake[counter2 - 1].x;
snake[counter2].y = snake[counter2 - 1].y;
}
snake [0].y++; //Try to go up
if(snake[0].y == 8){
/* If hit the edge */
snake [0].y = 0;
}
break;

case 'd':
for(counter2 = snakeLength - 1; counter2 > 0; counter2--){

/* counter2 run from last point to 1 */
snake [counter2].x = snake[counter2 - 1].x;
snake [counter2].y = snake[counter2 - 1].y;
}
snake[0].y --; //Try to go down
if(snake[0].y == 255){
/* If hit the edge */
snake[0].y = 7;
}
break;

default:
break;
}
/* If snake eat an apple */
if((snake[0].x == apple.x) && (snake[0].y == apple.y)){
	snake[snakeLength].x = snake_tail.x;
	snake[snakeLength].y = snake_tail.y;
	snakeLength++; //Increase snake's length
	Apple_Create();
}
/*///////////////////////////////////////////////////////////////////*/
/* If snake hit itself */
if(isSnakeHitItself()){
Game_Stop();
}
/* Update the snake on display */
Snake_Display();
}

unsigned char isSnakeHitItself(){
	unsigned char returnValue = 0;
	unsigned char counter2 = 0;
	for(counter2 = 1; counter2 < snakeLength; counter2++){
		returnValue = (snake[0].x == snake[counter2].x) && (snake[0].y == snake[counter2].y);
		if (returnValue) break;
	}
	return returnValue;
}
void Apple_Display(){
Display_Point (apple);
}
void Apple_Create(){
unsigned char check = 1;
unsigned char counter2 = 0;
do
{
apple.x = ((unsigned int) rand()) % 8;
apple.y = ((unsigned int) rand()) % 8;
for (counter2 = 0; counter2 < snakeLength; counter2++)
{

check = (apple.x == snake[counter2].x) && (apple.y == snake [counter2].y);
if(check) break;
}
}

while (check);
}

/* Clear the display */
void Display_Clear(){

/*/////////////////////////////////////////////////////////////////////////////*/

unsigned char counter2 = 0;
for (counter2 = 0; counter2 < 8; counter2++)
{
displayBuffer [counter2] = 0;
}
}
/* Display a Point */
void Display_Point(Point point){
unsigned char x = point.x;
unsigned char y = point.y;
displayBuffer[7 - y] =displayBuffer[7- y] | (1 << (7-x));
}
char Keypad_Check(){
unsigned short clickPos = 0;
P1 = 0xef;
clickPos = (clickPos << 4) | (P1 & 0x0f);
P1 = 0xdf;
clickPos = (clickPos << 4) | (P1 & 0x0f);
P1 = 0xbf;
clickPos = (clickPos << 4) | (P1 & 0x0f);
P1 = 0x7f;
clickPos = (clickPos << 4) | (P1 & 0x0f);
return Keypad_Position(clickPos);
}
char Keypad_Position(unsigned short value){
char returnvalue = 16;
switch (value & 0xff)
{
			case 0xf7:
				returnvalue = 0;
			break;
			case 0xfb:
				returnvalue = 1;
			break;
			case 0xfd:
				returnvalue = 2;
			break;
			case 0xfe:
				returnvalue = 3;
			break;
case 0x7f:
returnvalue = 4;
break;
case 0xbf:
returnvalue = 5;
break;
case 0xdf:
returnvalue = 6;
break;
case 0xef:
returnvalue = 7;
break;
default:
value = value >> 8;
switch (value & 0xff)
{
case 0xf7:
returnvalue = 8;
break;
case 0xfb:
returnvalue = 9;
break;
case 0xfd:
returnvalue = 10;
break;
case 0xfe:
returnvalue = 11;
break;
case 0x7f:
returnvalue = 12;
break;
case 0xbf:
returnvalue = 13;
break;
case 0xdf:
returnvalue = 14;
break;
case 0xef:
returnvalue = 15;
break;
/*/////////////////////////////////////////////////////////////*/
}
break;
}
return returnvalue;
}

void delay(int time)
{
	while(--time);
}

void Timer0_Isr(void) interrupt 1
{

TL0 = 0x00;
TH0 = 0x04;
timer0_counter++;
if(timer0_counter == TIMER0_COUNTER_MAX){
timer0_counter = 0;
Snake_Step();
}
//Initial timer vatue
//Initial timer value
}
void Game_Init(){
snakeLength = 3;
/* Initial apple position */
apple.x = 5;
apple.y = 5;
/* Initial snake position */
snake [2].x = 3;
snake [2].y = 3;
snake [1].x = 4;
snake [1].y = 3;
snake [0].x = 5;
snake [0].y = 3;
/* Initial direction */
snakeDirection = 'r';
gameState = GAME_OVER;
Snake_Display();
}
void Game_Start(void)
{

TMOD &= 0xF0;
TMOD |= 0x01;
TL0 = 0x00;
TH0 = 0x04;
TF0 = 0;
TR0 = 1;

//70ms@11.0592MHz

//Set timer work mode
//Set timer work mode
//Initial timer value
//Initial timer value
//Clear TF0 flag
//Timer0 start run

/*///////////////////////////////////////////////////*/

ET0 = 1;

//Stop the timer0

//2ms@11.0592MHz

//Initial timer work mode register
//Initial timer control register
//Initial timer value
//Initial timer value
//Set reload value

//Timer2 start run

//Enable timer0 interrupt
gameState = GAME_PLAYING;
}
void Game_Stop(){
TR0 = 0;
gameState = GAME_OVER;
}
void Timer2_Isr(void) interrupt 5
{
TF2 = 0;
DATA = timer2_counter == 0;
CLOCK = 1;
P0 = ~displayBuffer[7 - timer2_counter];
LATCH = 1;
timer2_counter++;
if(timer2_counter == 8) timer2_counter = 0;
CLOCK = 0;
LATCH = 0;
}
void Matrix_StartDisplay(void)
{

T2MOD =0;
T2CON = 0;
TL2 = 0xCD;
TH2 = 0xF8;
RCAP2L = 0xCD;
RCAP2H = 0xF8;
TR2 =1;
ET2 = 1;
}

