#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << RXCIE);/* Turn on the transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */

	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits */
}

ISR(USART_RXC_vect)
{
	PORTB=UDR;
}

int main()
{
	UART_init(9600);
	sei();
	DDRB=0xFF;
	while(1);
}