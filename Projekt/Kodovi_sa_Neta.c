// Link na ovaj kod i shemu: https://www.electronicwings.com/avr-atmega/pir-motion-sensor-interface-with-atmega16-32
#define F_CPU 8000000UL
#include <avr/io.h>
#define LED_OUTPUT		PORTB
#define PIR_Input		PINC

int main(void)
{
	DDRC = 0x00;	/* Set the PIR port as input port */
	DDRB = 0xff;	/* Set the LED port as output port */

    while(1)
    {
        LED_OUTPUT = PIR_Input;
	}
}

// Link na ovaj kod i shemu: https://www.gadgetronicx.com/pir-motion-sensor-interface-with-avr/
#include<avr/io.h>
#define F_CPU 8000000UL
#include<util/delay.h>
int main()
{
DDRC=0x00; //configuring PortC pin 0 as input
PORTC=0x00; 
DDRB=0x01; // configuring PortB as output
PORTB=0x00; // buzzer/LED off

while(1)
{
 if((PINC&(1<<0)))            // check for sensor pin PC.0 using bit
  {
   PORTB=0x01;           // buzzer /LED on
   _delay_ms(5000);
   PORTB=0x00;
  }
  else
  PORTB=0x00;  // buzzer/LED off
}           

return 0;
}


// Link na ovaj kod i shemu:https://exploreembedded.com/wiki/PIR_motion_Sensor_interface_with_Atmega128

/*
 * PIR sensor with Atmega128.c
 *
 * Created: 15-Apr-16 3:26:31 PM
 * Author : exploreembedded
 */ 

#include <avr/io.h>
#include "lcd.h"

#define PIR_sensor PA0

unsigned int PIR_Status = 0;
int main()
{
	
	DDRA = (0 << PIR_sensor);           // Configure PORTA as INput to connect switches
	/*Connect RS->PB0, RW->PB1, EN->PB2 and data bus to PORTC.4 to PORTC.7*/
	while(1)
	{
		while(((PINA) & (1<<PIR_sensor)) == 1)
		{
			LCD_SetUp(PB_0,PB_1,PB_2,P_NC,P_NC,P_NC,P_NC,PC_4,PC_5,PC_6,PC_7);
			LCD_Init(2,16);
			LCD_DisplayString("Motion Detected!");
			
		}
		 LCD_Clear();
		 
		while(((PINA) & (1<<PIR_sensor)) == 0) 
		{
		 LCD_SetUp(PB_0,PB_1,PB_2,P_NC,P_NC,P_NC,P_NC,PC_4,PC_5,PC_6,PC_7);
		 LCD_Init(2,16);		
		 LCD_DisplayString("Motion Ended!");
		}
		 LCD_Clear();	
	
		
				
	}
	
	return (0);
}