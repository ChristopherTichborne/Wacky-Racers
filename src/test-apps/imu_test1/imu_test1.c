/* File:   imu_test1.c
   Author: B Mitchell, UCECE
   Date:   15/4/2021
   Descr:  Read from an MP9250 IMU and write its output to the USB serial.
*/
#include <pio.h>
#include <fcntl.h>
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "mpu9250.h"


/* Define how fast ticks occur.  This must be faster than
   TICK_RATE_MIN.  */
enum {LOOP_POLL_RATE = 10};

/* Define LED flash rate in Hz.  */
enum {LED_FLASH_RATE = 2};


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


int
main (void)
{

    uint8_t flash_ticks;
    flash_ticks = 0;


    /* Configure LED PIO as output.  */
    pio_config_set (LED_GREEN, PIO_OUTPUT_LOW);

    mcu_jtag_disable();
    // Create non-blocking tty device for USB CDC connection.
    usb_serial_init (&usb_serial_cfg, "/dev/usb_tty");

    freopen ("/dev/usb_tty", "a", stdout);
    freopen ("/dev/usb_tty", "r", stdin);

    // Initialise the TWI (I2C) bus for the MPU
    twi_t twi_mpu = twi_init(&mpu_twi_cfg);
    // Initialise the MPU9250 IMU
    mpu_t* mpu = mpu9250_create(twi_mpu, MPU_ADDRESS);


    pacer_init (LOOP_POLL_RATE);


    // use tan o/a to get position

    while (1)
    {
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

        fflush(stdout);

        flash_ticks++;
        if (flash_ticks >= LOOP_POLL_RATE / (LED_FLASH_RATE * 2))
        {
            flash_ticks = 0;

            /* Toggle LED.  */
            pio_output_toggle (LED_GREEN);
        }
        
    }
}
