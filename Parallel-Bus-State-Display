#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

/* LCD wiring: PORTB = data, PC0 = RS, PC1 = RW, PC2 = E */
#define LCD_DATA_DIR DDRB
#define LCD_DATA_OUT PORTB

#define LCD_CTRL_DIR DDRC
#define LCD_CTRL_OUT PORTC
#define LCD_PIN_RS PC0
#define LCD_PIN_RW PC1
#define LCD_PIN_E PC2

/* Send enable pulse */
static void lcd_trigger(void)
{
	LCD_CTRL_OUT |= (1 << LCD_PIN_E);
	_delay_us(1);
	LCD_CTRL_OUT &= ~(1 << LCD_PIN_E);
	_delay_ms(2);
}

/* Write a command to LCD */
void lcd_write_cmd(uint8_t c)
{
	LCD_CTRL_OUT &= ~(1 << LCD_PIN_RS);
	LCD_CTRL_OUT &= ~(1 << LCD_PIN_RW);
	LCD_DATA_OUT = c;
	lcd_trigger();
}

/* Write a data character */
void lcd_write_char(char c)
{
	LCD_CTRL_OUT |= (1 << LCD_PIN_RS);
	LCD_CTRL_OUT &= ~(1 << LCD_PIN_RW);
	LCD_DATA_OUT = (uint8_t)c;
	lcd_trigger();
}

/* Print text */
void lcd_print(char *t)
{
	while (*t)
	{
		lcd_write_char(*t);
		t++;
	}
}

/* LCD setup */
void lcd_setup(void)
{
	LCD_DATA_DIR = 0xFF;
	LCD_CTRL_DIR |= (1 << LCD_PIN_RS) | (1 << LCD_PIN_RW) | (1 << LCD_PIN_E);

	LCD_CTRL_OUT &= ~(1 << LCD_PIN_RW);

	_delay_ms(30);
	lcd_write_cmd(0x38);
	lcd_write_cmd(0x0C);
	lcd_write_cmd(0x01);
	_delay_ms(2);
	lcd_write_cmd(0x06);
}

int main(void)
{
	uint8_t sw_reading;
	uint16_t computed;
	char textbuf[20];
	
	/* Port-D as inputs */
	DDRD = 0x00;
	/* Enable internal pull-up resistors to prevent floating pins */
	PORTD = 0xFF;
	
	lcd_setup();
	
	while (1)
	{
		/* Read input switches */
		sw_reading = PIND;
		
		/* Display first state */
		lcd_write_cmd(0x80);
		lcd_print("State 1 = ");
		/* Use %-3u to pad the string with blank spaces to clear ghosting */
		snprintf(textbuf, sizeof(textbuf), "%-3u", sw_reading);
		lcd_print(textbuf);
		
		/* Calculate value + 50 with limit */
		computed = (uint16_t)sw_reading + 50;
		if (computed > 255)
		{
			computed = 255;
		}
		
		/* Display second state */
		lcd_write_cmd(0xC0);
		lcd_print("State 2 = ");
		/* Use %-3u here as well */
		snprintf(textbuf, sizeof(textbuf), "%-3u", (uint8_t)computed);
		lcd_print(textbuf);
		
		_delay_ms(180);
	}
	return 0;
}
