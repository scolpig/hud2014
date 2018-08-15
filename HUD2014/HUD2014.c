/*
 * HUD2014.c
 *
 * Created: 2014-12-11 오전 8:50:29
 *  Author: scolpig
 */ 


#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "FND.h"
#include "adc.h"
#include "sht11.h"

#define volt 1
#define temperature 2
#define humi 3

volatile char out_mod=1;
int main(void)
{
	
	DDRB = 0xFF;
	PORTB = 0b00011111;
	
	FND_Init();
	ADC_Init();
	SHT11Init();
	Timer1A_Init(1);
	OCR1A = 1;
	OCR1B = 1;
	OCR1C = 1;
	sei();
    while(1)
    {
       FND_UPDATE(1080+(ADC_Value()-547)*23);
	   out_mod = volt;
	   _delay_ms(2000);
	   //OCR1B *= 5;
	   FND_TempOut(Get_Temp());
	   out_mod = temperature;
	   _delay_ms(2000);
	   //OCR1B *= 5;
	   out_mod = humi;
	   FND_HumiOut(Get_Humi());
	   _delay_ms(2000);
	   //OCR1B *= 5;
	   //if(OCR1B >= 7000)OCR1B = 1;
	   
	}
}

