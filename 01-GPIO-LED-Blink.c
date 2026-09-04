#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define LED PB3
#define LED_ONN PORTB|= (1<<PINB5);

int main(void)
{
	DDRB |= (1 << LED);

	while (1)
	{
		LED_ONN
		PORTB |= (1 << LED);
		_delay_ms(2000);
		PORTB &= ~(1 << LED);
		_delay_ms(1000);
	}
}
