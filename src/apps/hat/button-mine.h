/*  File:   button.h
    Author: Dan Bowles & Cam Maslin
    Date:   5 May 2021
    Descr:  Button module for wacky-racers hat
*/

#ifndef BUTTON-MINE_H
#define BUTTON-MINE_H

#include <pio.h>
#include "target.h"


void button_setup ();

bool read_button (pio_t pin);


#endif