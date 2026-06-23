/****************************************************************************
 * boards/arm/stm32h7/vanguard-fc/src/stm32_appinitialize.c
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
#include <nuttx/board.h>
#include <sys/boardctl.h>
#include <sys/types.h>
#include <syslog.h>
#include <errno.h>

#include "stdlib.h"
#include "vanguard-stm32h7.h"
#include "sys/boardctl.h"
#include "sys/mount.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_app_initialize
 *
 * Description:
 *   Perform application specific initialization.  This function is never
 *   called directly from application code, but only indirectly via the
 *   (non-standard) boardctl() interface using the command BOARDIOC_INIT.
 *
 * Input Parameters:
 *   arg - The boardctl() argument is passed to the board_app_initialize()
 *         implementation without modification.  The argument has no
 *         meaning to NuttX; the meaning of the argument is a contract
 *         between the board-specific initialization logic and the
 *         matching application logic.  The value could be such things as a
 *         mode enumeration value, a set of DIP switch settings, a pointer
 *         to configuration data read from a file or serial FLASH, or
 *         whatever you would like to do with it.  Every implementation
 *         should accept zero/NULL as a default configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure to indicate the nature of the failure.
 *
 ****************************************************************************/

int board_app_initialize(uintptr_t arg)
{
  int ret = OK;

#ifdef CONFIG_BOARD_LATE_INITIALIZE
  /* Board initialization already performed by board_late_initialize() */

#else
  /* Perform board-specific initialization */

  ret = stm32_bringup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: stm32_bringup() failed: %d\n", ret);
      return ret;
    }
#endif

  system("sh /etc/rcS");

#ifdef CONFIG_USBDEV_COMPOSITE
  /* Connect the USB composite device (CDC/ACM console + CDC/ECM ethernet).
   * This triggers board_composite_initialize() and board_composite_connect()
   * in stm32_composite.c, which registers both interfaces with the USB
   * device controller and makes them visible to the host.
   */

  struct boardioc_usbdev_ctrl_s ctrl;

  ctrl.usbdev   = BOARDIOC_USBDEV_COMPOSITE;
  ctrl.action   = BOARDIOC_USBDEV_CONNECT;
  ctrl.instance = 0;
  ctrl.handle   = NULL;

  ret = boardctl(BOARDIOC_USBDEV_CONTROL, (uintptr_t)&ctrl);
  if (ret < 0)
    {
      syslog(LOG_ERR,
             "ERROR: boardctl(BOARDIOC_USBDEV_CONTROL) failed: %d\n", ret);
    }
#endif

  return ret;
}