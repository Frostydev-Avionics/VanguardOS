/****************************************************************************
 * boards/arm/stm32h7/vanguard-fc/src/vanguard-stm32h7.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#pragma once

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>

#include "stm32_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* procfs File System *******************************************************/

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define STM32_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define STM32_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/* Board LEDs
 *
 * LED RED    PG2
 * LED GREEN  PG3
 * LED BLUE   PB2
 */

#define GPIO_LD_RED    (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_CLEAR | GPIO_PORTG | GPIO_PIN2)
#define GPIO_LD_GREEN  (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_CLEAR | GPIO_PORTG | GPIO_PIN3)
#define GPIO_LD_BLUE   (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                        GPIO_OUTPUT_CLEAR | GPIO_PORTB | GPIO_PIN2)

/* GPIO *********************************************************************/

#define BOARD_NGPIOIN  4  /* Amount of GPIO Input Pins */
#define BOARD_NGPIOOUT 8  /* Amount of GPIO Output Pins */
#define BOARD_NGPIOINT 1  /* Amount of GPIO Input w/ Interrupt Pins */

#define GPIO_IN1   (GPIO_INPUT | GPIO_FLOAT | GPIO_PORTE | GPIO_PIN7)
#define GPIO_IN2   (GPIO_INPUT | GPIO_FLOAT | GPIO_PORTE | GPIO_PIN12)
#define GPIO_IN3   (GPIO_INPUT | GPIO_FLOAT | GPIO_PORTE | GPIO_PIN14)
#define GPIO_IN4   (GPIO_INPUT | GPIO_FLOAT | GPIO_PORTE | GPIO_PIN15)

#define GPIO_OUT1  (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                    GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN4)
#define GPIO_OUT2  (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                    GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN5)
#define GPIO_OUT3  (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                    GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN6)
#define GPIO_OUT4  (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                    GPIO_OUTPUT_SET | GPIO_PORTA | GPIO_PIN5)
#define GPIO_OUT5  (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                    GPIO_OUTPUT_SET | GPIO_PORTF | GPIO_PIN3)
#define GPIO_INT1  (GPIO_INPUT | GPIO_FLOAT | GPIO_PORTE | GPIO_PIN3)

#define GPIO_MMCSD_CS (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_50MHz | \
                       GPIO_OUTPUT_SET | GPIO_PORTA | GPIO_PIN4)

/* PWM **********************************************************************/

#if defined(CONFIG_STM32H7_TIM1_PWM)
#  define VANGUARD_PWMTIMER 1
#else
#  define VANGUARD_PWMTIMER 3
#endif

/* USB OTG FS ***************************************************************
 *
 * The Vanguard FC uses only the OTG FS data and ID pins. There is no
 * external VBUS power switch or overcurrent GPIO on this board -- VBUS
 * is sensed internally by the STM32H743 OTG FS peripheral.
 *
 *   PA10  OTG_FS_ID
 *   PA11  OTG_FS_DM
 *   PA12  OTG_FS_DP
 *
 * These are configured in board.h as GPIO_OTGFS_ID/DM/DP and applied
 * in stm32_usbinitialize().
 */

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_bringup
 *
 * Description:
 *   Perform architecture-specific initialization. Called from
 *   board_late_initialize() when CONFIG_BOARD_LATE_INITIALIZE=y, or
 *   from the NSH library when CONFIG_NSH_ARCHINIT=y.
 *
 ****************************************************************************/

int stm32_bringup(void);

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called early in initialization to configure USB OTG FS GPIO pins.
 *   Actual USB class drivers are initialized later in stm32_bringup().
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_OTGFS
void weak_function stm32_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host
 *   functionality. Starts a thread that monitors for device
 *   connection/disconnection events.
 *
 ****************************************************************************/

#if defined(CONFIG_STM32H7_OTGFS) && defined(CONFIG_USBHOST)
int stm32_usbhost_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_spidev_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the board.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_SPI
void stm32_spidev_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_mmcsd_spi_initialize
 *
 * Description:
 *   Initialize SPI-based MMC/SD card support
 *
 ****************************************************************************/

#ifdef CONFIG_MMCSD_SPI
int stm32_mmcsd_spi_initialize(int minor);
#endif

/****************************************************************************
 * Name: stm32_adc_setup
 *
 * Description:
 *   Initialize ADC and register the ADC driver.
 *
 ****************************************************************************/

#ifdef CONFIG_ADC
int stm32_adc_setup(void);
#endif

/****************************************************************************
 * Name: stm32_sdio_initialize
 *
 * Description:
 *   Initialize SDIO-based MMC/SD card support.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32H7_SDMMC
int stm32_sdio_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_gpio_initialize
 *
 * Description:
 *   Initialize GPIO driver.
 *
 ****************************************************************************/

#if defined(CONFIG_DEV_GPIO) && !defined(CONFIG_GPIO_LOWER_HALF)
int stm32_gpio_initialize(void);
#endif

/****************************************************************************
 * Name: stm32_pwm_setup
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

#ifdef CONFIG_PWM
int stm32_pwm_setup(void);
#endif

/****************************************************************************
 * Name: stm32_qencoder_initialize
 *
 * Description:
 *   Initialize and register a quadrature encoder.
 *
 ****************************************************************************/

#ifdef CONFIG_SENSORS_QENCODER
int stm32_qencoder_initialize(const char *devpath, int timer);
#endif
