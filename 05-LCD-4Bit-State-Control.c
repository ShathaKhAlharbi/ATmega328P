#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// Sends a 4-bit nibble to pins PB2, PB3, PB4, PB5
void lcd_send_nibble(uint8_t nibble) {
	// Mask out PB2-PB5 (0x3C is 00111100) and shift the nibble into place
	PORTB = (PORTB & ~0x3C) | ((nibble & 0x0F) << 2);
	
	PORTC |= (1 << PC2); // Enable pulse
	_delay_us(1);
	PORTC &= ~(1 << PC2);
	_delay_ms(2);
}

void lcd_write_cmd(uint8_t cmd) {
	PORTC &= ~(1 << PC0); // RS = 0[cite: 1]
	PORTC &= ~(1 << PC1); // RW = 0[cite: 1]
	lcd_send_nibble(cmd >> 4);
	lcd_send_nibble(cmd & 0x0F);
}

void lcd_write_char(uint8_t data) {
	PORTC |= (1 << PC0);  // RS = 1[cite: 1]
	PORTC &= ~(1 << PC1); // RW = 0[cite: 1]
	lcd_send_nibble(data >> 4);
	lcd_send_nibble(data & 0x0F);
}

void lcd_init() {
	DDRB |= 0x3C; // PB2, PB3, PB4, PB5 as outputs[cite: 1]
	DDRC |= 0x07; // PC0, PC1, PC2 as outputs[cite: 1]
	
	_delay_ms(100); // Wait for LCD to stabilize
	
	// Robust 4-bit reset sequence
	lcd_send_nibble(0x03);
	_delay_ms(5);
	lcd_send_nibble(0x03);
	_delay_us(150);
	lcd_send_nibble(0x03);
	lcd_send_nibble(0x02); // Set to 4-bit mode
	
	lcd_write_cmd(0x28); // 2 lines, 5x7 matrix[cite: 1]
	lcd_write_cmd(0x0C); // Display ON[cite: 1]
	lcd_write_cmd(0x01); // Clear display[cite: 1]
	_delay_ms(2);
}

int main(void) {
	DDRD = 0x00; // Port D as input[cite: 1]
	PORTD = 0xFF; // Enable pull-ups[cite: 1]
	
	lcd_init();
	char buffer[16];
	
	while (1) {
		uint8_t sw = ~PIND; // Read switches (active low)[cite: 1]
		
		// Line 1: State 1[cite: 2]
		lcd_write_cmd(0x80);
		sprintf(buffer, "State 1 = %-3u", sw);
		for(int i=0; buffer[i]!=0; i++) lcd_write_char(buffer[i]);
		
		// Line 2: State 2 (Value + 50, max 255)[cite: 2]
		uint16_t res = (uint16_t)sw + 50;
		if (res > 255) res = 255;
		
		lcd_write_cmd(0xC0);
		sprintf(buffer, "State 2 = %-3u", (uint8_t)res);
		for(int i=0; buffer[i]!=0; i++) lcd_write_char(buffer[i]);
		
		_delay_ms(100);
	}
}
