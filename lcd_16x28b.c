/************************************************************************/
/*  lcd_16x2.c - Biblioteca de manipula��o de m�dulo LCD 16x2 com RL78  */
/*               em modo de 8bits                                       */
/*                                                                      */
/************************************************************************/

#include "lcd_16x28b.h"


//**************************************************************************
//* Fun��o de atraso em ms baseada em ciclos de maquina
//**************************************************************************
//* Argumentos de chamada:
//* char time : tempo de atraso desejado em mili segundos
//**************************************************************************
//* Retorno : nada
//**************************************************************************

void LCD_delay_ms(unsigned char time)
{
  unsigned int temp;
  for (;time;time--)//mantem o la�o at� que tempo=0
    for (temp=CPU_CLOCK/9;temp;temp--) //mantem o la�o at� que temp=0, 
                                       //com temp atrelada ao clock
      __no_operation(); //Microcontrolador inoperante (instru��o NOP)
}


//**************************************************************************
//* Fun��o de escrita de 1 byte no display
//**************************************************************************
//* Argumentos de chamada:
//* char endereco : 0 se instru��o, 1 se dado
//* char dado : dado ou comando a ser enviado ao display
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send8(char addr, char data )
{
  // configura a linha rs dependendo do modo selecionado
  PIN_LCD_RS = addr;      // faz pino RS assumir 0 para comando ou 1 para dado
  LCD_delay_ms(1);        // Aguarda 1ms
  PORT_LCD_DATA8 = data;  // coloca dados ou comando no port conectado ao D16x2
 
  PIN_LCD_EN = 1;   // habilita LCD para receber dados ou comando
  LCD_delay_ms(1);  // Aguarda 1ms
  PIN_LCD_EN = 0;   // desabilita LCD para receber dados ou comando
}

//**************************************************************************
//* Fun��o de escrita de 1 byte no display
//**************************************************************************
//* Argumentos de chamada:
//* char endereco : 0 se instru��o, 1 se dado
//* char dado : dado ou comando a ser enviado ao display
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_send4(char addr, char data )
{
  // configura a linha rs dependendo do modo selecionado
  PIN_LCD_RS = addr;      // faz pino RS assumir 0 para comando ou 1 para dado
  LCD_delay_ms(1);        // Aguarda 1ms
  PORT_LCD_DATA8 = data;  // coloca dados ou comando no port conectado ao D16x2
 
  PIN_LCD_EN = 1;   // habilita LCD para receber dados ou comando
  LCD_delay_ms(1);  // Aguarda 1ms
  PIN_LCD_EN = 0;   // desabilita LCD para receber dados ou comando
}




//**************************************************************************
//* Fun��o de inicializa��o do display em modo 8 bits
//**************************************************************************
//* Argumentos de chamada:
//
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_init_4_bits(void)

