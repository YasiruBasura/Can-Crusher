/*
 * GccApplication14.c
 *
 * Created: 5/12/2022 5:20:57 PM
 * Author : Nishshanka
 */ 
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "torelay.h"

/*void switchon();
void switchoff();
*/
int main(void)
{
    /* Replace with your application code */
	DDRC|=(1<<PC2);
	PORTC&=~(1<<PC2);
	
    while (1) 
    {
		switchon();
		_delay_ms(10000);
		switchoff();
		_delay_ms(10000);
    }

}
/*
void switchon()
{
	PORTC|=(1<<PC0);
}

void switchoff()
{
	PORTC&=~(1<<PC0);
}*/




