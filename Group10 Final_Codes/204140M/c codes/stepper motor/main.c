#ifndef F_CPU

#define F_CPU 16000000UL //freaquency of external crystal

#endif

#include <avr/io.h>

#include <util/delay.h>



//define interfacing pins between L293D and ATmega32

#define a1 (1<<PD4)

#define a2 (1<<PD5)

#define a3 (1<<PD6)

#define a4  (1<<PD7)



//For switch

#define SW (1<<PD3)    //Ir
#define SW2 (1<<PD2)   //ALL validation



int main(void){

	Stepper_motor();

	return(0);

}

void Stepper_motor(){
	
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
							PORTD |= a1;

							PORTD &= ~a2;

							PORTD &= ~a3;

							PORTD &= ~a4;

							_delay_ms(50);



							PORTD |= a1;

							PORTD &= ~a2;

							PORTD  &= ~ a3;

							PORTD |= a4;

							_delay_ms(20);
							
							

							
							PORTD &= ~a1;

							PORTD &= ~a2;

							PORTD &= ~a3;

							PORTD |= a4;

							_delay_ms(50);
							
							
							PORTD |= a1;

							PORTD &= ~a2;

							PORTD &= ~ a3;

							PORTD |= a4;

							_delay_ms(20);
							
							
							
							PORTD |= a1;

							PORTD &= ~a2;

							PORTD &= ~a3;

							PORTD &= ~a4;

							_delay_ms(50);
							break;
						}
					}

					


					

				}
				

				else if(!(PIND & SW)){

				

					


					PORTD |= a1;

					PORTD &= ~a2;

					PORTD &= ~a3;

					PORTD &= ~a4;

					_delay_ms(50);



					PORTD |= a1;

					PORTD &= ~a2;

					PORTD |= a3;

					PORTD &= ~a4;

					_delay_ms(20);
					
					

					
					PORTD &= ~a1;

					PORTD &= ~a2;

					PORTD |= a3;

					PORTD &= ~a4;

					_delay_ms(50);
					
					
					PORTD |= a1;

					PORTD &= ~a2;

					PORTD |= a3;

					PORTD &= ~a4;

					_delay_ms(20);
					
					
					
					PORTD |= a1;

					PORTD &= ~a2;

					PORTD &= ~a3;

					PORTD &= ~a4;

					_delay_ms(50);
					

					

					break;
				}

				

			}
			
		}
	}

	
	
	
};