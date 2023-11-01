/*
 * ultra_final.c
 *
 * Created: 5/11/2022 12:48:12 PM
 * Author : ravindu
 */ 



#define F_CPU 16000000UL
#include <avr/io.h>            /* Include AVR std. library file */
#include <util/delay.h>        
#include <stdlib.h>            //convert integer to character array 

#define lcd_port PORTD          // connected lcd on to the port D
#define lcd_data_dir  DDRD      // difinding the direction of the pins- input/out
#define rs PD0                  
#define en PD1     
             
#define US_PORT PORTC           // connected ultrasonic to port C
#define	US_PIN	PINC            // initialization of pin register
#define US_DDR 	DDRC            // data direction register to set the data direction flow 

#define US_TRIG_POS	PC0          
#define US_ECHO_POS	PC1        


#define US_ERROR		-1     //difinding variable to  working or not
#define	US_NO_OBSTACLE	-2

int distance, previous_distance;




void HCSR04Init();
void HCSR04Trigger();

void lcd_command( unsigned char );

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
		if(!(US_PIN & (1<<US_ECHO_POS)))
		continue;	
		else
		break;		
	}

	if(i==600000)
	return US_ERROR;	
	
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



void initialize (void)
{
	lcd_data_dir = 0xFF;     //lcd pins that connected to micro-controller as out puts
	_delay_ms(15);           
	lcd_command(0x02);       
	lcd_command(0x28);      
	lcd_command(0x0c);
	lcd_command(0x06);
	lcd_command(0x01);
	_delay_ms(2);
}

void lcd_command( unsigned char cmnd )
{

	lcd_port = (lcd_port & 0x0F) | (cmnd & 0xF0);  
	lcd_port &= ~ (1<<rs);
	lcd_port |= (1<<en);
	_delay_us(1);
	lcd_port &= ~ (1<<en);
	
	
	_delay_us(200);
	
	
	lcd_port = (lcd_port & 0x0F) | (cmnd << 4);
	lcd_port |= (1<<en);
	_delay_us(1);
	lcd_port &= ~ (1<<en);
	_delay_ms(2);
}

void lcd_clear()
{
	lcd_command (0x01);   
	_delay_ms(2);        
	lcd_command (0x80);   
}


void lcd_print (char *str)
{
	
	int i;
	for(i=0; str[i]!=0; i++)
	{
		
		lcd_port = (lcd_port & 0x0F) | (str[i] & 0xF0);
		lcd_port |= (1<<rs);
		lcd_port|= (1<<en);
		_delay_us(1);
		lcd_port &= ~ (1<<en);
		_delay_us(200);
		lcd_port = (lcd_port & 0x0F) | (str[i] << 4);
		lcd_port |= (1<<en);
		_delay_us(1);
		lcd_port &= ~ (1<<en);
		_delay_ms(2);
	}
}



void lcd_setCursor(unsigned char x, unsigned char y){    
	unsigned char adr[] = {0x80, 0xC0};    
	lcd_command(adr[y-1] + x-1);
	_delay_us(100);
}

int main()
{
	initialize();           

	char numberString[4];   

	while(1) {
		
		uint16_t r;
		
		_delay_ms(100);


		
		
		HCSR04Init();  // set io port direction of sensor


		  
		lcd_print("hello customer !");
		_delay_ms(10);
		lcd_setCursor(1, 2); 
		lcd_print("enter your can");
		_delay_ms(1000);
		lcd_clear();                                             
		while(1)
		{
			
			HCSR04Trigger();     // calling the ultrasonic sound wave generator function
			
			r=GetPulseWidth();      // getting the duration of the ultrasound took echo back
       
	        // handle errors
			if(r==US_ERROR)               
			{
				lcd_setCursor(1, 1);     
				lcd_print("Error!");
			}
			else
			{
				
				distance=(r*0.034/2.0);	
				
				
				if (distance != previous_distance)    
				{
					lcd_clear();
				}
				
				lcd_setCursor(1, 1);  
				   
				
				if((distance>45)&&(distance<50))
				{
					lcd_setCursor(1, 1); 
					lcd_print("can  detected");
					
				}
				else
				{
					lcd_setCursor(3, 1); 
					lcd_print("can not detected");
				}
				
				     
				itoa(distance, numberString, 10);   // converting
				lcd_setCursor(8, 2);  
				lcd_print(numberString); 
				lcd_setCursor(10, 2);   
				lcd_print(" cm");
				
				
				
				
				previous_distance = distance;
				_delay_ms(30);
			}
		}
		
	}
}
