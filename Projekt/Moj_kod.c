#define F_CPU 7372800UL
#include <avr/io.h>
#define LED_OUTPUT 0
#define PIR_INPUT  0
#define SOUND_INPUT 1

int main(void)
{
	DDRC = 0x00;	/* Set the PIR port as input port */
	DDRB = 0xff;	/* Set the LED port as output port */

    while(1)
    {
        if(PINC & _BV(PIR_INPUT)){ // Ako ovo ne radi probat PINC & (1 << PIR_INPUT)
            PORTB = _BV(LED_OUTPUT);
            _delay_ms(5000);
            PORTB = 0x00;
        }
        else if(PINC & _BV(SOUND_INPUT)){
            PORTB = _BV(LED_OUTPUT);
            _delay_ms(5000);
            PORTB = 0x00;
        }
        else{
            PORTB = 0x00;
        }
	}
}