{
  // configura os pinos do microcontrolador de controle do display como sa�das
  PIN_LCD_EN_DIR = 0;
  PIN_LCD_RS_DIR = 0;
  PORT_LCD_DATA8_DIR = 0x00;
  
  // coloca os pinos em n�vel l�gico 0
  PORT_LCD_DATA8 = 0;
  PIN_LCD_RS = 0;
  PIN_LCD_EN = 0;
  LCD_delay_ms(20);  // Aguarda 15ms
  
  // envia uma seq��ncia de 3 vezes 0x03 e depois 0x02 para iniciar o Display
  // conforme fluxograma no datasheet
  
    LCD_send4(PIN_LCD_RS_0,0x30); // Envia comando para inicializar o display 
    LCD_delay_ms(5);              // Aguarda 5ms
    
    LCD_send4(PIN_LCD_RS_0,0x30); // Envia comando para inicializar o display 
    LCD_delay_ms(5);              // Aguarda 5ms

    LCD_send4(PIN_LCD_RS_0,0x30); // Envia comando para inicializar o display 
    LCD_delay_ms(5);              // Aguarda 5ms
    
    //*******************************************************
    LCD_send4(PIN_LCD_RS_0,0x30); // 2 linhas 
    LCD_delay_ms(5);              // Aguarda 5ms 
    //******************************************************
    LCD_send4(PIN_LCD_RS_0,0x20); 
    LCD_delay_ms(5);
    LCD_send4(PIN_LCD_RS_0,0x80);  
    LCD_delay_ms(5);

    //*********************Display_OFF***********************
    LCD_send4(PIN_LCD_RS_0,0x30); 
    LCD_delay_ms(5);              
    LCD_send4(PIN_LCD_RS_0,0x80); 
    LCD_delay_ms(5);
    //*******************************************************

    //********************Display_ON*************************
    LCD_send4(PIN_LCD_RS_0,0x30); // ENTRY MODE SET - Desloca o cursor para a 
                                  // para a direita
    LCD_delay_ms(5);              // Aguarda 5ms
    LCD_send4(PIN_LCD_RS_0,0x10 ); // ENTRY MODE SET - Desloca o cursor para a 
                                  // para a direita
    LCD_delay_ms(5);              // Aguarda 5ms
    //*******************************************************

    //*******************ENTRY_MODE**************************
    LCD_send4(PIN_LCD_RS_0,0x60); // ENTRY MODE SET - Desloca o cursor para a direita  
    LCD_delay_ms(5);              // Aguarda 5ms

    LCD_send4(PIN_LCD_RS_0,0x00); // DISPLAY CONTROL= Display ligado, sem cursor  
    LCD_delay_ms(5);              // Aguarda 5ms
    LCD_send4(PIN_LCD_RS_0,0xC0); // DISPLAY CONTROL= Display ligado, sem cursor  
    LCD_delay_ms(5);              // Aguarda 5ms
    
}




//**************************************************************************
//* Fun��o de posicionamento do cursor do display 16x2
//**************************************************************************
//* Argumentos de chamada:
//* char x : coluna a ser posicionado o cursor (iniciando em 0 e vai ate 15)
//* char y : linha a ser posicionado o cursor (0 ou 1)
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_pos_xy(char x, char y)
{
  if (y==0)         // reconhece linha 0 do display selecionada
    LCD_send8(PIN_LCD_RS_0,0x80|x); // as colunas da linha 0 possuem endere�o  
                                    //de 0x80 at� 0x8F
  else if(y==1)     // reconhece linha 1 do display selecionada
    LCD_send8(PIN_LCD_RS_0,0xC0|x); // as colunas da linha 0 possuem endere�o  
                                    //de 0xC0 at� 0xCF 
  LCD_delay_ms(5);                  // aguarda posicionamento do cursor
}

//**************************************************************************
//* Fun��o de escrita de um caractere no display
//**************************************************************************
//* Argumentos de chamada :
//* char c : caractere a ser escrito
//**************************************************************************
//* Retorno : nada
//**************************************************************************
void LCD_write_char(char data)

{
  LCD_send8(PIN_LCD_RS_1,data); //envia um caractere da variavel data para
                                //o display
  LCD_delay_ms(5);              // aguarda Reconehcimento do Display
}

//**************************************************************************
//* Fun��o de escrita de uma string no display
//**************************************************************************
//* Argumentos de chamada:
//* char *ptr : ponteiro para o caractere inicial da string
//**************************************************************************
//* Retorno : nada
//**************************************************************************
//* Observa��es :
// Ponteiros s�o variaveis que apontam para um endere�o de mem�ria
// na mem�ria, toda string � terminada com o caractere 0
//**************************************************************************
void LCD_write_string (char *ptr)
{

  char dado;
  while (*ptr) // verifica se o ponteiro esta apontando p o final da string
  {
    dado=*ptr; //variavel dado recebe caractere apontado pelo ponteiro *ptr
    LCD_send8(PIN_LCD_RS_1,dado); //envia caractere atual para display
    LCD_delay_ms(5); //aguarda tempo de envio do caracter para o display
    ptr++; //incrementa ponteiro para apontar para proximo caracter da string
  }
}
