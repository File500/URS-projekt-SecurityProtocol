#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#define LED_OUTPUT 3
#define PIR_INPUT  0
#define SOUND_INPUT 1

int main(void)
{
	DDRC = 0x00;
	DDRA = 0x00;	/* Set the PIR port as input port */
	DDRB = 0xff;
	DDRD = 0xff;	/* Set the LED port as output port */

	while(1)
	{
		
		 if(PINC & _BV(SOUND_INPUT)){
			PORTB = _BV(LED_OUTPUT);
			_delay_ms(2000);
			PORTB = 0x00;
			_delay_ms(2000);
		} 
	
	}
}