/*
 * torelay.h
 *
 * Created: 5/12/2022 5:49:20 PM
 *  Author: Nishshanka
 


#ifndef TORELAY_H_
#define TORELAY_H_





#endif TORELAY_H_ */
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

void switchon();
void switchoff();

void switchon()
{
	PORTC|=(1<<PC2);
}

void switchoff()
{
	PORTC&=~(1<<PC2);
}
