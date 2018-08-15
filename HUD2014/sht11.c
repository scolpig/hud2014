#include <avr/io.h>
#define F_CPU 14745600UL
#include <util/delay.h>

#define TEMP    0x03
#define HUMI    0x05
//#define TRUE    1
//#define FALSE   0
#define c1      -4.0
#define c2      0.0405
#define c3      -0.0000028
#define d1      -39.66
#define d2      0.01
#define SHT11Delay           _delay_us(2);
#define SHT11_DDR            DDRD
#define SHT11_PIN            PIND
#define SHT11_PORT           PORTD
#define SHT11_DATA           0x02
#define SHT11_SCK          	 0x01
                                        //adr    command r/w
//#define SHT11_STATUS_REG_W   0x06    //000     0011    0
//#define SHT11_STATUS_REG_R   0x07    //000     0011    1
#define SHT11_MEASURE_TEMP   0x03    //000     0001    1
#define SHT11_MEASURE_HUMI   0x05    //000     0010    1
#define SHT11_RESET          0x1e    //000     1111    0

#define noACK 0
#define ACK   1

//unsigned int PreTimerCnt = 0;
//unsigned short HumiError = FALSE;
//float Temp, Hum;
//float Temperature, Humidity;


/******************************************************************************

Name:           unsigned short SHT11TransStart(void)

Description:   Generates a transmission start

Input:           none

Output:          none

Misc:
              _____         ________
DATA:              |_______|
                  ___     ___
SCK :         ___|   |___|   |______
******************************************************************************/
void SHT11TransStart(void)
{
  SHT11_DDR  |= SHT11_DATA;       //DATA-line in output

  SHT11_PORT |= SHT11_DATA;       //DATA=1
  SHT11_PORT &= ~SHT11_SCK;       //SCK=0
  SHT11Delay
  SHT11_PORT |= SHT11_SCK;        //SCK=1
  SHT11Delay
  SHT11_PORT &= ~SHT11_DATA;      //DATA=0
  SHT11Delay
  SHT11_PORT &= ~SHT11_SCK;       //SCK=0
  SHT11Delay
  SHT11_PORT |= SHT11_SCK;           //SCK=1
  SHT11Delay
  SHT11_PORT |= SHT11_DATA;          //DATA=1
  SHT11Delay
  SHT11_PORT &= ~SHT11_SCK;          //SCK=0
  SHT11Delay
}
void SHT11ConnectionReset(void)
{
    unsigned char i;
	
	SHT11_PORT |= SHT11_DATA;       //DATA=1
	SHT11_PORT &= ~SHT11_SCK;       //SCK=0
	SHT11Delay
    for(i=0;i<9;i++)
    {
		SHT11_PORT |= SHT11_SCK;           //SCK=1
		SHT11Delay  
		SHT11_PORT &= ~SHT11_SCK;          //SCK=0
		SHT11Delay              
    }        
    SHT11TransStart();
}


void SHT11WriteByte(unsigned short value)
{
  unsigned short i;

  SHT11_DDR  |= SHT11_DATA;       //DATA-line in output

  for (i=0x80;i>0;i/=2)                 //shift bit for masking
  {
    SHT11Delay
    if (i & value) SHT11_PORT |= SHT11_DATA;
    else SHT11_PORT &= ~SHT11_DATA;
    SHT11Delay
    SHT11_PORT |= SHT11_SCK;
    SHT11Delay
    SHT11_PORT &= ~SHT11_SCK;
  }

  SHT11_PORT |= SHT11_DATA;       //release DATA-line
  SHT11_DDR  &= ~SHT11_DATA;      //DATA-line in input
  SHT11Delay
  SHT11_PORT |= SHT11_SCK;       //clk #9 for ack
  SHT11Delay
  //if (SHT11_PIN & SHT11_DATA) HumiError = TRUE;
  SHT11_PORT &= ~SHT11_SCK;
  SHT11Delay
}
unsigned short SHT11ReadByte(unsigned short ack)
{
  unsigned short i,val=0;

  SHT11_PORT |= SHT11_DATA;       //release DATA-line
  SHT11_DDR  &= ~SHT11_DATA;      //DATA-line in input

  for (i = 0x80; i > 0; i /= 2)         //shift bit for masking
  {
    SHT11Delay
    SHT11_PORT |= SHT11_SCK;      //clk for SENSI-BUS
    SHT11Delay
    if (SHT11_PIN & SHT11_DATA) val = (val | i);  //read bit
    SHT11_PORT &= ~SHT11_SCK;
  }

  if (ack == 1) {
    SHT11_DDR  |= SHT11_DATA;     //DATA-line in output
    SHT11_PORT &= ~SHT11_DATA;    //"ack==1" pull down DATA-Line
    SHT11Delay
    SHT11_PORT |= SHT11_SCK;
    SHT11Delay
    SHT11_PORT &= ~SHT11_SCK;
  }
  else {
    SHT11Delay
    SHT11_PORT |= SHT11_SCK;
    SHT11Delay
    SHT11_PORT &= ~SHT11_SCK;
  }
  return val;
}
void SHT11SoftReset(void)
{
  int i;
  SHT11ConnectionReset();           //Reset communication
  SHT11WriteByte(SHT11_RESET);   //send Reset-command to sensor
  _delay_ms(210);
  for (i=0;i<100;i++) SHT11Delay // 11ms delay
}

void SHT11Init(void)
{
  SHT11_DDR |= SHT11_DATA + SHT11_SCK;
  SHT11_PORT |= SHT11_DATA + SHT11_SCK;
  SHT11SoftReset();
}
signed int Get_Temp(void){

	unsigned short msb=0, lsb=0;
	float temp=0.0;

	SHT11TransStart();
	_delay_ms(2);	
	SHT11WriteByte(TEMP);
	SHT11_PORT |= SHT11_DATA;           //release DATA-line
	SHT11_DDR  &= ~SHT11_DATA;          //DATA-line in input
	while((SHT11_PIN & SHT11_DATA) != 0);     //wait until sensor has finished the measure
	msb = SHT11ReadByte(ACK);              //read the first byte (MSB)
	lsb = SHT11ReadByte(ACK);              //read the second byte (LSB)
	SHT11ReadByte(noACK);             //dummy read checksum
	temp = (msb << 8) + lsb;
	temp = d1+d2*temp;
	return temp;
}
int Get_Humi(void){

	unsigned short msb=0, lsb=0;
	unsigned temp=0;

	SHT11TransStart();
	_delay_ms(2);	
	SHT11WriteByte(HUMI);
	SHT11_PORT |= SHT11_DATA;           //release DATA-line
	SHT11_DDR  &= ~SHT11_DATA;          //DATA-line in input
	while((SHT11_PIN & SHT11_DATA) != 0);     //wait until sensor has finished the measure
	msb = SHT11ReadByte(ACK);              //read the first byte (MSB)
	lsb = SHT11ReadByte(ACK);              //read the second byte (LSB)
	SHT11ReadByte(noACK);             //dummy read checksum
	temp = (msb << 8) + lsb;
	temp = c1+c2*temp+c3*temp*temp;
	return temp;
}
