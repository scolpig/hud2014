#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Timer.h"

#define FND_0 0b11110101
#define FND_1 0b10000100
#define FND_2 0b10111001
#define FND_3 0b10101101
#define FND_4 0b11001100
#define FND_5 0b01101101
#define FND_6 0b01011101
#define FND_7 0b10100100
#define FND_8 0b11111101
#define FND_9 0b11101100
volatile char FND_flag;
volatile char FND_Data[4];
volatile char FND_Font[10];
volatile char csec,sec;
SIGNAL(SIG_OUTPUT_COMPARE1A)   //인터럽트 서비스 루틴
{
	FND_flag++;
	if(FND_flag >= 4)FND_flag = 0;
	PORTD = ~(1<<FND_flag);
	PORTB = FND_Data[FND_flag];

}
SIGNAL(SIG_OUTPUT_COMPARE3A)	//인터럽트 서비스 루틴
{
	csec++;
	if(csec >= 100){
		csec = 0;
		sec++;
		if(sec >= 60)sec=0;
	}
	FND_Data[0] = FND_Font[csec%10];
	FND_Data[1] = FND_Font[csec%100/10];
	FND_Data[2] = FND_Font[sec%10] | 0b00000100;
	FND_Data[3] = FND_Font[sec%100/10];
}
void main(void){
	char i;
	int a;
	

	Timer1A_Init(124);
	Timer3A_Init(624);

	FND_Font[0] = FND_0;
	FND_Font[1] = FND_1;
	FND_Font[2] = FND_2;
	FND_Font[3] = FND_3;
	FND_Font[4] = FND_4;
	FND_Font[5] = FND_5;
	FND_Font[6] = FND_6;
	FND_Font[7] = FND_7;
	FND_Font[8] = FND_8;
	FND_Font[9] = FND_9;
	
	DDRB = 0xFF;
	DDRD = 0x0F;
	PORTB = 0x00;
	PORTD = 0x0F;

	
	sei();
	while(1);
	
}
