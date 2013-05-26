/* Tenkeyless replacement controller board parameters. */

#include "hid_liber.h"

/* Specifies the ports and pin numbers for the rows */
uint8_t *const  row_ddr[NROW] = {
                                   _DDRB,                  _DDRB,
                                                   _DDRC,  _DDRC,
   _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,  _DDRD,
   _DDRF,  _DDRF,                  _DDRF,  _DDRF,  _DDRF,  _DDRF
};

uint8_t *const row_port[NROW] = {
                                  _PORTB,                 _PORTB,
                                                  _PORTC, _PORTC,
  _PORTD, _PORTD, _PORTD, _PORTD, _PORTD, _PORTD, _PORTD, _PORTD,
  _PORTF, _PORTF,                 _PORTF, _PORTF, _PORTF, _PORTF
};

uint8_t *const  row_pin[NROW] = {
                                   _PINB,                  _PINB,
                                                   _PINC,  _PINC,
   _PIND,  _PIND,  _PIND,  _PIND,  _PIND,  _PIND,  _PIND,  _PIND,
   _PINF,  _PINF,                  _PINF,  _PINF,  _PINF,  _PINF
};

const uint8_t   row_bit[NROW] = {
                                   _BIT4,                  _BIT7,
                                                   _BIT6,  _BIT7,
   _BIT0,  _BIT1,  _BIT2,  _BIT3,  _BIT4,  _BIT5,  _BIT6,  _BIT7,
   _BIT0,  _BIT1,                  _BIT4,  _BIT5,  _BIT6,  _BIT7
};

/* Specifies the ports and pin numbers for the columns */
const uint8_t col_bit[NCOL] = {  0x00,   0x02,   0x04,   0x06,   0x08,   0x0A,   0x0C,   0x0E};

inline void pull_column(uint8_t col) {
  PORTB &= 0b11110001;
  PORTB |= col_bit[col];
  _delay_us(SETTLE_TIME_US);
}

inline void release_column(uint8_t col) {
}

inline uint8_t probe_row(uint8_t row) {
  return ( *row_pin[row] & row_bit[row] ) != 0;
}

inline void update_leds(uint8_t keyboard_leds) {
  PORTB = (PORTB & 0b10011111) | ((~keyboard_leds << 4) & 0b01100000);
}


void keyboard_init() {
  /* 16MHz operation */
  CPU_PRESCALE(0);
  /* Disable JTAG */
  MCUCR |= 0x80; MCUCR |= 0x80;
  /* The column pins are set as outputs. The columns are driven
     through a 3-8 decoder via pins B1,2,3. */
  DDRB  |=  0b00001110;
  PORTB &= ~0b00001110;
  /* The row pins are set as input (with external pull-down
     resistors) */
  for(uint8_t row = 0; row < NROW; row++) {
    *row_ddr[row]  &= ~row_bit[row];
    *row_port[row] &= ~row_bit[row];
  }
  /* LEDs are on pins B6,7. They are set as outputs. */
  DDRB  |= 0b01100000;
  PORTB |= 0b01100000;
  bounce_timer_setup();
}

void bounce_timer_setup(void) {
  TCCR0A |=      // Timer control register 0A
    (1<<WGM01);  // Set CTC, clear timer on compare
  TCCR0B |=      // Timer control register 0A
    (1<<CS00) |  // Prescaler 1024, frequency 15.6kHz (Combined with next line)
    (1<<CS02);   // Prescaler 256, frequency 62.5kHz (This line alone)
  OCR0A = 16;    // Output compare register 0A
}

inline void bounce_timer_enable(void) {
  TIMSK0 |=      // Timer interrupt mask register 0
    (1<<OCIE0A); // Enable timer interrupt on compare match with OCR0A
}

inline void bounce_timer_disable(void) {
  TIMSK0 &=       // Timer interrupt mask register 0
    ~(1<<OCIE0A); // Disable timer interrupt on compare match with OCR0A
}

void toggle_leds(void) {
  PORTB = (PORTB & 0b10011111) | (~PORTB & 0b01100000);
}
