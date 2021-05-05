/** @file   target.h
    @author M. P. Hayes, UCECE
    @date   12 February 2018
    @brief
*/
#ifndef TARGET_H
#define TARGET_H

#include "mat91lib.h"

/* This is for the carhat (chart) board configured as a hat!  */

/* System clocks  */
#define F_XTAL 12.00e6
#define MCU_PLL_MUL 16
#define MCU_PLL_DIV 1

#define MCU_USB_DIV 2
/* 192 MHz  */
#define F_PLL (F_XTAL / MCU_PLL_DIV * MCU_PLL_MUL)
/* 96 MHz  */
#define F_CPU (F_PLL / 2)

/* TWI  */
#define TWI_TIMEOUT_US_DEFAULT 10000
#define MPU_ADDRESS 0x68

/* USB  */
//#define USB_VBUS_PIO PA5_PIO
#define USB_CURRENT_MA 500

/* ADC  */
#define ADC_BATTERY PA17_PIO
#define ADC_JOYSTICK_X PB2_PIO
#define ADC_JOYSTICK_Y PB1_PIO

/* IMU  */
#define IMU_INT_PIO PA0_PIO

/* LEDs  */
#define LED_RED PA20_PIO
#define LED_GREEN PA23_PIO
#define LED_STRIP PA7_PIO

/* General  */
#define APPENDAGE_PIO PA1_PIO
#define SERVO_PWM_PIO PA2_PIO
#define PIEZO_P1 PA19_PIO
#define PIEZO_P2 PB0_PIO
#define EN_RADIO PA9_PIO
#define EN_3V3_REG PA8_PIO


/* Buttons and dipswitches  */
#define BUTTON_PIO PA4_PIO
#define JOY_BUT PA6_PIO
#define RF_CHANNEL_1 PA24_PIO
#define RF_CHANNEL_2 PA25_PIO
#define SELECT_1 PA28_PIO
#define SELECT_2 PA29_PIO

/* Radio  */
#define RADIO_CS_PIO PA11_PIO
#define RADIO_CE_PIO PA10_PIO
#define RADIO_IRQ_PIO PA26_PIO


#endif /* TARGET_H  */
