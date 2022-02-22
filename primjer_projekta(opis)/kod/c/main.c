
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "utility.h"
#include "usart.h"
#include "spi.h"
#include "mfrc522.h"
#define BUZZER_PORT	PORTA
#define BUZZER_DDR  	DDRA
#define BUZZER_PIN  	0


void buzzerOn(uint16_t time);
void detectReaderVersion();


int main(void) {
	
	BUZZER_DDR = 0xff;
	BUZZER_PORT = ~_BV(0);
	
	// enable interrupts
	MCUCR = _BV(ISC01) | _BV(ISC11);
	GICR = _BV(INT0) | _BV(INT1);
	
	// used for storing status after communication with the reader
	uint8_t status;
	
	// buffer for fetching information from the reader
	uint8_t str[16];
	
	// initialize SPI, UART and MFRC
	spi_init();
	_delay_ms(1000);
	
	mfrc522_init();
	_delay_ms(1000);
	
	usart_init(9600);
	_delay_ms(500);

	// enable interrupts
	sei();
	
	detectReaderVersion();
   
	_delay_ms(3000);
   
   
	char buffer[64];
	while(1){
		// request information about any tag in range of the antenna 
		status = mfrc522_request(PICC_REQALL,str);
	   	usart_puts("Waiting...\n");
		   
		if(status == CARD_FOUND) {
		   // if card is found, try to fetch card id number
			status = mfrc522_get_card_serial(str);
			
			if(status == CARD_FOUND) {
				// send id number (as hex characters) through USART interface
				usart_puts(":");
				for(uint8_t i = 0; i < 5; ++i) {
					usart_hex(str[i]);
				}
				usart_puts("\n");
		   }
		   else {
			 usart_puts("Error reading serial!\n");
		   }
		   _delay_ms(2500);
		}
	 
		// Test for a tag every 1000ms
		_delay_ms(1000);
   }
	
	
}

// Detects and prints(sends through UART) reader version
void detectReaderVersion() {
	//check version of the reader
	uint8_t byte = mfrc522_read(VersionReg);
	
	if (byte == 0x92) {
		usart_puts("Reader detected ");
		usart_puts("MIFARE RC522 v2\n");

	} else if (byte == 0x91 || byte==0x90) {
		usart_puts("Reader detected ");
		usart_puts("MIFARE RC522v1\n");
	} else {
		usart_puts("No reader found\n");
	}
}


ISR(USART_RXC_vect)
{
	char data;
	
	// read the data from the RX buffer
	data = UDR;
	
	// 0 - unauthorized card
	// 1 - authorized card
	// using buzzer to inform a user
	switch (data) {
		case '0': 
			buzzerOn(250);
			_delay_ms(250);
			buzzerOn(250);
			break;
		case '1':
			buzzerOn(500);
			break;

	}
}

/* Interrupts INT0 and INT1 are used for changing privilege level
*	INT0 increment privilege, INT1 decrements privilege
*/
ISR(INT0_vect) {
	usart_puts(".+\n");
	debounce();

}

ISR(INT1_vect) {
	usart_puts(".-\n");
	debounce();
}

// Turns buzzer on for a specified amount of time
void buzzerOn(uint16_t time) {
	BUZZER_DDR = ~_BV(0);
	delay(time);
	BUZZER_DDR = 0xff;
}

