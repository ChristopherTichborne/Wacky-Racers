/* File:   our-program.c
   Author: 
   Date:   05/05/21
   Descr:  
*/
#include <pio.h>
#include <fcntl.h>

#include "nrf24.h"
#include "stdio.h"
#include "delay.h"
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"






static void panic(void)
{
    while (1) {
        pio_output_toggle(LED_RED);
        pio_output_toggle(LED_GREEN);
        delay_ms(400);
    }
}

static usb_serial_cfg_t usb_serial_cfg =
{
    .read_timeout_us = 1,
    .write_timeout_us = 1,
};

static twi_cfg_t mpu_twi_cfg =
{
    .channel = TWI_CHANNEL_1,
    .period = TWI_PERIOD_DIVISOR(100000), // 100 kHz
    .slave_addr = 0 // only needed in slave mode.
};


/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 200};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 20};



int main (void)
{
    uint8_t count = 0;
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

    /* Configure LED PIO as output.  */
    pio_config_set(LED_GREEN, PIO_OUTPUT_HIGH);
    pio_config_set(LED_RED, PIO_OUTPUT_LOW);
    pacer_init(10);

#ifdef RADIO_PWR_EN
    pio_config_set(RADIO_PWR_EN, PIO_OUTPUT_HIGH);
#endif

    spi = spi_init(&nrf_spi);
    nrf = nrf24_create(spi, RADIO_CE_PIO, RADIO_IRQ_PIO);
    if (!nrf)
        panic();

    // initialize the NRF24 radio with its unique 5 byte address
    if (!nrf24_begin(nrf, 4, 0x0123456789, 32))
        panic();

    uint8_t flash_ticks;
    flash_ticks = 0;
    
    int16_t accel[3];

    led_setup();
    button_setup();

    mcu_jtag_disable();

    usb_serial_init (&usb_serial_cfg, "/dev/usb_tty");
    freopen ("/dev/usb_tty", "a", stdout);
    freopen ("/dev/usb_tty", "r", stdin);

    // Initialise the TWI (I2C) bus for the MPU
    twi_t twi_mpu = twi_init(&mpu_twi_cfg);
    // Initialise the MPU9250 IMU
    mpu_t* mpu = mpu9250_create(twi_mpu, MPU_ADDRESS);

    pacer_init (10);
    // use tan o/a to get position

    while (1)
    {
        char buffer[32];

        /* Wait until next clock tick.  */
        pacer_wait ();


        if (mpu) {
            /* read in the accelerometer data */
            if (!mpu9250_is_imu_ready(mpu)) {
                printf("waiting for IMU to be ready...\n");
            } else {
                int16_t accel[3];
                if (mpu9250_read_accel(mpu, accel)) {
                    printf("x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);
                } else {
                    printf("ERROR: failed to read acceleration\n");
                }
            }
        } else {
            printf("ERROR: can't find MPU9250!\n");
        }

        sprintf (buffer, "x: %5d  y: %5d  z: %5d\n", accel[0], accel[1], accel[2]);

        if (! nrf24_write(nrf, buffer, sizeof (buffer)))
            pio_output_set(LED_GREEN, 0);
        else
            pio_output_set(LED_GREEN, 1);


        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;

            /* Toggle LED.  */
            pio_output_toggle (LED_RED);
        }


        printf("%d\n", pio_input_get(SELECT_2));
        // printf("%d %d   %d %d   %d %d\n", pio_input_get(SELECT_1), pio_input_get(SELECT_2), pio_input_get(RF_CHANNEL_1), pio_input_get(RF_CHANNEL_2), pio_input_get(JOY_BUT), pio_input_get(BUTTON_PIO));
        fflush(stdout);
    }
}