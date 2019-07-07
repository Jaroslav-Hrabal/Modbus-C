
#include <AT89C51CC03.h>
#include "typy.h"

void AdcInit(byte adconf)
{
   ADCF=adconf;
   ADCLK=0x08;
   ADCON = MSK_ADCON_ADEN;
}

word AdcConv(byte channel)
{
	  ADCON &= ~MSK_ADCON_SCH;
	  ADCON |= (channel & MSK_ADCON_SCH);
	  ADCON |= MSK_ADCON_ADSST;                   /* Start prevodu */
	  while((ADCON & MSK_ADCON_ADEOC)==0);
	  ADCON &= ~MSK_ADCON_ADEOC;
	  return (ADDH <<2)+(ADDL & 3);
}
