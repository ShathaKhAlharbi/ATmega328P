/*
 * LED Lighting Project 
 */

#define F_CPU 16000000UL // Defining external crystal 16 MHz 
#include <avr/io.h>
#include <avr/delay.h>
#define LED_ON PORTD |=(1 << PIND5); // on mode 
#define LED_OFF PORTD &=~(1 << PIND5);

int main(void)
{
	/* DDR = Data Direction register ( input or output port ) 
	Input -> Zeros , Output -> Ones  */
  
  /* PORT D is 8 bits DDRD is 8 bits
  If PORT D is defined as an output,
  every single bit of DDRD must be given logic value of 1 */
  
    DDRD = 0xFF; // or in bin = 0b11111111 or Dec = 255
	// Light is off
    PORTD = 0x00 ;
	
    while (1) 
    {
		LED_ON
		_delay_ms(50) ; 
		LED_OFF
		_delay_ms(50) ; 
		
		
    }
}


