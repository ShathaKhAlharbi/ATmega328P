#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

/* ------------------------------------------------------------------ */
/*  LCD Pin Mapping                                                     */
/* ------------------------------------------------------------------ */
#define LCD_RS  PC0
#define LCD_RW  PC1
#define LCD_EN  PC2

/* ------------------------------------------------------------------ */
/*  LCD Low-Level Functions                                             */
/* ------------------------------------------------------------------ */

/* Send a command byte to the LCD (RS=0) */
static void LCD_Command(uint8_t cmd)
{
    PORTB  = cmd;                                    /* data bus        */
    PORTC &= ~((1 << LCD_RS) | (1 << LCD_RW));      /* RS=0, RW=0      */
    PORTC |=  (1 << LCD_EN);
    _delay_ms(1);
    PORTC &= ~(1 << LCD_EN);
}

/* Send a data byte to the LCD (RS=1) */
static void LCD_Data(uint8_t data)
{
    PORTB  = data;
    PORTC |=  (1 << LCD_RS);                         /* RS=1            */
    PORTC &= ~(1 << LCD_RW);                         /* RW=0            */
    PORTC |=  (1 << LCD_EN);
    _delay_ms(1);
    PORTC &= ~(1 << LCD_EN);
}


static void LCD_Init(void)
{
    _delay_ms(20);               /* power-on stabilisation             */
    LCD_Command(0x38);           /* 8-bit bus, 2 lines, 5x8 font       */
    LCD_Command(0x0C);           /* display ON, cursor OFF             */
    LCD_Command(0x06);           /* auto-increment cursor              */
    LCD_Command(0x01);           /* clear display                      */
    _delay_ms(2);                /* FIX: clear needs >= 1.52 ms        */
}

/* Write a null-terminated string to the LCD */
static void LCD_String(const char *str)
{
    while (*str)
        LCD_Data((uint8_t)(*str++));
}

/* ------------------------------------------------------------------ */
/*  ADC Configuration                                                   */
/* ------------------------------------------------------------------ */

static void ADC_Init(void)
{
    /* Internal 1.1V reference, channel 0 initially */
    ADMUX  = (1 << REFS1) | (1 << REFS0);
    /* Enable ADC, prescaler /64 (16 MHz / 64 = 250 kHz � within spec) */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}

static uint16_t ADC_Read(uint8_t channel)
{
    /* Select channel, keep internal 1.1V reference */
    ADMUX  = (1 << REFS1) | (1 << REFS0) | (channel & 0x07);
    _delay_ms(1);                /* allow mux + reference to settle    */
    ADCSRA |= (1 << ADSC);       /* start conversion                   */
    while (ADCSRA & (1 << ADSC)); /* wait for completion               */
    return ADC;                  /* 10-bit result                      */
}

/* ------------------------------------------------------------------ */
/*  Main Application                                                    */
/* ------------------------------------------------------------------ */
int main(void)
{
    /* ---- I/O Direction Setup ---- */
    DDRB  = 0xFF;                                     /* LCD data bus   */
    DDRC  = (1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN); /* LCD ctrl */
    DDRD |= (1 << PD0) | (1 << PD1);                 /* Fan, Heater    */
    /* PD2, PD3: button inputs with internal pull-ups */
    PORTD |= (1 << PD2) | (1 << PD3);

    LCD_Init();
    ADC_Init();

    /* Setpoint and its allowed range */
    int setpoint    = 20;
    const int SET_MIN = 10;
    const int SET_MAX = 35;

    /*
     * FIX #4 � Hysteresis offset in tenths of a degree.
     * Upper threshold = (setpoint + 2) �C
     * Lower threshold = (setpoint - 2) �C
     */
    const int HYST = 2;          /* � 2 �C hysteresis band             */

    char lcd_buf[17];            /* 16 chars + null terminator         */

    while (1)
    {
        /* ============================================================ */
        /*  1. Temperature Acquisition                                   */
        /* ============================================================ */
        uint16_t adc_value = ADC_Read(3);   /* LM35 on ADC3            */
        uint16_t temp_x10 = (uint16_t)((adc_value * 1100UL + 512) / 1024);

        /* Split into whole degrees and one fractional digit */
        uint8_t temp_whole = (uint8_t)(temp_x10 / 10);
        uint8_t temp_frac  = (uint8_t)(temp_x10 % 10);

        /* Thresholds in tenths of a degree, derived from setpoint */
        uint16_t upper = (uint16_t)((setpoint + HYST) * 10);
        uint16_t lower = (uint16_t)((setpoint - HYST) * 10);

        /* ============================================================ */
        /*  2. Control Logic with True Hysteresis                        */
        /* ============================================================ */
        if (temp_x10 > upper)
        {
            /* Above upper threshold ? activate cooling */
            PORTD |=  (1 << PD0);    /* Fan ON                         */
            PORTD &= ~(1 << PD1);    /* Heater OFF                     */
        }
        else if (temp_x10 < lower)
        {
            /* Below lower threshold ? activate heating */
            PORTD &= ~(1 << PD0);    /* Fan OFF                        */
            PORTD |=  (1 << PD1);    /* Heater ON                      */
        }
        /*
         * else: temperature is inside the hysteresis band ?
         * do NOT change PORTD; whatever was running keeps running.
         * This is true hysteresis � prevents rapid switching.
         */

        /* ============================================================ */
        /*  3. Button Handling � edge detection (FIX #9)                */
        /* ============================================================ */
        static uint8_t prev_btn = (1 << PD2) | (1 << PD3); /* released  */
        uint8_t btn = PIND & ((1 << PD2) | (1 << PD3));

        if (btn != prev_btn)          /* state changed (press OR release) */
        {
            _delay_ms(40);            /* debounce                         */
            btn = PIND & ((1 << PD2) | (1 << PD3));

            /* Detect falling edge on PD2 (was HIGH, now LOW = pressed)  */
            if (!(btn & (1 << PD2)) && (prev_btn & (1 << PD2)))
            {
                if (setpoint < SET_MAX) setpoint++;   /* UP               */
            }
            /* Detect falling edge on PD3 */
            if (!(btn & (1 << PD3)) && (prev_btn & (1 << PD3)))
            {
                if (setpoint > SET_MIN) setpoint--;   /* DOWN             */
            }

            prev_btn = btn;           /* update previous state            */
        }

        /* ============================================================ */
        /*  4. LCD Refresh                                               */
        /* ============================================================ */
        LCD_Command(0x01);   /* clear display                           */
        _delay_ms(2);        /* mandatory post-clear delay              */

        /* -- Line 1: temperature + setpoint -- */
        LCD_Command(0x80);   /* cursor to line 1, position 0           */
        snprintf(lcd_buf, sizeof(lcd_buf),
                 "Temp:%2u.%uC/S:%2d",
                 temp_whole, temp_frac, setpoint);
        LCD_String(lcd_buf);

        /* -- Line 2: fan and heater state -- */
        LCD_Command(0xC0);   /* cursor to line 2, position 0            */
        snprintf(lcd_buf, sizeof(lcd_buf),
                 "Fan:%-3s Heat:%-3s",
                 (PORTD & (1 << PD0)) ? "ON"  : "OFF",
                 (PORTD & (1 << PD1)) ? "ON"  : "OFF");
        LCD_String(lcd_buf);

        _delay_ms(100);      /* main loop ? 10 Hz                      */
    }
}
