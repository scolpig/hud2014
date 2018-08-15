/*! Timer.h  
*****************************************************************************
//
// File Name	: 'Timer.h'
// Title		: Timer driver
// Author		: SEOK JU HEON
// Created		: 5/2/2012
// Version		: 1.1
// Target MCU	: Atmel AVR ATMega128
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
// *************************************************/ 

/*SIGNAL(SIG_OUTPUT_COMPARE1A)   //���ͷ�Ʈ ���� ��ƾ
{

}
SIGNAL(SIG_OUTPUT_COMPARE3A)	//���ͷ�Ʈ ���� ��ƾ
{

}*/
void Timer1A_Init(int ms);	//Timer1A CTC_mod   // CTC mode(4), don't output OC1A

void Timer3A_Init(int ms);	//Timer3A CTC_mod
	
