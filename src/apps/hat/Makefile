PERIPHERALS = pit
LED_DIR = $(DRIVER_DIR)/led
DRIVERS = pacer usb_serial mpu
TARGET = hat.bin

SRC = hat.c
OPT = -O2

ifneq ($(BOARD), hat)
$(error BOARD should be set to hat!)
endif

include ../../boards/board.mk
