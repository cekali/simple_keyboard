/* Filco Majestouch Tenkeyless replacement controller board parameters. */

#ifndef hid_liber_h__
#define hid_liber_h__

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../lib/avr_extra.h"
#include "../lib/keycode.h"

#define STR_PRODUCT L"Filco Majestouch Tenkeyless"

#define SCAN_INTERRUPT_FUNCTION TIMER0_COMPA_vect
#define SETTLE_TIME_US 1

#define NROW  18
#define NCOL   8
#define NKEY 144

void pull_column(uint8_t col);
void release_columns(void);
uint8_t probe_row(uint8_t row);
void update_leds(uint8_t keyboard_leds);
void keyboard_init(void);
void poll_timer_setup(void);
void poll_timer_enable(void);
void poll_timer_disable(void);
void toggle_leds(void);

/* Convert physical keyboard layout to matrix array.
 * This is a macro to define keymap easily in keyboard layout form.
 *
 * All ANSI ISO JIS Layouts are on the same PCB
 * ,---.   ,---------------. ,---------------. ,---------------. ,-----------.
 * |Esc|   |F1 |F2 |F3 |F4 | |F5 |F6 |F7 |F8 | |F9 |F10|F11|F12| |PrS|ScL|Pau|
 * `---'   `---------------' `---------------' `---------------' `-----------'
 * ,-----------------------------------------------------------. ,-----------.
 * |~  |  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =| Backsp| |Ins|Hom|PgU|
 * |-----------------------------------------------------------| |-----------|
 * |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P|  [|  ]|    \| |Del|End|PgD|
 * |-----------------------------------------------------------| `-----------'
 * |FN1   |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '|  Return|              
 * |-----------------------------------------------------------|     ,---.    
 * |Shft|ISO|  Z|  X|  C|  V|  B|  N|  M|  ,|  .|  /|     Shift|     |Up |    
 * |-----------------------------------------------------------| ,-----------.
 * |Ctl|Gui|Alt|          Space                |Alt|Gui|App|Ctl| |Lef|Dow|Rig|
 * `-----------------------------------------------------------' `-----------'
 */
#define KEYMAP( \
    KG1, KH7, KJ7, KJ6, KJ1, KO5, KL1, KA6, KA7, KD7, KD5, KD1, KD2,      KB5, KB3, KO3,  \
    KG7, KG5, KH5, KJ5, KI5, KI7, KK7, KK5, KL5, KA5, KC5, KC7, KL7, KD6, KQ7, KN7, KM7,  \
    KG6, KG3, KH3, KJ3, KI3, KI6, KK6, KK3, KL3, KA3, KC3, KC6, KL6, KD4, KP7, KN5, KM5,  \
    KH6, KG4, KH4, KJ4, KI4, KI1, KK1, KK4, KL4, KA4, KC4, KC1, KD0,                      \
    KF6, KH1, KG0, KH0, KJ0, KI0, KI2, KK2, KK0, KL0, KA0, KC2, KF4,           KN1,       \
    KO7, KE6, KB1,           KP1,                     KB2, KR4, KA2, KO0, KN2, KP2, KQ2 ) \
/*A         B         C         D         E         F         G         H         I    
  J         K         L         M         N         O         P         Q         R */    \
{                                                                                         \
  KC_##KA0, KC_NO,    KC_NO,    KC_##KD0, KC_NO,    KC_NO,    KC_##KG0, KC_##KH0, KC_##KI0,    /* 0 */ \
  KC_##KJ0, KC_##KK0, KC_##KL0, KC_NO,    KC_NO,    KC_##KO0, KC_NO,    KC_NO,    KC_NO,               \
  KC_NO,    KC_##KB1, KC_##KC1, KC_##KD1, KC_NO,    KC_NO,    KC_##KG1, KC_##KH1, KC_##KI1,    /* 1 */ \
  KC_##KJ1, KC_##KK1, KC_##KL1, KC_NO,    KC_##KN1, KC_NO,    KC_##KP1, KC_NO,    KC_NO,               \
  KC_##KA2, KC_##KB2, KC_##KC2, KC_##KD2, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##KI2,    /* 2 */ \
  KC_NO,    KC_##KK2, KC_NO,    KC_NO,    KC_##KN2, KC_NO,    KC_##KP2, KC_##KQ2, KC_NO,               \
  KC_##KA3, KC_##KB3, KC_##KC3, KC_NO,    KC_NO,    KC_NO,    KC_##KG3, KC_##KH3, KC_##KI3,    /* 3 */ \
  KC_##KJ3, KC_##KK3, KC_##KL3, KC_NO,    KC_NO,    KC_##KO3, KC_NO,    KC_NO,    KC_NO,               \
  KC_##KA4, KC_NO,    KC_##KC4, KC_##KD4, KC_NO,    KC_##KF4, KC_##KG4, KC_##KH4, KC_##KI4,    /* 4 */ \
  KC_##KJ4, KC_##KK4, KC_##KL4, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##KR4,            \
  KC_##KA5, KC_##KB5, KC_##KC5, KC_##KD5, KC_NO,    KC_NO,    KC_##KG5, KC_##KH5, KC_##KI5,    /* 5 */ \
  KC_##KJ5, KC_##KK5, KC_##KL5, KC_##KM5, KC_##KN5, KC_##KO5, KC_NO,    KC_NO,    KC_NO,               \
  KC_##KA6, KC_NO,    KC_##KC6, KC_##KD6, KC_##KE6, KC_##KF6, KC_##KG6, KC_##KH6, KC_##KI6,    /* 6 */ \
  KC_##KJ6, KC_##KK6, KC_##KL6, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,               \
  KC_##KA7, KC_NO,    KC_##KC7, KC_##KD7, KC_NO,    KC_NO,    KC_##KG7, KC_##KH7, KC_##KI7,    /* 7 */ \
  KC_##KJ7, KC_##KK7, KC_##KL7, KC_##KM7, KC_##KN7, KC_##KO7, KC_##KP7, KC_##KQ7, KC_NO                \
}

#define ANSI_ISO_JIS \
  KEYMAP( ESC,         F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,  F10,  F11,  F12,  PSCR, SLCK, PAUS, \
          GRV,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0, MINS,  EQL, BSPC,   INS, HOME, PGUP, \
          TAB,    Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P, LBRC, RBRC, BSLS,   DEL,  END, PGDN, \
         CAPS,    A,    S,    D,    F,    G,    H,    J,    K,    L, SCLN, QUOT,        ENT,                    \
         LSFT, NUBS,    Z,    X,    C,    V,    B,    N,    M, COMM,  DOT, SLSH,       RSFT,          UP,       \
         LCTL, LGUI, LALT,              SPC,                   RALT, RGUI,  APP, RCTL,        LEFT, DOWN, RGHT)
                                                         
#define DVORAK \
  KEYMAP( ESC,         F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,  F10,  F11,  F12,  PSCR, SLCK, PAUS, \
          GRV,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0, LBRC, RBRC, BSPC,   INS, HOME, PGUP, \
          TAB, QUOT, COMM,  DOT,    P,    Y,    F,    G,    C,    R,    L, SLSH,  EQL, BSLS,   DEL,  END, PGDN, \
         CAPS,    A,    O,    E,    U,    I,    D,    H,    T,    N,    S, MINS,        ENT,                    \
         LSFT, NUBS, SCLN,    Q,    J,    K,    X,    B,    M,    W,    V,    Z,       RSFT,          UP,       \
         LCTL, LGUI, LALT,              SPC,                   RALT, RGUI,  APP, RCTL,        LEFT, DOWN, RGHT)


#endif
