#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#define LED_OUTPUT_SOUND 3
#define LED_OUTPUT_PIR 0
#define PIR_INPUT  7
#define SOUND_INPUT 2

int main(void)
{
	DDRC = 0x00; 
	DDRB = 0xff;
	DDRA = 0xff;
	DDRD = 0x00;
	
	PORTA = 0xff;


	while(1)
	{
		
		 if(PINC & _BV(SOUND_INPUT)){
			PORTB |= _BV(LED_OUTPUT_SOUND);
			PORTA = 0b11101111;
			_delay_ms(2000);
		} 
		
		
		if(PIND & _BV(PIR_INPUT)){
			PORTB |= _BV(LED_OUTPUT_PIR);
			//while((PINB & _BV(PIR_INPUT)));
			_delay_ms(10000);
		}
		
		
		PORTB = 0x00;
		
		_delay_ms(2000);
	}
}