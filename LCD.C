//**********************************************************************
//**********************************************************************
//***                                                                ***
//***                       L C D  V1.0                       ***
//***                                                                ***
//**********************************************************************
//**********************************************************************



#include <8051.h>

#include "typy.h"


#define LCD_RS  0x10
#define LCD_RW  0x20
#define LCD_EN  0x40
#define LCD_LINE 16
#define _RS P2_4
#define _RW P2_5
#define _EN P2_6

#define DISPLAY_ON                   0x0C
#define DISPLAY_OFF                  0x08

#define CLEAR_DISPLAY                0x01
#define ENTRY_MODE                   0x06
#define CURSOR_HOME                  0x02
#define FUNCTION_SET                 0x28
#define FUNCTION_INIT                0x03

#define DDRAM_ADDRESS                0x80

#define BUSY_FLAG                    0x80

#define LINE1_ORIGIN                 0x00
#define LINE2_ORIGIN                 0x40



#define INIT_NIBBLE(b)    \
  P2=( LCD_EN | (b) );    \
  Pause(1);               \
  _EN=0;

#define DATA_NIBBLE(b)    \
  P2=( LCD_RS | (b) );    \
  Pause(1);                \
  _EN=1;  \
  Pause(1);                \
  _EN=0;



#define CMD_NIBBLE(b)    \
  P2=( b );                 \
  Pause(1);                \
  _EN=1;                    \
  Pause(1);                \
  _EN=0;


byte LCD_Pos;

void Pause(int pause)
  { do
	  pause--;
	while(pause);
  }

byte LCD_State(__bit b)
{
	byte temp;

	Pause(1);
	P2=(LCD_RW | 0x0F);
	Pause(1);
	_EN=1;
	Pause(1);
	temp=(P2&0xF)<<4;
	_EN=0;
	if(b)
	{
	 Pause(1);
	 P2=(LCD_RW | 0x0F);
	 Pause(1);
	 _EN=1;
	 Pause(1);
	 temp=temp+(P2&0x0F);
	 _EN=0;
	}
	return (temp);
}

void LCD_SendCmd( byte val )
{
	__bit ea=EA;
	EA=0;
	CMD_NIBBLE((val>>4) & 0x0F)
	CMD_NIBBLE(val & 0x0F)
	while(LCD_State(1) & BUSY_FLAG);
	EA=ea;
}



void LCD_SendData( byte val )
{
	__bit ea=EA;
	EA=0;
	DATA_NIBBLE((val>>4) & 0x0F)
	DATA_NIBBLE(val & 0x0F)
	while(LCD_State(1) & BUSY_FLAG);
	LCD_Pos++;
	EA=ea;
}

/* --- Inicializace LCD - 4-bitovy interface ---   */
void LcdInit(void)

{
  P2=( 0 );                         // nulovani RS, RW a EN
  Pause(8000);                      //po zapnuti - prodleva > 15 ms
  CMD_NIBBLE(FUNCTION_INIT)         // 1. 8bitovy interface
  Pause(2000);                      //prodleva > 4,1 ms
  CMD_NIBBLE(FUNCTION_INIT)         // 2. 8bitovy interface
  Pause(50);                        //prodleva > 100 us
  CMD_NIBBLE(FUNCTION_INIT)         // 3. 8bitovy interface
  while(LCD_State(0)& BUSY_FLAG);   // cekani dokud neni LCD volny
  CMD_NIBBLE(FUNCTION_SET>>4)       // 4bitovy interface
  while(LCD_State(0)& BUSY_FLAG);   // cekani dokud neni LCD volny
  LCD_SendCmd(FUNCTION_SET);        // 4bitovy interface, 2 radky, 5x8 font
  LCD_SendCmd(CLEAR_DISPLAY);       // nulovani (vymazani) LCD
  LCD_SendCmd(ENTRY_MODE);          // normalni zapis na LCD
  LCD_SendCmd(DISPLAY_ON);          // LCD ON
  LCD_Pos=0;
}



void putchar(char c)

{
   static byte cursor;
   static byte last_cur;
   static __xdata char linebuff[16];
   byte n;

   if (c == '\r')
   {
	  LCD_SendCmd(DDRAM_ADDRESS | LINE2_ORIGIN);
	  if (last_cur == 0)
	  last_cur = cursor;
	  cursor = 0;
   }
   else if (c == '\n' || cursor == 16)
   {
	  LCD_SendCmd(CLEAR_DISPLAY);
	  if (last_cur < cursor)
		 last_cur = cursor;

	  for (n = 0 ; n < last_cur ; n++)
	  {
		 LCD_SendData(linebuff[n]);

	  }

	  LCD_SendCmd(DDRAM_ADDRESS | LINE2_ORIGIN);

	  last_cur = cursor = 0;

	  if (c != '\n')
		 putchar(c);
   }
   else
   {
	  linebuff[cursor++] = c;
	  LCD_SendData(c);
   }

}

