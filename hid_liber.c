/* Tenkeyless replacement controller board parameters. */

#include "hid_liber.h"

struct { uint8_t *const pin; const uint8_t bit; } rows[] = 
  {
    {_PINB, _BIT4}, {_PINB, _BIT7}, {_PINC, _BIT6}, {_PINC, _BIT7}, {_PIND, _BIT0}, {_PIND, _BIT1},
    {_PIND, _BIT2}, {_PIND, _BIT3}, {_PIND, _BIT4}, {_PIND, _BIT5}, {_PIND, _BIT6}, {_PIND, _BIT7},
    {_PINF, _BIT0}, {_PINF, _BIT1}, {_PINF, _BIT4}, {_PINF, _BIT5}, {_PINF, _BIT6}, {_PINF, _BIT7},
  };

/* Specifies the ports and pin numbers for the columns */
const uint8_t col_bits[NCOL] =  
  {
    0b00000000, 0b00000010, 0b00000100, 0b00000110, 0b00001000, 0b00001010, 0b00001100, 0b00001110
  };

/* Activate one of the columns in the matrix. And wait for the
   electrical state to settle. */
inline void pull_column(uint8_t c) {
  PORTB &= 0b11110001;
  PORTB |= col_bits[c];
  _delay_us(SETTLE_TIME_US);
}

/* De-activate column. */
inline void release_column(uint8_t c) {
}

/* Probe one of the rows to look for pressed keys. Returns 0x01 if
   there is a key pressed in the row (while the correct column is
   activated) and 0x00 otherwise. */
inline uint8_t probe_row(uint8_t r) {
  return ( *rows[r].pin & rows[r].bit ) != 0;
}

/* Update keyboard LEDs. Which LEDs to turn on is indicated by bits in
   keyboard_leds.
   bit 2 = scroll lock, bit 1 = caps lock, bit 0 = num lock.
   LEDs are active low on pin B6, B7, luckily in the correct
   order. The contents of keyboard_leds only needs to be shifted into
   the correct position and masked out. */
inline void update_leds(uint8_t keyboard_leds) {
  PORTB = (PORTB & 0b10011111) | ((~keyboard_leds << 4) & 0b01100000);
}


void keyboard_init() {
  /* 16MHz operation */
  CPU_PRESCALE(0);
  /* Disable JTAG */
  MCUCR |= 0x80; MCUCR |= 0x80;
  /* The column pins are set as outputs. The columns are driven
     through a 3-8 decoder via pins B1,B2,B3. */
  DDRB |=  0b00001110; PORTB &= ~0b00001110;
  /* The row pins are set as inputs (with external pull-down
     resistors) */
  DDRB &= ~0b10010000; PORTB &= ~0b10010000;
  DDRC &= ~0b11000000; PORTC &= ~0b11000000;
  DDRD &= ~0b11111111; PORTD &= ~0b11111111;
  DDRF &= ~0b11001111; PORTF &= ~0b11001111;
  /* LEDs are on pins B6,7. They are set as outputs. */
  DDRB  |= 0b01100000;
  PORTB |= 0b01100000;
  poll_timer_setup();
}

/* Polling of the keyboard matrix is done on regular intervals. This
   is acheived by an interrupt on timer 0. The timer is scaled down to
   15600Hz and the match is done when it reaches 16. This gives a
   polling frequency of 975Hz. If bouncing still occurs the value of
   OCR0A can be increased to decrease the polling frequency. */
void poll_timer_setup(void) {
  TCCR0A |=      // Timer control register 0A
    (1<<WGM01);  // Set CTC, clear timer on compare
  TCCR0B |=      // Timer control register 0A
    (1<<CS00) |  // Prescaler 1024, frequency 15.6kHz (Combined with next line)
    (1<<CS02);   // Prescaler 256, frequency 62.5kHz (This line alone)
  OCR0A = 16;    // Output compare register 0A
}

/* Turn on polling. */
inline void poll_timer_enable(void) {
  TIMSK0 |=      // Timer interrupt mask register 0
    (1<<OCIE0A); // Enable timer interrupt on compare match with OCR0A
}

/* Turn off polling. */
inline void poll_timer_disable(void) {
  TIMSK0 &=       // Timer interrupt mask register 0
    ~(1<<OCIE0A); // Disable timer interrupt on compare match with OCR0A
}

/* Blink the LEDs, mostly for debugging purposes. */
void toggle_leds(void) {
  PORTB = (PORTB & 0b10011111) | (~PORTB & 0b01100000);
}
