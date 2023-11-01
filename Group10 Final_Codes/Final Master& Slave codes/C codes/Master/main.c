


#ifndef F_CPU
#define F_CPU 16000000UL		/* freaquency of external crystal */
#endif					/* Define CPU Frequency e.g. here its 8MHz */
#include <avr/io.h>						/* Include AVR std. library file */
#include <util/delay.h>					/* Include Delay header file */
#include <stdio.h>
#include <string.h>
#include "SPI_Master.c"			/* Include SPI master header file */


/*----------------------------------------------------------------------------------------------------*/

//define interfacing pins between L293D and ATmega32

#define a1 (1<<PD4)

#define a2 (1<<PD5)

#define a3 (1<<PD6)

#define a4  (1<<PD7)



//For switch

#define SW (1<<PD3)    //Ir
#define SW2 (1<<PD2)   //ALL validation

//lcd and key board
#define LCD PORTC
#define EN 0
#define RS 0
#define RW 1

 int get_pn;
 //int get_cn;//number of cans
 char can_no[10];
 int phone_no;
 int option=0;
 int validcan=0;
 

unsigned char keypad();
int keypad_func(){
	unsigned char x;
	char no[10];
	int i=0;
	int i1=0;
	DDRA =0x0F; // make PA0 to PA3 = output and PA4 to PA7 = Input
	lcd_init();
	while(1){
		PORTA=0xF0; // make all 4 column 1 and all rows 0
		if(PINA!= 0xF0){
			_delay_ms(5);
			x=keypad();
			if (x=='*')
			{
				lcdcmd(0x01);//clear lcd
				lcdcmd(0x80);//set the curser to first line
				lcd_Print("1-Fast Charge");
				lcdcmd(0xc0);//set the curser to second line
				lcd_Print("2-Ez Cash");
				_delay_ms(100);
				lcdcmd(0x01);
				lcdcmd(0x80);//set the curser to first line
				lcd_Print("3-Donate");
				
				while(1){
					if(PINA!= 0xF0){
						x=keypad();
						//charge
						if(x=='1'){
							option=1;
							_delay_ms(5);
							lcdcmd(0x80);//set the curser to first line
							lcd_Print(" ");
							lcdcmd(0x01);//clear lcd
							break;
						}
						//easy cash
						else if(x=='2'){
							option=2;
							get_pn=1;//get phone no
							_delay_ms(5);
							lcdcmd(0x80);//set the curser to first line
							lcd_Print(" ");
							lcdcmd(0x01);//clear lcd
							lcd_Print("Enter the Phone");
							lcdcmd(0xc0);
							lcd_Print("Number");
							lcdcmd(0x80);
							_delay_ms(100);
							lcdcmd(0x01);
							lcd_Print("Press = to");
							lcdcmd(0xc0);
							lcd_Print("Finish");
							lcdcmd(0x80);
							_delay_ms(100);
							lcdcmd(0x01);
							break;
						}
						//donate
						else if(x=='3'){
							option=3;
						}
						
					}
					/*else
					{
						lcddata(x);
						
					}*/

				}
				
				
			}
			else if (x=='=')
			{
				
				get_pn=0;
				lcdcmd(0x01);
				lcd_Print("Entered phone NO.:");
				lcdcmd(0xc0);
				lcd_Print(&no);//print the no
				lcdcmd(0x80);
				//lcd_Print(&can_no);//print the no
				int phone_no= atoi(no);//phone number in int
				_delay_ms(100);
				lcdcmd(0x01);
			
				_delay_ms(100);
				lcdcmd(0x01);
				i=0;
				return 0;		
			}
			else
			{
				
				lcddata(x);
				if(get_pn==1){
					no[i]=x;
					i++;
				}
				else{
					can_no[i1]=x;
					i1++;
				}
				
			}
		}
	}
	
}
void lcdcmd(unsigned char cmd){
	PORTB &=~(1<<RS); //RS=0 for command (1111 1110)
	PORTB &=~(1<<RW); //RW=0 for write (1111 1101)
	LCD= cmd & 0xF0;  //Send upper nibble
	PORTD|=(1<<EN); //EN=1 for H to L pulse
	_delay_ms(1);
	PORTD &= ~(1<<EN); // EN=0 for H to L pulse

	LCD = cmd<<4; // send low nibble;
	PORTD |=(1<<EN); //EN=0for H to L pulse
	_delay_ms(1);
	PORTD &= ~(1<<EN);
}

