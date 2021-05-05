/*  File:   hat.c
    Author: Dan Bowles & Cam Maslin
    Date:   5 May 2021
    Descr:  Main code for Wacky Racers hat board 2021
*/

#include <stdio.h>
#include <math.h>
#include <pio.h>
#include <fcntl.h>

#include "pacer.h"

#include "led.h"
#include "button.h"



// Set to the desired polling rate.
#define PACER_RATE 200

/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};



void setup (void)
{
    pacer_init (PACER_RATE);    
    mcu_watchdog_enable();
    button_setup();
    led_setup();
    mcu_jtag_disable();
}



int main (void)
{
    uint8_t flash_ticks;
    flash_ticks = 0;

    setup();

    // TODO: write hat program here...
    while (1);

        pacer_wait ();
        mcu_watchdog_reset ();

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;

            /* Toggle LED.  */
            pio_output_toggle (LED_GREEN);
        }
    }
}
