/*
 * presentation_ir2.c
 *
 * Created: 5/23/2022 2:48:26 PM
 * Author : Nishath
 */ 

#include <avr/io.h>

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LCD PORTC
#define EN 0
#define RS 0
#define RW 1


int check_discard_bin=0;

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
	DDRC= 0xF0; // Define  output LCD port
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
	_delay_ms(2);
	//checking whether discard bin is empty or not
	lcdcmd(0x80);//set the curser to first line
	lcdcmd(0xc0);//set the curser to second line

}

int main(void)
{ 
	lcd_init();
	while(1){
			while(1){
				if(PINC & 0x01==0x01){
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
					break;
				}
				
			};
	
	
	}
}

