#ifndef F_CPU

#define F_CPU 16000000UL //freaquency of external crystal

#endif
#include <avr/io.h>
#include <util/delay.h>




//lcd and key board
#define LCD PORTC
#define EN 0
#define RS 0
#define RW 1

 int get_pn;
 char can_no[10];
 

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
							_delay_ms(5);
							lcdcmd(0x80);//set the curser to first line
							lcd_Print(" ");
							lcdcmd(0x01);//clear lcd
							break;
						}
						//easy cash
						else if(x=='2'){
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
				int phone_no= atoi(no);//phone nimber in int
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
	_delay_ms(5);
	PORTD &= ~(1<<EN); // EN=0 for H to L pulse

	LCD = data<<4; // send low nibble;
	PORTD |=(1<<EN); //EN=1 for H to L pulse
	_delay_ms(5);
	PORTD &= ~(1<<EN);
	
}

void lcd_init(){
	//DDRC= 0xFF;
	DDRC= 0xF0; // Define  output LCD port(ALL c ports are outputs) 
	DDRD |=(1<<EN); //Define EN pin  as output
	DDRB=0xFF; // define RS and  RW pin as output
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

	

int main(void){
    while(1){
		keypad_func();
		
	}
	
	return 0;
}
