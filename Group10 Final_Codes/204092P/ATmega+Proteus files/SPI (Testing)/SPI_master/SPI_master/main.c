/*
 * ATmega16_Master.c
 * http://www.electronicwings.com
 */ 


#define F_CPU 8000000UL					/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>						/* Include AVR std. library file */
#include <util/delay.h>					/* Include Delay header file */
#include <stdio.h>
#include <string.h>
//#include "LCD_16x2_H_file.h"			/* Include LCD header file */
#include "SPI_Master.c"			/* Include SPI master header file */

int main(void)
{
	unsigned char count[4]="kin";
	//char buffer[5];
	
	//LCD_Init();
	
	SPI_Init();	
	SS_Enable;
	
	for(int i=0;i<4;i++)
	{
		SPI_Write(count[i]);
		
		
	}
	
	
	
	
	
	
	/*LCD_String_xy(1, 0, "Master Device");
	LCD_String_xy(2, 0, "Sending Data:    ");
	SS_Enable;
	count = 0;
	while (1)
	{
		SPI_Write(count);
		sprintf(buffer, "%d   ", count);
		LCD_String_xy(2, 13, buffer);
		count++;
		_delay_ms(500);
	}*/
		
}