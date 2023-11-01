#define F_CPU 16000000UL
#include <avr/io.h>        /* Include AVR std. library file */   
#include <util/delay.h>        

#define lcd_port PORTD    // connected lcd on to the port D      
#define lcd_data_dir  DDRD   // difinding the direction of the pins- input/out 
#define rs PD0                  
#define en PD1                 

void lcd_command( unsigned char );

void initialize (void)
{
	lcd_data_dir = 0xFF;     
	_delay_ms(15);           
	lcd_command(0x02); // 4-bit initialization of lcd      
	lcd_command(0x28);   // 2 line,5*7 matrix in 4-bit mode   
	lcd_command(0x0c);  //display on cursor off
	lcd_command(0x06);  //increment cursor
	lcd_command(0x01);  // clear display screen
	_delay_ms(2);
}

void lcd_command( unsigned char cmnd )
{
	// send higher 4 bit of data
	lcd_port = (lcd_port & 0x0F) | (cmnd & 0xF0);  
	lcd_port &= ~ (1<<rs);
	lcd_port |= (1<<en);
	_delay_us(1);
	lcd_port &= ~ (1<<en);

	
	_delay_us(200);

	// lower 4 bit of data
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
	lcd_command (0x80); // cursor at home 
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
	
	lcd_setCursor(2, 1);
	lcd_print("hello customer");
	lcd_setCursor(2,2 );
	lcd_print("Enter your can");
	
	
			
}