void lcd_Print (char*str){
	int i;
	for(i=0;str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		lcddata(str[i]);
	}
}

void lcddata(unsigned char data){
	PORTB |= (1<<RS); //RS=1 for data
	PORTB &= ~(1<<RW); //RW=0 for write
	LCD= data & 0xF0;  //Send upper nibble
	PORTD|= (1<<EN); //EN=1 for H to L pulse
	_delay_ms(2);
	PORTD &= ~(1<<EN); // EN=0 for H to L pulse

	LCD = data<<4; // send low nibble;
	PORTD |=(1<<EN); //EN=1 for H to L pulse
	_delay_ms(2);
	PORTD &= ~(1<<EN);
}

void lcd_init(){
	DDRC= 0xFF;
	DDRD |=(1<<EN); //Define EN pin  as output
	DDRB=0b11110111; //0xff define RS and  RW pin as output
	PORTD&= ~(1<<EN); //initialize EN=0
	lcdcmd(0x33);
	lcdcmd(0x32);
	lcdcmd(0x28); // LCD in 4 bit mode
	lcdcmd(0x0E); //display on cursor on
	/*lcddata(72);
	lcddata(69);
	lcddata(76);
	lcddata(76);
	lcddata(79);*/
	lcd_Print("WELCOME");
	_delay_ms(40);
	lcdcmd(0x01); //clear LCD
	lcd_Print("Enter the Number");
    lcdcmd(0xc0);//set the curser to second line 
	lcd_Print("of cans and");
	_delay_ms(40);
	lcdcmd(0x01); //clear LCD
	lcdcmd(0x80);//set the curser to first line 
	lcd_Print("Press + to start");
	_delay_ms(40);
	 lcdcmd(0xc0);//set the curser to second line
	 lcd_Print("the process : ");
	  //lcdcmd(0x80);	 
}

unsigned char keypad(){
	PORTA=0b11111110; // make 1st row 0
	if((PINA & (1<<PINA4))==0){
		_delay_ms(3);
		return '7';
		}else if((PINA & (1<<PINA5))==0){
		_delay_ms(3);
		return '8';
		}else if((PINA & (1<<PINA6))==0){
		_delay_ms(3);
		return '9';
		}else if((PINA & (1<<PINA7))==0){
		_delay_ms(3);
		return '/';
	}
	PORTA=0b11111101;
	if((PINA & (1<<PINA4))==0){
		_delay_ms(3);
		return '4';
		}else if((PINA & (1<<PINA5))==0){
		_delay_ms(3);
		return '5';
		}else if((PINA & (1<<PINA6))==0){
		_delay_ms(3);
		return '6';
		}else if((PINA & (1<<PINA7))==0){
		_delay_ms(3);
		return '+';
	}
	PORTA=0b11111011;
	if((PINA & (1<<PINA4))==0){
		_delay_ms(3);
		return '1';
		}else if((PINA & (1<<PINA5))==0){
		_delay_ms(3);
		return '2';
		}else if((PINA & (1<<PINA6))==0){
		_delay_ms(3);
		return '3';
		}else if((PINA & (1<<PINA7))==0){
		_delay_ms(3);
		return '-';
	}
	PORTA=0b11110111;
	if((PINA & (1<<PINA4))==0){
		_delay_ms(3);
		return 'C';
		}else if((PINA & (1<<PINA5))==0){
		_delay_ms(3);
		return '0';
		}else if((PINA & (1<<PINA6))==0){
		_delay_ms(3);
		return '=';
		}else if((PINA & (1<<PINA7))==0){
		_delay_ms(3);
		return '*';
	}
}

