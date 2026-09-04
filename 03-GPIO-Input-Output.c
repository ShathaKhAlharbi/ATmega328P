#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>03-GPIO-Input-Output

#define LED_ON PORTD = |=(1<<PD5);

int main(void)
{
   DDRD = 0xFF;

    while (1) 
    {
	
		}
		
    }


