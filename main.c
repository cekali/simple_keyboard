/* USB Keyboard Firmware code for generic Teensy Keyboards
 * Copyright (c) 2012 Fredrik Atmer, Bathroom Epiphanies Inc
 * http://bathroomepiphanies.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* #include <stdlib.h> */
/* #include <avr/io.h> */
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifdef DEBUG
#include "usb_keyboard_debug.h"
#else
#include "usb_keyboard.h"
#endif

#include "print.h"
#include "keycode.h"
#include "avr_extra.h"
#include KEYBOARD_MODEL

#define MODIFIER_KEY 0x01

typedef struct _key {
  uint8_t value;
  uint8_t type;
} KEY;


uint8_t key_status[NKEY];
uint8_t key_bounce[NKEY];
uint16_t _layout[NKEY] = KEYBOARD_LAYOUT;
KEY * layout = (KEY *)_layout;

/* queue     contains the keys that are sent in the HID packet 
   mod_keys  is the bit pattern corresponding to pressed modifier keys */
uint8_t queue[7] = {255,255,255,255,255,255,255};
uint8_t mod_keys = 0;

/* void pull_column(uint8_t row); */
/* void update_leds(uint8_t keyboard_leds); */
void setup_bounce_timer(void);
void init(void);
void send(void);
void key_press(uint8_t key);
void key_release(uint8_t key);

int count = 0;
ISR(INTERRUPT_FUNCTION) {
  bounce_timer_disable();

  for(uint8_t col = 0, key = 0; col < NCOL; col++) {
    pull_column(col);
    for(uint8_t row = 0; row < NROW; row++, key++) {
      key_status[key] = (key_status[key] & 0xFE) | probe_row(row);
      if(key_status[key] == 0x7F)
        key_press(key);
      else if(key_status[key] == 0x80)
        key_release(key);
      key_status[key] = (key_status[key]<<1) | (key_status[key]&0x01);
    }
  }
  
#ifdef DEBUG
  count ++;
  if(count > 100) {
    count = 0;
    for(uint8_t i = 0; i < 7; i++)
      phex(queue[i]);
    print("\n");
    
    for(uint8_t key = 0; key < NKEY; key++)
      phex(key_bounce[key]);
    print("\n");
  }
#endif  

  if(mod_keys == (uint8_t)(KEY_LEFT_SHIFT | KEY_RIGHT_SHIFT))
    jump_bootloader();

  update_leds(keyboard_leds);

  bounce_timer_enable();
}

int main(void) {
  init();
  bounce_timer_enable();
  while(true) ;
}

/* */
void send(void) {
  for(uint8_t i = 0; i < 6; i++)
    keyboard_keys[i] = queue[i]<255? layout[queue[i]].value: 0;
  keyboard_modifier_keys = mod_keys;
  usb_keyboard_send();
}

/* */
void key_press(uint8_t key) {
  if(layout[key].type == MODIFIER_KEY)
    mod_keys |= layout[key].value;
  else {
    for(uint8_t i = 5; i > 0; i--) queue[i] = queue[i-1];
    queue[0] = key;
  }
  send();
}

/* */
void key_release(uint8_t key) {
  uint8_t i;
  if(layout[key].type == MODIFIER_KEY)
    mod_keys &= ~layout[key].value;
  else {
    for(i = 0; i < 6; i++) 
      if(queue[i]==key)
        break;
    for( ; i < 6; i++)
      queue[i] = queue[i+1];
  }
  send();
}

/* Call initialization functions */
void init(void) {
  usb_init();
  while(!usb_configured());
  keyboard_init();
  mod_keys = 0;
  for(uint8_t key = 0; key < NKEY; key++)
    key_status[key] = false;
  for(uint8_t key = 0; key < NKEY; key++)
    key_bounce[key] = 0x00;
  sei();  // Enable interrupts
}
