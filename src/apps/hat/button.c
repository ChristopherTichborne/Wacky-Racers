/*  File:   button.c
    Author: Dan Bowles & Cam Maslin
    Date:   5 May 2021
    Descr:  Button module for wacky-racers hat
*/

#include "button.h"


void button_setup ()
{
    pio_config_set (SELECT_1, PIO_PULLUP);
    pio_config_set (SELECT_2, PIO_PULLUP);
    pio_config_set (RF_CHANNEL_1, PIO_PULLUP);
    pio_config_set (RF_CHANNEL_2, PIO_PULLUP);
    pio_config_set (JOY_BUT, PIO_PULLUP);
    pio_config_set (BUTTON_PIO, PIO_PULLUP);
}


bool read_button (pio_t pin)
{
    /* returns inverted pin read since all buttons are pullups*/
    return !pio_input_get(pin);
}