#include <nuttx/config.h>
#include <nuttx/fs/fs.h>
#include <nuttx/fs/ioctl.h>
#include <nuttx/drivers/ramdisk.h>
#include "romfs.h"              // REQUIRED to get romfs_img and romfs_img_len
#include "sys/mount.h"

int stm32_romfs_initialize(void)
{
    int ret;

    ret = ramdisk_register(0, romfs_img, romfs_img_len / 512, 512 ,true);

    if (ret < 0) {
        return ret;
    }

    ret = mount("/dev/ram0", "/etc", "romfs", MS_RDONLY, NULL);
    return ret;
}
