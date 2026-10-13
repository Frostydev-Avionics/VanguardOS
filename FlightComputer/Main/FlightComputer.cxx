#include <nuttx/config.h>
#include <nuttx/board.h>
#include <nuttx/sched.h>
#include <nshlib/nshlib.h>

#include <sys/mount.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

#include "syslog.h"

/* Forward Declarations */
extern "C" int stm32_bringup(void);
extern "C" int FC_StateMachine_main(int argc, char *argv[]);

#ifdef CONFIG_SYSTEM_NSH
extern "C" int nsh_main(int argc, char *argv[]);
#endif

/* ===== Mount SD Card ===== */
static void mountSDCard(void)
{
  /* Ensure mount point exists */
  mkdir("/sdcard", 0777);

  int ret = mount("/dev/mmcsd0", "/sdcard", "vfat", 0, nullptr);
  if (ret < 0) {
    syslog(LOG_ERR, "[Main] SD Card mount failed: %d", errno);
  } else {
    syslog(LOG_INFO, "[Main] SD Card mounted at /sdcard.");
  }
}

extern "C" int FlightComputer_main_main(int argc, char *argv[])
{
  mountSDCard();

#ifdef CONFIG_SYSTEM_NSH

  int nsh_pid = task_create("USB_Console", 100, 4096, nsh_main, nullptr);
  if (nsh_pid > 0) {
    syslog(LOG_INFO, "[Main] NSH Console started (pid=%d)", nsh_pid);
  } else {
    syslog(LOG_ERR, "[Main] NSH Console failed to start");
  }
#endif

  return 0;
}