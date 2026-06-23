/****************************************************************************
 * boards/arm/stm32h7/vanguard-fc/src/stm32_composite.c
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
#include <errno.h>
#include <debug.h>
#include <syslog.h>

#  include <nuttx/usb/composite.h>
#  include <nuttx/usb/cdcacm.h>
#  include <nuttx/usb/composite.h>
#  include <nuttx/usb/cdcacm.h>
#  include <nuttx/usb/cdcecm.h>
#  include <nuttx/usb/rndis.h>

#include "vanguard-stm32h7.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_USBDEV_COMPOSITE

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_composite_initialize
 *
 * Description:
 *   Perform architecture-specific initialization of the composite USB
 *   device. Called by boardctl(BOARDIOC_USBDEV_CONTROL) when
 *   USBDEV_COMPOSITE is selected.
 *
 * Input Parameters:
 *   port - For future use; should be zero.
 *
 * Returned Value:
 *   Zero (OK) on success, negated errno on failure.
 *
 ****************************************************************************/

int board_composite_initialize(int port)
{
  /* Nothing board-specific needed here for Vanguard FC.
   * USB GPIO pins are already configured in stm32_usbinitialize().
   */

  return OK;
}

/****************************************************************************
 * Name: board_composite_connect
 *
 * Description:
 *   Connect the composite USB device. Called by boardctl after
 *   board_composite_initialize(). Returns a handle to the composite
 *   device that can be used to disconnect it later.
 *
 * Input Parameters:
 *   port    - For future use; should be zero.
 *   inst    - Composite device instance number; should be zero.
 *
 * Returned Value:
 *   Non-NULL handle on success, NULL on failure.
 *
 ****************************************************************************/

FAR void *board_composite_connect(int port, int inst)
{
 static struct composite_devdesc_s dev[2];
 int n = 0;
 int ifnobase = 0;
 int epin = 1;
 int epout = 1;

 memset(dev, 0, sizeof(dev));

#ifdef CONFIG_RNDIS_COMPOSITE
 usbdev_rndis_get_composite_devdesc(&dev[n]);
 dev[n].devinfo.ifnobase = ifnobase;
 // Update ifnobase based on how many interfaces RNDIS actually uses
 ifnobase += dev[n].devinfo.ninterfaces;

 dev[n].devinfo.epno[RNDIS_EP_INTIN_IDX]  = epin++;
 dev[n].devinfo.epno[RNDIS_EP_BULKIN_IDX] = epin++;
 dev[n].devinfo.epno[RNDIS_EP_BULKOUT_IDX]= epout++;
 n++;
#endif

#ifdef CONFIG_CDCACM_COMPOSITE
 cdcacm_get_composite_devdesc(&dev[n]);
 dev[n].classobject      = cdcacm_classobject;
 dev[n].uninitialize     = cdcacm_uninitialize;
 dev[n].devinfo.ifnobase = ifnobase; // Use the running total
 ifnobase += dev[n].devinfo.ninterfaces;

 dev[n].devinfo.epno[CDCACM_EP_INTIN_IDX]   = epin++;
 dev[n].devinfo.epno[CDCACM_EP_BULKIN_IDX]  = epin++;
 dev[n].devinfo.epno[CDCACM_EP_BULKOUT_IDX] = epout++;
 n++;
#endif

 return composite_initialize(composite_getdevdescs(), dev, n);
}

#endif /* CONFIG_USBDEV_COMPOSITE */