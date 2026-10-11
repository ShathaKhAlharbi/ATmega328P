/*
 * Microcontroller: ATmega328P
 * Description: Reads 8-bit binary input from DIP switch (PORTD) 
 *              and displays values on LM016L LCD in 8-bit parallel mode.
 */

#ifndef F_CPU
#define F_CPU 16000000UL // 16MHz clock frequency
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// LCD Control Pin Definitions
#define RS PC0 // Pin 23
#define RW PC1 // Pin 24
#define EN PC2 // Pin 25

// LCD Command Function
void LCD_Command(unsigned char cmnd) {
    PORTB = cmnd;           // Output command byte on PORTB (D0-D7)[cite: 3]
    PORTC &= ~(1 << RS);    // RS = 0 (Command register)
    PORTC &= ~(1 << RW);    // RW = 0 (Write operation)
    PORTC |= (1 << EN);     // Pulse EN High
    _delay_us(1);
    PORTC &= ~(1 << EN);    // Pulse EN Low
    _delay_ms(3);
}

// LCD Data/Character Function
void LCD_Char(unsigned char data) {
    PORTB = data;           // Output data byte on PORTB (D0-D7)[cite: 3]
    PORTC |= (1 << RS);     // RS = 1 (Data register)
    PORTC &= ~(1 << RW);    // RW = 0 (Write operation)
    PORTC |= (1 << EN);     // Pulse EN High
    _delay_us(1);
    PORTC &= ~(1 << EN);    // Pulse EN Low
    _delay_ms(1);
}

// Write String to LCD
void LCD_String(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        LCD_Char(str[i]);
    }
}

// LCD Initialization
void LCD_Init(void) {
    DDRB = 0xFF; // Set PORTB as output for 8-bit LCD data bus[cite: 3]
    DDRC |= (1 << RS) | (1 << RW) | (1 << EN); // Set LCD control pins as outputs[cite: 3]
    
    _delay_ms(20);
    LCD_Command(0x38); // 8-bit mode, 2-line display, 5x7 matrix
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x01); // Clear Display
    _delay_ms(2);
    LCD_Command(0x06); // Increment cursor (shift right)
}

int main(void) {
    // Configure PORTD pins as Inputs with internal pull-ups
    DDRD = 0x00;        // Set PORTD as Input[cite: 3]
    PORTD = 0xFF;       // Enable internal pull-up resistors on PD0-PD7[cite: 3]

    LCD_Init();
    
    char buffer[17];

    // Static Header
    LCD_Command(0x80); // Move cursor to Line 1
    LCD_String("DIP SWITCH READ");

    while (1) {
        /*
         * Since the DIP switches pull PORTD pins to GND when switched ON,
         * reading PIND yields 0 for ON and 1 for OFF.
         * Inverting (~PIND) converts active-low switches to positive binary values.
         */
        uint8_t raw_input = PIND;
        uint8_t switch_state = ~raw_input; // Invert to make ON = 1, OFF = 0

        // Format raw binary and decimal values onto Line 2
        LCD_Command(0xC0); // Move cursor to Line 2
        
        // Print 8-bit binary representation + decimal value
        sprintf(buffer, "VAL:%u  HEX:0x%02X", switch_state, switch_state);
        LCD_String(buffer);

        _delay_ms(200); // Polling delay
    }

    return 0;
}
