#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "FND.h"

#define FND_0 0b01011111
#define FND_1 0b00001001
#define FND_2 0b11010101
#define FND_3 0b10011101
#define FND_4 0b10001011
#define FND_5 0b10011110
#define FND_6 0b11011110
#define FND_7 0b00001101
#define FND_8 0b11011111
#define FND_9 0b10011111
#define FND_CONTROL_DDR DDRC
#define FND_CONTROL_PORT PORTC
#define FND_DATABUS_DDR DDRA
#define FND_DATABUS_PORT PORTA

volatile char FND_flag;
volatile char FND_Data[4];
volatile char FND_Font[10] = {FND_0, FND_1, FND_2, FND_3, FND_4, FND_5,
FND_6, FND_7, FND_8, FND_9};
volatile char out_mod;
volatile char red_flag, green_flag, blue_flag;

SIGNAL( TIMER3_COMPA_vect)   //인터럽트 서비스 루틴 2ms 간격
{
	FND_flag++;
	if(FND_flag >= 4)FND_flag = 0;
	FND_CONTROL_PORT = ~(0x80>>FND_flag);
	if(FND_flag == 3 && out_mod == 1)FND_DATABUS_PORT = 0b01011011;
	else if(FND_flag == 2 && out_mod == 1){
		FND_DATABUS_PORT = FND_Data[FND_flag] | 0x20;
	}
	else if(FND_flag == 3 && out_mod == 2){
		FND_DATABUS_PORT = FND_Data[FND_flag] | 0x20;
	}
	else FND_DATABUS_PORT = FND_Data[FND_flag];
	if(blue_flag)OCR1A -= 2;
	else OCR1A += 2;
	if(green_flag)OCR1B -= 3;
	else OCR1B += 3;
	if(red_flag)OCR1C -= 5;
	else OCR1C += 5;
	if(OCR1A >= 7000 || OCR1A <= 1)blue_flag ^= 1;
	if(OCR1B >= 7000 || OCR1B <= 1)green_flag ^= 1;
	if(OCR1C >= 7000 || OCR1C <= 1)red_flag ^= 1;
}

void FND_Init(void){	
	FND_DATABUS_DDR = 0xFF;
	FND_CONTROL_DDR |= 0xF0;
	FND_DATABUS_PORT = 0x00;
	FND_CONTROL_PORT = 0xF0;
	Timer3A_Init(1);
}

void FND_UPDATE(unsigned int data){
	FND_Data[0] = FND_Font[data % 10000 / 1000];
	FND_Data[1] = FND_Font[data % 1000 / 100];
	FND_Data[2] = FND_Font[data % 100 / 10];
	FND_Data[3] = FND_Font[data % 10];
}

void FND_TempOut(signed int data){
	if(data < 0)FND_Data[0] = 0b10000000;
	else FND_Data[0] = 0;
	if(data < 0)FND_Data[1] = FND_Font[data*(-1) / 10];
	else FND_Data[1] = FND_Font[data / 10];
	if(data < 0)FND_Data[2] = FND_Font[data*(-1) % 10];
	else FND_Data[2] = FND_Font[data % 10];
	FND_Data[3] = 0b11010000;
}

void FND_HumiOut(unsigned int data){
	FND_Data[0] = 0;
	FND_Data[1] = 0;
	FND_Data[2] = FND_Font[data / 10];
	FND_Data[3] = FND_Font[data % 10];
}

