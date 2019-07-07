
#include <AT89C51CC03.h>

#include "typy.h"


#define LB_DS P1_3
#define LB_SH P1_4
#define LB_MR P1_5
#define LB_ST P1_6
#define LB_OE P1_7

void LBarInit(void)
{
	byte i;
	LB_ST=0;
	LB_SH=0;
	LB_MR=0;
	LB_DS=1;
	LB_MR=1;
	for(i=0;i<16;i++)
	  {
		LB_SH=1;
		LB_SH=0;
	  }

	LB_ST=1;
	LB_ST=0;
	LB_OE=0;
}


void LedBar(word val)
{
	byte i;

	LB_MR=0;
	LB_MR=1;

	for(i=0;i<10;i++)
	  {
		LB_DS=!(val & 0x200);
		val <<= 1;
		LB_SH=1;
		LB_SH=0;
	  }
	LB_ST=1;
	LB_ST=0;
}


