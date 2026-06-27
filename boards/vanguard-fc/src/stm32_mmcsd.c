/****************************************************************************
 * boards/arm/stm32h7/vanguard-fc/src/stm32_mmcsd.c
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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <stdio.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/mmcsd.h>
#include <nuttx/spi/spi.h>

#include "stm32_gpio.h"
#include "stm32_spi.h"

#include "vanguard-stm32h7.h"

#ifdef CONFIG_MMCSD_SPI

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_spi_cs
 *
 * Description:
 *   SPI Chip Select.
 *
 ****************************************************************************/

static void stm32_spi_cs(FAR struct spi_dev_s *dev, uint32_t devid,
                         bool selected)
{
  stm32_gpiowrite(GPIO_MMCSD_CS, !selected);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_mmcsd_spi_initialize
 *
 * Description:
 *   Initialize SPI-based MMC/SD card support
 *
 ****************************************************************************/

int stm32_mmcsd_spi_initialize(int minor)
{
  FAR struct spi_dev_s *spi;
  int ret;

  finfo("Initializing SPI port %d\n", 1);

  /* Get the SPI port */

  spi = stm32_spibus_initialize(1);
  if (!spi)
    {
      ferr("ERROR: Failed to initialize SPI port %d\n", 1);
      return -ENODEV;
    }

  /* Bind the SPI port to the slot */

  ret = mmcsd_spislotinitialize(minor, 0, spi);
  if (ret < 0)
    {
      mcerr("ERROR: Failed to bind SPI port %d to MMC/SD slot %d\n",
           1, minor);
      return ret;
    }

  finfo("Successfully initialized MMC/SD slot %d\n", minor);
  return OK;
}

#endif /* CONFIG_MMCSD_SPI */
