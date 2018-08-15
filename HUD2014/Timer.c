/*! Timer.c  
*****************************************************************************
//
// File Name	: 'Timer.c'
// Title		: Timer driver
// Author		: KYOUNG YOON JEONG
// Created		: 7/10/2012
// Version		: 1.0
// Target MCU	: Atmel AVR ATMega128
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
// *************************************************/ 

#include <avr/io.h>
#define F_CPU 14745600UL
#include <avr/interrupt.h>

SIGNAL(TIMER1_COMPA_vect)   //인터럽트 서비스 루틴
{

}
/*SIGNAL(TIMER1_COMPC_vect)	//인터럽트 서비스 루틴
{
	
}*/

void Timer1A_Init(int ms){	//매개변수로 넘겨받는 시간 주기로 인터럽트 발생(단위 ms)
	int divider;

	TCCR1A |= 0b10101010;              //don't output OC1A
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS11) ;  //Fast PWM_mod, divide 8 
	TCCR1C |= 0;       

	switch(TCCR1B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default :    divider = 1;       break;
	}

 	OCR1AH = ((F_CPU/divider/1000*ms-1) >> 8);  
	OCR1AL = (F_CPU/divider/1000*ms-1) & 0XFF;
	 
	ICR1 = 19999;									//0.5usec*20000=100Hz
	
	//OCR1B = 0x1A00;
	
	
	TCNT1H = 0x00;                               // clear Timer/Counter1
	TCNT1L = 0x00;

	//TIMSK |= 1<<OCIE1A;                          // enable OC1A interrupt

	TIFR = 0X00;                                 // clear all interrupt flags
	ETIFR = 0X00;
}	
void Timer3A_Init(int ms){	
	int divider;
	
	TCCR3A |= 0;								//don't output OC1A
	TCCR3B |= (1<<WGM32) | (1<<CS12);			//Timer1A CTC_mod
 	TCCR3C |= 0;       
	switch(TCCR3B & 0b00000111){
		case 0b001 : divider = 1;	    break;
		case 0b010 : divider = 8;		break;
		case 0b011 : divider = 64;		break;
		case 0b100 : divider = 256;		break;
		case 0b101 : divider = 1024;	break;
		default :    divider = 1;	    break;
	}

 	OCR3AH = ((F_CPU/divider/1000*ms-1) >> 8);  
	OCR3AL = (F_CPU/divider/1000*ms-1) & 0XFF; 
   
 	TCNT3H = 0X00;
 	TCNT3L = 0X00;
  
	ETIMSK |= 1<<OCIE3A;
	TIFR = 0X00;
	ETIFR = 0X00;
}
	

