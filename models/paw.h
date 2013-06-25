/* Filco Majestouch replacement controller board parameters. */

#ifndef paw_h__
#define paw_h__

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../lib/avr_extra.h"
#include "../lib/keycode.h"

#define STR_PRODUCT L"Filco Majestouch"

#define SCAN_INTERRUPT_FUNCTION TIMER0_COMPA_vect
#define SETTLE_TIME_US 1

#define NROW   8
#define NCOL  18
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
    KG6, KH4, KI4, KI2, KI6, KP5, KL6, KM2, KM4, KO4, KO5, KO6, KO0,       KN5, KN7, KP7,                      \
    KG4, KG5, KH5, KI5, KJ5, KJ4, KK4, KK5, KL5, KM5, KF5, KF4, KL4, KO2,  KR4, KC4, KE4,  KQ1, KR1, KE1, KE0, \
    KG2, KG7, KH7, KI7, KJ7, KJ2, KK2, KK7, KL7, KM7, KF7, KF2, KL2, KO1,  KQ4, KC5, KE5,  KQ7, KR7, KE7, KC7, \
    KH2, KG3, KH3, KI3, KJ3, KJ6, KK6, KK3, KL3, KM3, KF3, KF6, KO3,                       KQ2, KR2, KE2,      \
    KB2, KH6, KG1, KH1, KI1, KJ1, KJ0, KK0, KK1, KL1, KM1, KF0, KB3,            KC6,       KQ3, KR3, KE3, KC3, \
    KP4, KD2, KN6,           KQ6,                     KN0, KA3, KM0, KP1,  KC0, KQ0, KR0,  KR6,      KE6     ) \
{                                                                                       \
/*      0         1         2         3         4         5         6         7   */    \
/* A */ KC_NO,    KC_NO,    KC_NO,    KC_##KA3, KC_NO,    KC_NO,    KC_NO,    KC_NO,    \
/* B */ KC_NO,    KC_NO,    KC_##KB2, KC_##KB3, KC_NO,    KC_NO,    KC_NO,    KC_NO,    \
/* C */ KC_##KC0, KC_NO,    KC_NO,    KC_##KC3, KC_##KC4, KC_##KC5, KC_##KC6, KC_##KC7, \
/* D */ KC_NO,    KC_NO,    KC_##KD2, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    \
/* E */ KC_##KE0, KC_##KE1, KC_##KE2, KC_##KE3, KC_##KE4, KC_##KE5, KC_##KE6, KC_##KE7, \
/* F */ KC_##KF0, KC_NO,    KC_##KF2, KC_##KF3, KC_##KF4, KC_##KF5, KC_##KF6, KC_##KF7, \
/* G */ KC_NO,    KC_##KG1, KC_##KG2, KC_##KG3, KC_##KG4, KC_##KG5, KC_##KG6, KC_##KG7, \
/* H */ KC_NO,    KC_##KH1, KC_##KH2, KC_##KH3, KC_##KH4, KC_##KH5, KC_##KH6, KC_##KH7, \
/* I */ KC_NO,    KC_##KI1, KC_##KI2, KC_##KI3, KC_##KI4, KC_##KI5, KC_##KI6, KC_##KI7, \
/* J */ KC_##KJ0, KC_##KJ1, KC_##KJ2, KC_##KJ3, KC_##KJ4, KC_##KJ5, KC_##KJ6, KC_##KJ7, \
/* K */ KC_##KK0, KC_##KK1, KC_##KK2, KC_##KK3, KC_##KK4, KC_##KK5, KC_##KK6, KC_##KK7, \
/* L */ KC_NO,    KC_##KL1, KC_##KL2, KC_##KL3, KC_##KL4, KC_##KL5, KC_##KL6, KC_##KL7, \
/* M */ KC_##KM0, KC_##KM1, KC_##KM2, KC_##KM3, KC_##KM4, KC_##KM5, KC_NO,    KC_##KM7, \
/* N */ KC_##KN0, KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_##KN5, KC_##KN6, KC_##KN7, \
/* O */ KC_##KO0, KC_##KO1, KC_##KO2, KC_##KO3, KC_##KO4, KC_##KO5, KC_##KO6, KC_NO,    \
/* P */ KC_NO,    KC_##KP1, KC_NO,    KC_NO,    KC_##KP4, KC_##KP5, KC_NO,    KC_##KP7, \
/* Q */ KC_##KQ0, KC_##KQ1, KC_##KQ2, KC_##KQ3, KC_##KQ4, KC_NO,    KC_##KQ6, KC_##KQ7, \
/* R */ KC_##KR0, KC_##KR1, KC_##KR2, KC_##KR3, KC_##KR4, KC_NO,    KC_##KR6, KC_##KR7, \
}

#define ANSI_ISO_JIS \
  KEYMAP( ESC,         F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,  F10,  F11,  F12,   PSCR, SLCK, PAUS,                           \
          GRV,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0, MINS,  EQL, BSPC,    INS, HOME, PGUP,   NLCK, PSLS, PAST, PMNS, \
          TAB,    Q,    W,    E,    R,    T,    Y,    U,    I,    O,    P, LBRC, RBRC, BSLS,    DEL,  END, PGDN,     P7,   P8,   P9, PPLS, \
         CAPS,    A,    S,    D,    F,    G,    H,    J,    K,    L, SCLN, QUOT,        ENT,                         P4,   P5,   P6,       \
         LSFT, NUBS,    Z,    X,    C,    V,    B,    N,    M, COMM,  DOT, SLSH,       RSFT,           UP,           P1,   P2,   P3, PENT, \
         LCTL, LGUI, LALT,              SPC,                   RALT, RGUI,  APP, RCTL,         LEFT, DOWN, RGHT,     P0,       PDOT        )

#define DVORAK \
  KEYMAP( ESC,         F1,   F2,   F3,   F4,   F5,   F6,   F7,   F8,   F9,  F10,  F11,  F12,   PSCR, SLCK, PAUS,                           \
          GRV,    1,    2,    3,    4,    5,    6,    7,    8,    9,    0, LBRC, RBRC, BSPC,    INS, HOME, PGUP,   NLCK, PSLS, PAST, PMNS, \
          TAB, QUOT, COMM,  DOT,    P,    Y,    F,    G,    C,    R,    L, SLSH,  EQL, BSLS,    DEL,  END, PGDN,     P7,   P8,   P9, PPLS, \
         CAPS,    A,    O,    E,    U,    I,    D,    H,    T,    N,    S, MINS,        ENT,                         P4,   P5,   P6,       \
         LSFT, NUBS, SCLN,    Q,    J,    K,    X,    B,    M,    W,    V,    Z,       RSFT,           UP,           P1,   P2,   P3, PENT, \
         LCTL, LGUI, LALT,              SPC,                   RALT, RGUI,  APP, RCTL,         LEFT, DOWN, RGHT)     P0,       PDOT        )
#endif
