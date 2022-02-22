#define F_CPU 7372800UL
#include <avr/io.h>

#include <util/delay.h>

#include <avr/interrupt.h>

#define PIR_INPUT 6
#define SOUND_INPUT 0

#include "lcd.h"

static uint8_t tSS = 0;
static uint8_t tS = 0;
static uint8_t tM = 0;
static uint8_t tH = 0;

static uint8_t fMode = 0;

static uint16_t timeout = 10;
static int8_t sound_enable = 1;
static int8_t pir_enable = 1;

void delay(uint16_t timeout) {
	uint16_t i;
	for(i = 0; i < timeout; i++) {
		_delay_ms(1);
	}
}

void debounce() {
	_delay_ms(200);
	GIFR = _BV(INTF0) | _BV(INTF1);
}

ISR(INT0_vect) {
	if(sound_enable == 1){
		sound_enable = 0;
	}else{
		sound_enable = 1;
	}
	debounce();
}

ISR(INT1_vect) {
	if(pir_enable == 1){
		pir_enable = 0;
		}else{
		pir_enable = 1;
	}
	debounce();
}

void changeTime() {
	char time[9];

	time[0] = '0' + (tH / 10);
	time[1] = '0' + (tH % 10);
	time[2] = ':';
	time[3] = '0' + (tM / 10);
	time[4] = '0' + (tM % 10);
	time[5] = ':';
	time[6] = '0' + (tS / 10);
	time[7] = '0' + (tS % 10);
	time[8] = '\0';

	lcd_clrscr();
	if (fMode == 1) {
		lcd_gotoxy(0, 0);
		lcd_puts("Sound sensor");
	}
	if (fMode == 2) {
		lcd_gotoxy(0, 0);
		lcd_puts("Motion sensor");
	}
	lcd_gotoxy(0, 1); // pozicioniramo sat u sredinu lcd ekrana
	lcd_puts(time);
}

ISR(TIMER0_COMP_vect) {
	tSS++;

	if (tSS == 100) {
		tSS = 0;
		tS++;

		if (tS == 60) {
			tS = 0;
			tM++;
		}
		if (tM == 60) {
			tM = 0;
			tH++;
		}
		if (tH == 24) {
			tH = 0;
		}

	}
}

int main(void) {

	DDRB = 0x00; //Pir i sound kao izlazni

	DDRD = _BV(4);

	TCCR1A = _BV(COM1B1) | _BV(WGM10);
	TCCR1B = _BV(WGM12) | _BV(CS11);
	OCR1B = 28;

	TCCR0 = _BV(WGM01) | _BV(CS02) | _BV(CS00);
	OCR0 = 72;
	TIMSK = _BV(OCIE0);
	
	MCUCR = _BV(ISC01) | _BV(ISC11);
	GICR = _BV(INT0) | _BV(INT1);
	 

	
	sei();

	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	while (1) {
		if(sound_enable==1){
			if ((PINB & _BV(SOUND_INPUT)) == 0) {
				fMode = 1;
				changeTime();
			}
		}
		
		if (pir_enable==1)
		{
			if (PINB & _BV(PIR_INPUT)) {
				fMode = 2;
				changeTime();
			}
		}
		
		delay(2000);
		lcd_clrscr();

	}
}
