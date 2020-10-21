/* File:   rf_tester.c
   Author: A. Renaud
   Date:   Oct 2020
   Descr: 
*/
#include <stdio.h>
#include "usb_serial.h"
#include "nrf24.h"
#include "pio.h"
#include "pacer.h"
#include "delay.h"
#include "button.h"

#define TX_LED PA21_PIO
#define RX_LED PA31_PIO
#define TX_BTN PA6_PIO
#define RX_BTN PA18_PIO

static usb_serial_cfg_t usb_serial_cfg = {
    .read_timeout_us = 1,
    .write_timeout_us = 1,
};

static void panic(void)
{
    while (1) {
        pio_output_toggle(TX_LED);
        delay_ms(200);
        pio_output_toggle(RX_LED);
        delay_ms(200);
    }
}

int main (void)
{
    uint8_t tx_count = 0;
    spi_cfg_t nrf_spi = {
        .channel = 0,
        .clock_speed_kHz = 1000,
        .cs = RADIO_CS_PIO,
        .mode = SPI_MODE_0,
        .cs_mode = SPI_CS_MODE_FRAME,
        .bits = 8,
    };
    nrf24_t *nrf;
    spi_t spi;
    bool listening = false;

    /* Configure LED PIO as output.  */
    pio_config_set(TX_LED, PIO_OUTPUT_LOW);
    pio_config_set(RX_LED, PIO_OUTPUT_LOW);
    pacer_init(100);

    // Create non-blocking tty device for USB CDC connection.
    usb_serial_init (&usb_serial_cfg, "/dev/usb_tty");
    
    freopen ("/dev/usb_tty", "a", stdout);
    freopen ("/dev/usb_tty", "r", stdin);    

    // Start up the radio subsystem
#ifdef RADIO_PWR_EN
    pio_config_set(RADIO_PWR_EN, PIO_OUTPUT_HIGH);
#endif

    spi = spi_init(&nrf_spi);
    nrf = nrf24_create(spi, RADIO_CE_PIO, RADIO_IRQ_PIO);
    if (!nrf)
        panic();
    if (!nrf24_begin(nrf, 4, 0xabcdef0123456789, 32))
        panic();
    button_cfg_t tx_cfg = {.pio = TX_BTN};
    button_cfg_t rx_cfg = {.pio = RX_BTN};

    button_t tx_btn = button_init(&tx_cfg);   
    button_t rx_btn = button_init(&rx_cfg);   
    if (!tx_btn)
        panic();
    if (!rx_btn)
        panic();

    // Can't use poll, because of debounce. Button active low
    if (!pio_input_get(RX_BTN)) {
        if (!nrf24_listen(nrf))
            panic();
        listening = true;
        pio_output_set(RX_LED, 1);
    }

#if 0 // LED/switch tester
    while (1) {
        pio_output_set(TX_LED, pio_input_get(TX_BTN));
        pio_output_set(RX_LED, pio_input_get(RX_BTN));
        printf("poll rx: %d tx: %d\n", button_poll(rx_btn), button_poll(tx_btn));
        fflush(stdout);
    }
#endif

    while (1)
    {
        char buffer[32];
        
        pacer_wait();

        if (button_poll(tx_btn) == BUTTON_STATE_DOWN) {
            if (listening)
                mcu_reset();
            pio_output_set(TX_LED, 1);
            sprintf (buffer, "Hello world %d", tx_count++);

            if (! nrf24_write(nrf, buffer, sizeof (buffer))) {
                // No bytes were written
                printf("TX: FAILURE\n");
            } else {
                printf("TX: %s\n", buffer);
            }
        } else {
            pio_output_set(TX_LED, 0);
        }

        if (listening) {
            if (nrf24_read(nrf, buffer, sizeof(buffer))) {
                pio_output_set(RX_LED, 0);
                printf("RX: %s\n", buffer);
                pio_output_set(LED2_PIO, 0);
            } else {
                pio_output_set(RX_LED, 1);
            }
        }
        if (button_poll(rx_btn) == BUTTON_STATE_DOWN) {
            if (!listening)
                mcu_reset();
        }

#if 0
        pio_output_set(LED2_PIO, 1);
        if (nrf24_read(nrf, buffer, sizeof(buffer))) {
            printf("RX: %s\n", buffer);
            pio_output_set(LED2_PIO, 0);
        } else {
            printf("RX: nothing\n");
        }
#endif
        fflush(stdout);
    }
}

