
#define F_CPU 16000000UL						/* define Clock Frequency */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include "lcd.c"				/* include 16x2 LCD Header file */
#include "USART_Interrupt.c"				/* include USART Header file */

#define SREG   _SFR_IO8(0x3F)

void GSM_Begin();
void GSM_Dialling();
void GSM_Response();
void GSM_Response_Display();
void GSM_Send_Msg(char* ,char*);


char buff[160];								/* buffer to store responses and messages */
char status_flag = 0;						/* for checking any new message */
volatile int buffer_pointer;

int price=2;					/*price given to a single can*/
int cans;						/* amount of cans inserted by the user */
int amount;						/* total value user will receive */
char mob_no[11]="0775499920";	/*user's phone number */






int main(void)
{
	char lh[10];
	DDRA=0xff;

	DDRB=0x07;
	
	
	buffer_pointer = 0;

	lcdint();
	USART_Init(9600);						    /* initialize USART communication */
	sei();
	lcd_line_one();
	lcd_string("GSM Initializing",16);
	_delay_ms(90); //original-3000
	lcd_clear();
	lcd_line_one();
	lcd_string("AT",2);
	GSM_Begin();								/* check GSM responses and initialize GSM */
	lcd_clear();
	
	//amount=cans*price;
	amount=1000;
	sprintf(lh,"LH %d",amount);
	GSM_Send_Msg("77110",lh);
	memset(lh,0,strlen(lh));	/* clear lh array */
	
	lcd_string(".....Sending....",16);  /* lcd print function */
	_delay_ms(70); //original-7000
	lcd_clear();   /* function to clear lcd */
	
	lcd_string("Sent to Little",14);
	_delay_ms(30); // //original-500
	lcd_clear();
	lcd_string("Hearts",6);
	_delay_ms(30); //original-500
	lcd_clear();
	
	GSM_Dialling();
	lcd_string("Reload send",11);
}

void GSM_Begin()
{

	while(1)
	{
		lcdcmd(0xc0);
		USART_SendString("ATE0\r");			/* send ATE0 to check module is ready or not */
		_delay_ms(30); //original-500
		if(strstr(buff,"OK"))
		{
			GSM_Response();                 /* get Response */
			memset(buff,0,160);
			break;
		}
		else
		{
			lcd_line_one();
			lcd_string("Error",5);
		}
	}
	_delay_ms(50);  // original-1000

	lcd_clear();
	lcd_line_one();
	lcd_string("Text Mode",9);
	lcdcmd(0xc0);
	USART_SendString("AT+CMGF=1\r");/* select message format as text */
	GSM_Response();
	_delay_ms(40);  // original-1000
}


/* ISR routine to save responses/new message */
ISR(USART_RXC_vect)
{
	buff[buffer_pointer] = UDR;							/* copy UDR(received value) to buffer */
	buffer_pointer++;
	status_flag = 1;						            /* flag for new message arrival */
}


void GSM_Send_Msg(char* num,char *sms)
{
	char sms_buffer[35];
	buffer_pointer=0;
	sprintf(sms_buffer,"AT+CMGS=\"%s\"\r",num);
	USART_SendString(sms_buffer);                       /*send command AT+CMGS="Mobile No."\r */
	_delay_ms(20); // original-200
	while(1)
	{
		if(buff[buffer_pointer]==0x3e)                  /* wait for '>' character*/
		{
			buffer_pointer = 0;
			memset(buff,0,strlen(buff));				/* emptying "buff" */
			USART_SendString(sms);                      /* send msg to 77110 */
			USART_TxChar(0x1a);                         /* send Ctrl+Z then only message will transmit*/
			break;
		}
		buffer_pointer++;
	}
	_delay_ms(30);  // original-300
	buffer_pointer = 0;
	memset(buff,0,strlen(buff));				/* emptying "buff" */
	memset(sms_buffer,0,strlen(sms_buffer));	/* emptying "sms_buffer" */
}

void GSM_Dialling()
{
	char call[35];
	sprintf(call,"ATD#111#2#%s#%d#1234#;\r",mob_no,amount);  /* build command */
	USART_SendString(call);						        /* send command ATD<Dial_code> */
	
}


void GSM_Response()
{
	unsigned int timeout=0;
	int CRLF_Found=0;  /* character elements found CL & LF */
	char CRLF_buff[2];  /* store the CL & LF) */
	int Response_Length=0;
	while(1)
	{
		if(timeout>=60000)								/*if timeout occur then return */
		return;
		Response_Length = strlen(buff);				/* getting the length of the response */
		if(Response_Length)						/* checking the availability of the response */
		{
			_delay_ms(2);
			timeout++;
			if(Response_Length==strlen(buff))
			{
				for(int i=0;i<Response_Length;i++)
				{
					memmove(CRLF_buff,CRLF_buff+1,1);	 /* changing the memory location of the response */
					CRLF_buff[1]=buff[i];
					if(strncmp(CRLF_buff,"\r\n",2))
					{
						if(CRLF_Found++==2)				/* search for \r\n in string */
						{
							GSM_Response_Display();		/* display response */
							return;
						}
					}

				}
				CRLF_Found = 0;

			}

		}
		_delay_ms(1);
		timeout++;
	}
	status_flag=0;
}

void GSM_Response_Display()
{
	buffer_pointer = 0;
	int lcd_pointer = 0;
	while(1)
	{
		if(buff[buffer_pointer]== '\r' || buff[buffer_pointer]== '\n')  /*search for \r\n in string */
		{
			buffer_pointer++;
		}
		else
		break;
	}


	lcdcmd(0xc0);
	while(buff[buffer_pointer]!='\r')								   /* display response till "\r" */
	{
		LCD_Char(buff[buffer_pointer]);
		buffer_pointer++;
		lcd_pointer++;
		if(lcd_pointer==15)								               /* check for end of LCD line */
		lcdcmd(0x80);
	}
	buffer_pointer=0;
	memset(buff,0,strlen(buff));  /* emptying memory block */
}



