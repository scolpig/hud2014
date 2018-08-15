/*! \file sht11.h  
*****************************************************************************
//
// File Name	: 'sht11.h'
// Title		: SHT11 driver for Zigbex Platform
// Author		: ������
// Created		: 5/2/2011
// Version		: 1.0
// Target MCU	: Atmel AVR ATMega128
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
// *************************************************/ 
/*   SHT11_Init()  �ʱ�ȭ
     Get_Temp()    ���� �µ��� 
     Get_Humi()    ������ (%)            */


void SHT11_Start(void);

void SHT11_Write_command(char data);

char SHT11_Read_data(char ack);

void Connection_Reset(void);

int Get_Temp(void);

int Get_Humi(void);

void SHT11_Init(void);


