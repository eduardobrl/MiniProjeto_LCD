#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"
#include "lcd_16x28b.h"
#pragma location = "OPTBYTE"// Configura watchdog
__root __far const char opbyte0 = WDT_OFF;
// Configura detector de baixa tensão
#pragma location = "OPTBYTE"
__root __far const char opbyte1 = LVD_OFF;
// oscilador 32MHz flash high speed
#pragma location = "OPTBYTE"

__root __far const char opbyte2 = FLASH_HS | CLK_32MHZ;
// debug ativado, com apagamento em caso de falha de autenticação
#pragma location = "OPTBYTE"
__root __far const char opbyte3 = DEBUG_ON_ERASE;
/* Configura security ID */
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};
void main( void )
{
  LCD_init_4_bits();
  LCD_pos_xy(1,0);
  LCD_write_string("LCD 16x2 8b");
  LCD_pos_xy(4,1);
  LCD_write_string("FEI");
  LCD_pos_xy(13,1);
  LCD_write_char ('O');
  LCD_write_char ('K');
  LCD_send8(0,PISCA_CURSOR_ON);
  LCD_send8(0,PISCA_CURSOR_OFF);
  LCD_send8(0,APAGA_DISPLAY);
  LCD_send8(0,ACENDE_DISPLAY);
  LCD_send8(0,CLR_DISPLAY);
  LCD_send8(0,CURSOR_ON);
  LCD_send8(0,CURSOR_HOME);
  LCD_send8(0,PISCA_CURSOR_ON);
  LCD_send8(0,SHIFT_R_CURSOR);
  LCD_send8(0,SHIFT_R_CURSOR);
  LCD_send8(0,SHIFT_R_CURSOR);
  LCD_send8(0,SHIFT_L_CURSOR);
  LCD_send8(0,CURSOR_HOME);
  LCD_send8(0,PISCA_CURSOR_OFF);
  LCD_send8(0,CLR_DISPLAY);
  LCD_write_char ('O'); LCD_write_char ('K');
  while(1);
}