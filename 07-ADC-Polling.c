#define F_CPU 16000000UL 
#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>

#define LCD_DATA PORTB 
#define LCD_RC PC0
#define LCD_RW PC1
#define LCD_E PC2

#define START_SWITCH PD0

void init_LCD(void); 
void LCD_cmd(unsigned char cmd);
void LCD_write(  unsigned char data);

//void LCD_SetCuors(uint8_t row , uint8_t col);

void LCD_str(char* str);

void LCD_SetCuors(uint8_t row , uint8_t col){
	uint8_t address ;
	if (row == 0 )
	address = 0x80 +  col ;
	else
	address = 0xC0 +  col ;
	LCD_cmd(address);
}

int main(void)
{
	uint8_t count =0 ;
	 char buffer[16] ;
	
	//0 = in , 1 = out 
	DDRB = 0xFF ; //out 
	DDRC = 0xFF ; //out
	DDRD &=~(1<<START_SWITCH); //in 
	
	init_LCD();
	LCD_str("Prouduct counter ");
	LCD_SetCuors(1,0);
	LCD_str("counter = 0 ");
	
    /* Replace with your application code */
    while (1) 
    {
		if (PORTD & (1<<START_SWITCH)){
			_delay_ms(5); 
			if (PORTD & (1<<START_SWITCH)) {
				count ++ ; 
				sprintf(buffer , "%3d",count );
				LCD_SetCuors(1,0);
				LCD_str(buffer);
				while (PORTD & (1<<START_SWITCH))
				_delay_ms(20);
    }
		}
	}
}
void init_LCD(void){
	LCD_cmd(0x38); // 8 bit 
	_delay_ms(1);
	LCD_cmd(0x01); // clear 
	_delay_ms(1);
	LCD_cmd(0x02); // home 
	_delay_ms(1); 
	LCD_cmd(0x06); // inc 
	_delay_ms(1); 
	LCD_cmd(0x80); // first line
	_delay_ms(1);
}

void LCD_cmd(unsigned char cmd) {
	LCD_DATA = cmd ; 
	PORTC &=~(1<<LCD_RC);//RC = 0 in command 
	PORTC |=(1<<LCD_E);//EE
	PORTC &=~(1<<LCD_RW);//
	_delay_ms(2);
	PORTC &=~(1<<LCD_E);
}

void LCD_write(unsigned char data){
	LCD_DATA = data ;
	PORTC |=(1<<LCD_RC);//RC = 1 in write 
	PORTC |=(1<<LCD_E);//EE
	PORTC &=~(1<<LCD_RW);//WR
	_delay_ms(2);
	PORTC &=~(1<<LCD_E);
	
}

void LCD_str(char* str){
	while (*str){
		LCD_write(*str++); 
	}
}
