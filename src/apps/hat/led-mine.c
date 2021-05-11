/*  File:   led.c
    Author: Dan Bowles & Cam Maslin
    Date:   5 May 2021
    Descr:  LED module for surface mount LEDs
*/


#include "led-mine.h"


void led_setup ()
{
    /* set as output highs since they are tied to 3V3 */
    pio_config_set (LED_GREEN, PIO_OUTPUT_HIGH);
    pio_config_set (LED_RED, PIO_OUTPUT_HIGH);
}


void led_on (pio_t led)
{
    /* set led to low to turn on as they are tied to 3V3 */
    pio_output_low(led);
}


void led_off (pio_t led)
{
    /* set led high to turn off as they are tied to 3V3 */
    pio_output_high(led);
}