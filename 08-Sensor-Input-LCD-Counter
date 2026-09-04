#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

// ===== LCD 4-bit Mode Definitions =====
#define LCD_DATA_PORT  PORTB
#define LCD_DATA_DDR   DDRB
#define LCD_CTRL_PORT  PORTC
#define LCD_CTRL_DDR   DDRC

#define LCD_RS         PC0    // Pin 23
#define LCD_RW         PC1    // Pin 24
#define LCD_EN         PC2    // Pin 25
#define SENSOR_PIN     PIND
#define SENSOR         PD0    // Pin 2

// Function prototypes
void LCD_cmd(unsigned char cmd);
void LCD_write(unsigned char data);
void LCD_String(char *str);
void init_LCD(void);
void LCD_send_nibble(unsigned char nibble, unsigned char rs);
void ByteToStr(uint16_t val, char *str);

// ===== Send 4-bit nibble to LCD =====
void LCD_send_nibble(unsigned char nibble, unsigned char rs) {
	// Clear high nibble of PORTB (PB4-PB7) then set it with our data
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (nibble & 0xF0);
	
	if (rs)
	LCD_CTRL_PORT |=  (1 << LCD_RS);   // Data mode
	else
	LCD_CTRL_PORT &= ~(1 << LCD_RS);   // Command mode
	
	LCD_CTRL_PORT &= ~(1 << LCD_RW);       // Always Write (Low)
	
	// Pulse Enable
	LCD_CTRL_PORT |=  (1 << LCD_EN);
	_delay_us(10);
	LCD_CTRL_PORT &= ~(1 << LCD_EN);
	_delay_ms(2);                          // Increased for simulation stability
}

void LCD_cmd(unsigned char cmd) {
	LCD_send_nibble(cmd & 0xF0, 0);        // High nibble
	LCD_send_nibble((cmd << 4) & 0xF0, 0); // Low nibble
}

void LCD_write(unsigned char data) {
	LCD_send_nibble(data & 0xF0, 1);        // High nibble
	LCD_send_nibble((data << 4) & 0xF0, 1); // Low nibble
}

void LCD_String(char *str) {
	while (*str) LCD_write(*str++);
}

void init_LCD(void) {
	_delay_ms(100); // Give Proteus time to start
	
	// Manual 4-bit initialization sequence
	LCD_send_nibble(0x30, 0); _delay_ms(10);
	LCD_send_nibble(0x30, 0); _delay_ms(1);
	LCD_send_nibble(0x30, 0); _delay_ms(1);
	LCD_send_nibble(0x20, 0); _delay_ms(1); // Set 4-bit mode
	
	LCD_cmd(0x28); // 2 lines, 5x7 matrix
	LCD_cmd(0x0C); // Display ON, Cursor OFF
	LCD_cmd(0x06); // Increment cursor
	LCD_cmd(0x01); // Clear display
	_delay_ms(5);
}

void ByteToStr(uint16_t val, char *str) {
	itoa(val, str, 10);
}

int main(void) {
	// Set PB4-PB7 as output
	LCD_DATA_DDR |= 0xF0;
	// Set PC0, PC1, PC2 as output
	LCD_CTRL_DDR |= (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);
	// Set PD0 as input
	DDRD &= ~(1 << SENSOR);
	// Enable internal pull-up on PD0 if using a simple button to GND
	PORTD |= (1 << SENSOR);

	uint16_t counter = 0;
	char str[7];

	init_LCD();

	LCD_cmd(0x80);
	LCD_String("Product Counter");
	LCD_cmd(0xC0);
	LCD_String("Count: 0");

	while (1) {
		// Check for SENSOR (Active Low)
		if (!(PIND & (1 << SENSOR))) {
			_delay_ms(20); // Debounce
			if (!(PIND & (1 << SENSOR))) {
				counter++;
				
				LCD_cmd(0xC7); // Move cursor to after "Count: "
				ByteToStr(counter, str);
				LCD_String(str);
				LCD_String("   "); // Clear ghost digits
				
				// Wait for sensor to be released (Go back to HIGH)
				while (!(PIND & (1 << SENSOR)));
				_delay_ms(20);
			}
		}
	}
	return 0;
}
