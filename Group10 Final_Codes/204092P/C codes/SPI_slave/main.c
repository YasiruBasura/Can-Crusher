/*
 * ATmega32_SPI_Slave.c
 * http://www.electronicwings.com
 */ 


#define F_CPU 8000000UL					/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>						/* Include AVR std. library file */
#include <util/delay.h>					/* Include Delay header file */
#include <stdio.h>
#include <string.h>						/* Include string header file */
//#include "LCD_16x2_H_file.h"			/* Include LCD header file */
#include "SPI_Slave.c"			/* Include SPI slave header file */


int main(void)
{
	unsigned char a;
	//char buffer[5];
	
	DDRA=0xff;
	
	
	//LCD_Init();
	SPI_Init();
	
	for(int i=0;i<4;i++)
	{
		a = SPI_Receive();
		
		if(a=='k')
		{
			PORTA=0b00000100;
		}
		else if(a=='i')
		{
			PORTA=0b00000010;
			_delay_ms(10);
		}
		else if(a=='n')
		{
			PORTA=0b00000100;
			_delay_ms(10);
		}
		
	}
	


	
	//LCD_String_xy(1, 0, "Slave Device");
	//LCD_String_xy(2, 0, "Receive Data:    ");
	/*while (1)
	{
		
		sprintf(buffer, "%d   ", count);
		LCD_String_xy(2, 13, buffer);
	}*/

}