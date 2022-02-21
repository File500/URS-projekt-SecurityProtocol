#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


#define PIR_INPUT  6
#define SOUND_INPUT 0

int main(void)
{
	 
	DDRB = 0x00;
	
	DDRD = _BV(4);

	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 28;

	lcd_init(LCD_DISP_ON);


	while(1)
	{
		lcd_clrscr();
		
		 if(PINB & _BV(SOUND_INPUT)){
		 lcd_gotoxy(0,0);
		 lcd_puts("sound detected");
		} 
		
		
		if(PINB & _BV(PIR_INPUT)){
		lcd_gotoxy(0,1);
		lcd_puts("motion detected");
		}
		
		_delay_ms(5000);
		
		
	}
}