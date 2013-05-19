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

#include "usb_keyboard.h"
#include "keycode.h"
#include "avr_extra.h"
#include "hid_liber.h"

#define MODIFIER_KEY 1
typedef struct _key {
  uint8_t value;
  uint8_t type;
} KEY;


uint8_t key_status[NKEY];
uint16_t __layout[NKEY] = ANSI_ISO_JIS;
KEY * layout = (KEY *)__layout;

/* queue     contains the keys that are sent in the HID packet 
   mod_keys  is the bit pattern corresponding to pressed modifier keys */
uint8_t queue[7] = {255,255,255,255,255,255,255};
uint8_t mod_keys = 0;

/* void pull_column(uint8_t row); */
/* void update_leds(uint8_t keyboard_leds); */
void init(void);
void send(void);
void key_press(uint8_t key);
void key_release(uint8_t key);

int main(void) {
  init();
  
  uint8_t previous[NKEY];
  for(uint8_t key = 0; key < NKEY; key++)
    previous[key] = 0;

  while(true) {
    for(uint8_t col = 0, key = 0; col < NCOL; col++) {
      pull_column(col);
      for(uint8_t row = 0; row < NROW; row++, key++) {
        uint8_t this = probe_row(row);
        // Detect rising and falling edges on key status
        if(this && !previous[key]){
          key_press(key);
          _delay_ms(5);
        }
        else if(previous[key] && !this) {
          key_release(key);
          _delay_ms(5);
        }
        previous[key] = this;
      }
    }
    update_leds(keyboard_leds);
  }
}

/* */
void send(void) {
  for(uint8_t i=0; i<6; i++)
    keyboard_keys[i] = queue[i]<255? layout[queue[i]].value: 0;
  keyboard_modifier_keys = mod_keys;
  usb_keyboard_send();
}

/* */
void key_press(uint8_t key) {
  uint8_t i;
  key_status[key] = true;
  if(layout[key].type == MODIFIER_KEY)
    mod_keys |= layout[key].value;
  else {
    for(i = 5; i > 0; i--) queue[i] = queue[i-1];
    queue[0] = key;
  }
  send();
}

/* */
void key_release(uint8_t key) {
  uint8_t i;
  key_status[key] = false;
  if(layout[key].type == MODIFIER_KEY)
    mod_keys &= ~layout[key].value;
  else {
    for(i = 0; i < 6; i++) 
      if(queue[i]==key)
        break;
    for(i = i; i < 6; i++)
      queue[i] = queue[i+1];
  }
  send();
}

/* Call initialization functions */
void init(void) {
  usb_init();
  while(!usb_configured());
  init_keyboard();
  mod_keys = 0;
  for(uint8_t key = 0; key < NKEY; key++)
    key_status[key] = false;
  sei();  // Enable interrupts
}
