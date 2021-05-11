/*  File:   led.h
    Author: Dan Bowles & Cam Maslin
    Date:   5 May 2021
    Descr:  LED module for surface mount LEDs
*/

#ifndef LED-MINE_H
#define LED-MINE_H

#include <pio.h>
#include "target.h"


void
led_setup ();

void
led_on (pio_t led);

void
led_off (pio_t led);


#endif