int Stepper_motor(){
	
	//Set a1, a2, a3, a4 as output

	DDRD |= a1 | a2 | a3| a4;

	//Set input & pull-up resistor

	DDRD &= ~SW;

	PORTD |= SW;
	
	DDRD &= ~SW2;

	PORTD |= SW2;
	
	while(1){
		
		if((PIND & SW) && (PIND & SW2)){

			while(1){
				
				if((!(PIND & SW2))){
					
					while(1){
						if((!(PIND & SW))){
							validcan++;

							PORTD |= a1;

							PORTD &= ~a2;

							PORTD &= ~a3;

							PORTD &= ~a4;

							_delay_ms(20);


							PORTD |= a1;

							PORTD &= ~a2;

							PORTD  &= ~ a3;

							PORTD |= a4;

							_delay_ms(4);
							
							
							PORTD &= ~a1;

							PORTD &= ~a2;

							PORTD &= ~a3;

							PORTD |= a4;

							_delay_ms(20);
							
							
							PORTD |= a1;

							PORTD &= ~a2;

							PORTD &= ~ a3;

							PORTD |= a4;

							_delay_ms(4);
							
							
							PORTD |= a1;

							PORTD &= ~a2;

							PORTD &= ~a3;

							PORTD &= ~a4;

							_delay_ms(20);
							return 0;
						}
					}
				}
				

				else if(!(PIND & SW)){


					PORTD |= a1;

					PORTD &= ~a2;

					PORTD &= ~a3;

					PORTD &= ~a4;

					_delay_ms(20);



					PORTD |= a1;

					PORTD &= ~a2;

					PORTD |= a3;

					PORTD &= ~a4;

					_delay_ms(4);
					
					

					
					PORTD &= ~a1;

					PORTD &= ~a2;

					PORTD |= a3;

					PORTD &= ~a4;

					_delay_ms(20);
					
					
					PORTD |= a1;

					PORTD &= ~a2;

					PORTD |= a3;

					PORTD &= ~a4;

					_delay_ms(4);
					
					
					
					PORTD |= a1;

					PORTD &= ~a2;

					PORTD &= ~a3;

					PORTD &= ~a4;

					_delay_ms(20);
					

					return 0;
				}
			}
		}
	}
}
int ir2_func(){
	while(1){
		
		if(PINB & 0b00001000){
			lcdcmd(0x01); //clear LCD
			lcdcmd(0x80);//set the curser to first line
			lcd_Print("Discard bin is");
			lcdcmd(0xc0);//set the curser to second line
			lcd_Print("full");
			lcdcmd(0x80);//set the curser to first line
			_delay_ms(50);
			lcdcmd(0x01); //clear LCD
			lcd_Print("Clear the");
			lcdcmd(0xc0);//set the curser to second line
			lcd_Print("discard bin");
			lcdcmd(0x80);//set the curser to first line
			
		}
		else{
			return 0;
		}
		
	}
	
}



int main(void){
	
	int r_slave=0;
	
	int trans_num=0;
	int temp=0;
	
    while(1){
		
		keypad_func();
		
		temp=phone_no;
		
		SPI_Init();
		SS_Enable;
		
		SPI_Write(option);
		
		if(option==2)
		{
			for(int i=0;i<10;i++)  /* preparing the phone number to transmit to the slave */
			{
				trans_num=temp%10;
				SPI_Write(trans_num);
				temp /=10;
			}
		}
		
		
		int can_number= atoi(can_no);
		
		r_slave=SPI_Read();

		if(r_slave)
		{
			for(int k=0;k<can_number;k++){
			ir2_func();
			Stepper_motor();
			}
			
		}
		
		SPI_Write(validcan);
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------------------------------*/
/*int main(void)  /* SPI comm */
/*{
	int count=1,bag=2;
	
	
	SPI_Init();
	SS_Enable;
	

	
	SPI_Write(count);
	_delay_ms(50);
	SPI_Write(bag);
	
	
}*/