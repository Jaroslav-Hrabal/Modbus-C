
#include <AT89C51CC03.h>
#include <stdio.h>
#include "typy.h"


void LcdInit(void);
void putchar(char c);

void LBarInit(void);
void LedBar(word val);

void AdcInit(byte adconf);
word AdcConv(byte channel);


#define T_30MS 50000
#define N_TICKS 6 
#define CHANNEL0 0
#define CHANNEL1 1


byte cnt_ticks;
word valAD;

void TimerInit()
{
   TMOD=0x21;
   TR0=1;
   EA=1;
   ET0=1;
   cnt_ticks=0;

}


void main(void)
{
   LcdInit();
   AdcInit(1<<CHANNEL0);
   LBarInit();
   TimerInit();

   while(1)
   {







   }
}

void timer0() __interrupt 1
{
	 TH0=(byte)(-T_30MS>>8);
	 TL0=(byte)(-T_30MS);
	 cnt_ticks++;
	 if(cnt_ticks == N_TICKS)
	 {
		cnt_ticks=0;
		valAD=AdcConv(CHANNEL0);
		LedBar(valAD);




	 }

}


