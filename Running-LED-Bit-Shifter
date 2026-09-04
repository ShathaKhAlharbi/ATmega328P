/*
 * LED_Blinking_circuit.c
 *
 * Purpose: Turn ON LEDs sequentially with delay
 */

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD = 0b11111111;   // Set PORTD as output

    while (1)
    {
        int mybit = 0;
        PORTD = 0;
        _delay_ms(100);

        for (int i = 0; i <= 7; i++)
        {
            mybit = 1;
            for (int j = 1; j <= i; j++)
            {
                mybit *= 2;   // Shift bit left
            }

            PORTD = mybit;   // Turn ON LED
            _delay_ms(100);
        }
    }
}
