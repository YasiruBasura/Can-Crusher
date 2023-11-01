/*
 * ultra_final.c
 *
 * Created: 1/11/2022 12:48:12 PM
 * Author : Ravindu
 */ 



#define F_CPU 16000000UL
#include <avr/io.h>            
#include <util/delay.h>        
#include <stdlib.h>            

                  
#define US_PORT PORTC           
#define	US_PIN	PINC           
#define US_DDR 	DDRC            

#define US_TRIG_POS	PC6       
#define US_ECHO_POS	PC7     
#define led1 PC2                


#define US_ERROR		-1     
#define	US_NO_OBSTACLE	-2

int distance, previous_distance;
int sec0=0;
int sec1=0;
int sec2=0;



void HCSR04Init();
void HCSR04Trigger();



void HCSR04Init()
{
	
	
	US_DDR|=(1<<US_TRIG_POS);
}

void HCSR04Trigger()
{  
	
	US_PORT|=(1<<US_TRIG_POS);	
	
	_delay_us(15);				
	
	US_PORT&=~(1<<US_TRIG_POS);	
}

uint16_t GetPulseWidth()
{
	
	
	uint32_t i,result;


	for(i=0;i<600000;i++)
	{
		if(!(US_PIN & (1<<US_ECHO_POS))){
		continue;	
		}
		else
		{
			break;
		}
			
	}

	if(i==600000){
		return US_ERROR;
	}
		
	TCCR1A=0X00;
	TCCR1B=(1<<CS11);	
	TCNT1=0x00;			

	
	for(i=0;i<600000;i++)              
	{
		if(US_PIN & (1<<US_ECHO_POS))
		{
			if(TCNT1 > 60000) break; else continue;  
		}
		else
		break;
	}

	if(i==600000)
	return US_NO_OBSTACLE;	


	result=TCNT1;          

	
	TCCR1B=0x00;

	if(result > 60000)
	return US_NO_OBSTACLE;	
	else
	return (result>>1);
}

int ults_func0(){
	
	char numberString[4];

	while(1) {
		
		uint16_t r;
		
		_delay_ms(100);

		
		HCSR04Init();
		while(1)
		{
			
			HCSR04Trigger();

			
			r=GetPulseWidth();
			
			
			
			distance=(r*0.034/2.0);	
			
			if((distance>45)&&(distance<50))
			{
				
				sec0=1;
                return sec0;
				
			}
			
			previous_distance = distance;
			_delay_ms(30);
			
		}
	}	
}


int ults_func1(){
	
	char numberString[4];

	while(1) {
		
		uint16_t r;
		
		_delay_ms(100);

		HCSR04Init();
		while(1)
		{
			
			HCSR04Trigger();

			r=GetPulseWidth();
			
			distance=(r*0.034/2.0);
			
			if((distance>45)&&(distance<50))
			{	
				sec1=1;
				
				return sec1;
				
			}
			
			previous_distance = distance;
			_delay_ms(30);
			
		}
		
	}
	
	
}

int ults_func2(){
	
	char numberString[4];

	while(1) {
		
		uint16_t r;
		
		_delay_ms(100);


		
		
		HCSR04Init();


		
		while(1)
		{
			
			HCSR04Trigger();

			
			r=GetPulseWidth();
			
			
			
			distance=(r*0.034/2.0);
			
			
			
			
			
			if((distance>45)&&(distance<50))
			{
				
				sec2=1;
				return sec2;
				
			}
			
			previous_distance = distance;
			_delay_ms(30);
			
		}
		
	}
	
	
}
