/*
 * Assignment 4: Digital Measurement System
 * Hardware: ATmega328P, LCD (LM016L), Potentiometer
 * Frequency: 16MHz
 */

#ifndef F_CPU
#define F_CPU 16000000UL // Set clock to 16MHz [cite: 118, 249]
#endif

#include <avr/io.h>
#include <avr/interrupt.h> // Required for Timer interrupts [cite: 252, 290]
#include <util/delay.h>
#include <stdio.h>

// LCD Control Pin Definitions [cite: 8]
#define RS PC0
#define RW PC1
#define EN PC2

volatile uint8_t update_display = 0; // Flag to trigger ADC read in main loop

// Timer1 Overflow Interrupt Service Routine [cite: 265, 294]
ISR(TIMER1_OVF_vect) {
    update_display = 1; // Set flag when timer overflows [cite: 296]
}

// LCD Command Function
void LCD_Command(unsigned char cmnd) {
    PORTB = cmnd;           // Put command on PORTB [cite: 7]
    PORTC &= ~(1 << RS);    // RS = 0 for command [cite: 8]
    PORTC &= ~(1 << RW);    // RW = 0 for write [cite: 8]
    PORTC |= (1 << EN);     // Enable pulse [cite: 8]
    _delay_us(1);
    PORTC &= ~(1 << EN);
    _delay_ms(3);
}

// LCD Data Function
void LCD_Char(unsigned char data) {
    PORTB = data;           // Put data on PORTB [cite: 7]
    PORTC |= (1 << RS);     // RS = 1 for data [cite: 8]
    PORTC &= ~(1 << RW);    // RW = 0 for write [cite: 8]
    PORTC |= (1 << EN);     // Enable pulse [cite: 8]
    _delay_us(1);
    PORTC &= ~(1 << EN);
    _delay_ms(1);
}

// LCD Initialization
void LCD_Init() {
    DDRB = 0xFF; // Set PORTB as output for data [cite: 7, 196]
    DDRC |= (1 << RS) | (1 << RW) | (1 << EN); // Set control pins as output [cite: 8]
    _delay_ms(20);
    LCD_Command(0x38); // 8-bit mode, 2 lines, 5x7 dots
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x01); // Clear Display
    _delay_ms(2);
}

// ADC Initialization [cite: 10]
void ADC_Init() {
    // AVCC reference (REFS0) and Channel 5 (MUX0, MUX2) [cite: 6, 10]
    ADMUX = (1 << REFS0) | (1 << MUX2) | (1 << MUX0); 
    // Enable ADC and set Prescaler to 64 [cite: 10]
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); 
}

// Read ADC Value [cite: 11]
uint16_t ADC_Read() {
    ADCSRA |= (1 << ADSC);          // Start conversion
    while(ADCSRA & (1 << ADSC));    // Wait for conversion to finish
    return ADCW;                    // Return 10-bit result (0-1023) [cite: 16]
}

int main(void) {
    LCD_Init();
    ADC_Init();
    
    // Timer1 Setup [cite: 199, 242]
    TCCR1B |= (1 << CS12);          // Start Timer1 with 256 prescaler [cite: 167, 241]
    TIMSK1 |= (1 << TOIE1);         // Enable Timer1 Overflow Interrupt [cite: 234, 242]
    
    sei(); // Enable global interrupts [cite: 257, 305]
    
    char buffer[16];

   while (1) {
	   if (update_display) {
		   uint16_t raw_val = ADC_Read(); // [cite: 11]

		   // --- DISTANCE CALCULATION (0-50.0) ---
		   // We multiply by 10 to keep one decimal place as an integer
		   uint16_t dist_total = (raw_val * 500UL) / 1023;
		   uint16_t dist_whole = dist_total / 10;
		   uint16_t dist_decimal = dist_total % 10;

		   // --- WEIGHT CALCULATION (1.00-6.00) ---
		   // Range is 5.00, offset is 1.00. Multiply by 100 for two decimals
		   uint32_t wgt_total = 100 + ((uint32_t)raw_val * 500UL) / 1023;
		   uint16_t wgt_whole = wgt_total / 100;
		   uint16_t wgt_decimal = wgt_total % 100;

		   // Display Line 1: DIST [cite: 14]
		   LCD_Command(0x80);
		   sprintf(buffer, "DIST: %u.%u cm  ", dist_whole, dist_decimal);
		   for(int i=0; buffer[i] != '\0'; i++) LCD_Char(buffer[i]);
		   
		   // Display Line 2: WGT [cite: 15]
		   LCD_Command(0xC0);
		   sprintf(buffer, "WGT: %u.%02u kg  ", wgt_whole, wgt_decimal);
		   for(int i=0; buffer[i] != '\0'; i++) LCD_Char(buffer[i]);
		   
		   update_display = 0; // [cite: 292, 296]
	   }
   }
}