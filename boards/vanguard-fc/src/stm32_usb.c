/****************************************************************************
 * boards/arm/stm32h7/vanguard-fc/src/stm32_usb.c
 *
 * SPDX-License-Identifier: Apache-2.0
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

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/kthread.h>
#include <nuttx/usb/usbdev.h>
#include <nuttx/usb/usbhost.h>
#include <nuttx/usb/usbdev_trace.h>

#include "arm_internal.h"
#include "chip.h"
#include "stm32_gpio.h"
#include "stm32_otg.h"
#include "vanguard-stm32h7.h"

#ifdef CONFIG_STM32H7_OTGFS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_USBDEV) || defined(CONFIG_USBHOST)
#  define HAVE_USB 1
#else
#  warning "CONFIG_STM32H7_OTGFS is enabled but neither CONFIG_USBDEV nor CONFIG_USBHOST"
#  undef HAVE_USB
#endif

#ifndef CONFIG_VANGUARD_USBHOST_PRIO
#  define CONFIG_VANGUARD_USBHOST_PRIO 100
#endif

#ifndef CONFIG_VANGUARD_USBHOST_STACKSIZE
#  define CONFIG_VANGUARD_USBHOST_STACKSIZE 1024
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_USBHOST
static struct usbhost_connection_s *g_usbconn;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: usbhost_waiter
 *
 * Description:
 *   Wait for USB devices to be connected.
 *
 ****************************************************************************/

#ifdef CONFIG_USBHOST
static int usbhost_waiter(int argc, char *argv[])
{
  struct usbhost_hubport_s *hport;

  uinfo("Running\n");
  for (; ; )
    {
      /* Wait for the device to change state */

      DEBUGVERIFY(CONN_WAIT(g_usbconn, &hport));
      uinfo("%s\n", hport->connected ? "connected" : "disconnected");

      /* Did we just become connected? */

      if (hport->connected)
        {
          /* Yes.. enumerate the newly connected device */

          CONN_ENUMERATE(g_usbconn, hport);
        }
    }

  /* Keep the compiler from complaining */

  return 0;
}
#endif /* CONFIG_USBHOST */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_usbinitialize
 *
 * Description:
 *   Called from stm32_boardinitialize() early in initialization to set up
 *   USB-related GPIO pins for the Vanguard FC board.
 *
 *   Note: The Vanguard FC uses only the OTG FS data pins (DM/DP/ID).
 *   There is no external VBUS control or overcurrent GPIO — the STM32H743
 *   OTG FS peripheral handles VBUS sensing internally.
 *
 *   Actual USB class drivers (CDC/ACM, CDC/ECM, composite) are initialized
 *   later in stm32_bringup().
 *
 ****************************************************************************/

void stm32_usbinitialize(void)
{
#ifdef CONFIG_STM32H7_OTGFS
  /* Configure OTG FS data and ID pins.
   * DM = PA11, DP = PA12, ID = PA10 (defined in board.h)
   * VBUS/PWRON/OVER are not present on this board.
   */

  stm32_configgpio(GPIO_OTGFS_DM);
  stm32_configgpio(GPIO_OTGFS_DP);
  stm32_configgpio(GPIO_OTGFS_ID);
#endif
}

/****************************************************************************
 * Name: stm32_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host
 *   functionality. Starts a thread that monitors for device
 *   connection/disconnection events.
 *
 ****************************************************************************/

#ifdef CONFIG_USBHOST
int stm32_usbhost_initialize(void)
{
  int ret;

  uinfo("Register class drivers\n");

#ifdef CONFIG_USBHOST_HUB
  ret = usbhost_hub_initialize();
  if (ret < 0)
    {
      uerr("ERROR: usbhost_hub_initialize failed: %d\n", ret);
    }
#endif

#ifdef CONFIG_USBHOST_MSC
  ret = usbhost_msc_initialize();
  if (ret != OK)
    {
      uerr("ERROR: Failed to register the mass storage class: %d\n", ret);
    }
#endif

#ifdef CONFIG_USBHOST_CDCACM
  ret = usbhost_cdcacm_initialize();
  if (ret != OK)
    {
      uerr("ERROR: Failed to register the CDC/ACM serial class: %d\n", ret);
    }
#endif

#ifdef CONFIG_USBHOST_HIDKBD
  ret = usbhost_kbdinit();
  if (ret != OK)
    {
      uerr("ERROR: Failed to register the HID keyboard class\n");
    }
#endif

#ifdef CONFIG_USBHOST_HIDMOUSE
  ret = usbhost_mouse_init();
  if (ret != OK)
    {
      uerr("ERROR: Failed to register the HID mouse class\n");
    }
#endif

  uinfo("Initialize USB host\n");
  g_usbconn = stm32_otgfshost_initialize(0);
  if (g_usbconn)
    {
      uinfo("Start usbhost_waiter\n");
      ret = kthread_create("usbhost",
                           CONFIG_VANGUARD_USBHOST_PRIO,
                           CONFIG_VANGUARD_USBHOST_STACKSIZE,
                           usbhost_waiter, NULL);
      return ret < 0 ? -ENOEXEC : OK;
    }

  return -ENODEV;
}
#endif /* CONFIG_USBHOST */

/****************************************************************************
 * Name: stm32_usbhost_vbusdrive
 *
 * Description:
 *   Enable/disable driving of VBUS 5V output.
 *   The Vanguard FC has no external VBUS power switch GPIO, so this is
 *   a no-op stub required by the OTG FS host driver interface.
 *
 * Input Parameters:
 *   iface  - Should be zero.
 *   enable - true: enable VBUS power; false: disable VBUS power
 *
 ****************************************************************************/

#ifdef CONFIG_USBHOST
void stm32_usbhost_vbusdrive(int iface, bool enable)
{
  DEBUGASSERT(iface == 0);

  /* No external VBUS power switch on this board -- VBUS is always powered
   * from the USB host connector directly.
   */
}
#endif /* CONFIG_USBHOST */

/****************************************************************************
 * Name: stm32_usbsuspend
 *
 * Description:
 *   Board logic must provide stm32_usbsuspend() if the USBDEV driver is
 *   used. Called whenever the USB enters or leaves suspend mode.
 *
 ****************************************************************************/

#ifdef CONFIG_USBDEV
void stm32_usbsuspend(struct usbdev_s *dev, bool resume)
{
  uinfo("resume: %d\n", resume);
}
#endif /* CONFIG_USBDEV */

#endif /* CONFIG_STM32H7_OTGFS */