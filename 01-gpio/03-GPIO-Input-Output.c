#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED_ON  PORTD |= (1 << PORTD5)
#define LED_OFF PORTD &= ~(1 << PORTD5)

int main(void)
{
    // Set PD5 as OUTPUT
    DDRD |= (1 << DDD5);

    while (1) 
    {
        LED_ON;          // Turn LED on
        _delay_ms(500);  // Wait 500ms
        
        LED_OFF;         // Turn LED off
        _delay_ms(500);  // Wait 500ms
    }

    return 0;
}
