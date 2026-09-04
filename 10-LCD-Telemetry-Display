#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h> // Required for itoa()

// *********** LCD PROGRAM STARTS *********** //
// port B selected as LCD data port
// register select signal is connected to port C pin 0
// read/write signal is connected to port C pin 1
// enable signal is connected to port C pin 2
#define LCD_DATA PORTB
#define LCD_RS PC0
#define LCD_RW PC1
#define LCD_E PC2

void init_LCD(void);
void LCD_cmd(unsigned char cmd);
void LCD_write(unsigned char data);
void LCD_String(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);

// *********** LCD Initialization *********** //
void init_LCD(void)
{
	LCD_cmd(0x38); // Initialization of 16x2 LCD in 8bit mode
	_delay_ms(1);
	LCD_cmd(0x01); // clear LCD display screen
	_delay_ms(1);
	LCD_cmd(0x02); // Return home
	_delay_ms(1);
	LCD_cmd(0x06); // make increment in cursor
	_delay_ms(1);
	LCD_cmd(0x80); // go to the first line and 0th position
	_delay_ms(1);
}

// *********** sending command on LCD *********** //
void LCD_cmd(unsigned char cmd)
{
	LCD_DATA = cmd;         // Data line are set to send command
	PORTC &= ~(1<<LCD_RS);  // LCD_RS sets 0
	PORTC |= (1<<LCD_E);    // make enable high
	PORTC &= ~(1<<LCD_RW);  // LCD_RW sets 0
	_delay_ms(2);
	PORTC &= ~(1<<LCD_E);   // make enable from high to low
}

// *********** write data on LCD (single character) *********** //
void LCD_write(unsigned char data)
{
	LCD_DATA = data;        // Data line are set to send data
	PORTC |= (1<<LCD_RS);   // LCD_RS sets 1
	PORTC |= (1<<LCD_E);    // make enable high
	PORTC &= ~(1<<LCD_RW);  // LCD_RW sets 0
	_delay_ms(2);
	PORTC &= ~(1<<LCD_E);   // make enable from high to low
}

// *********** Display null-terminated string on LCD *********** //
void LCD_String(char *str)
{
	while(*str) // Continue until end of string ('\0')
	{
		LCD_write(*str++); // Write current character and move to next
	}
}

// *********** Set cursor position on LCD *********** //
void LCD_SetCursor(uint8_t row, uint8_t col)
{
	uint8_t address;
	if(row == 0)
	address = 0x80 + col; // First row starts at address 0x80
	else
	address = 0xC0 + col; // Second row starts at address 0xC0
	
	LCD_cmd(address); // Send cursor position command to LCD
}

// *********** Main Function *********** //
int main(void)
{
	DDRB = 0xFF; // set LCD data port as output
	DDRC = 0x0F; // set LCD signals (RS, RW, E) as output
	
	init_LCD();  // initialize LCD
	_delay_ms(50);
	
	LCD_cmd(0x0C); // display on, cursor off
	_delay_ms(50);
	LCD_cmd(0x01); // Clear LCD display screen
	
	LCD_String("TEMP="); // Display title on first line
	_delay_ms(50);
	
	LCD_cmd(0xC0); // move cursor to the start of 2nd line
	_delay_ms(50);
	
	LCD_cmd(0x0C); // display on, cursor off
	_delay_ms(50);
	
	LCD_String("SPD="); // Display title on second line
	_delay_ms(50);
	
	unsigned int r0 = 768, r1 = 113; // Define integer: first row, second row
	char ch[4] = {' '};

	while(1)
	{
		// First Row (Temperature)
		for (int j=0; j<3; j++) {
			ch[j] = ' ';
		}
		
		LCD_SetCursor(0, 5); // Start from row 1 and 6 column
		itoa(r0, ch, 10);    // convert integer to ASCII
		
		for (int j=0; j<3; j++) {
			if (ch[j] < '0' || ch[j] > '9')
			LCD_write(' ');
			else
			LCD_write(ch[j]);
		}
		
		r0 = r0 + 1;
		if (r0 > 999) {
			r0 = 0;
		}

		// Second Row (Speed)
		for (int j=0; j<3; j++) {
			ch[j] = ' ';
		}
		
		LCD_SetCursor(1, 4); // Start from row 2 and 5 column
		itoa(r1, ch, 10);    // convert integer to ASCII
		
		for (int j=0; j<3; j++) {
			if (ch[j] < '0' || ch[j] > '9')
			LCD_write(' ');
			else
			LCD_write(ch[j]);
		}
		
		r1 = r1 + 1;
		if (r1 > 999) {
			r1 = 0;
		}
		
		_delay_ms(50);
	}
}
