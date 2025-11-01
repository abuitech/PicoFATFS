#include "diskio.h"
#include "fatfs_disk.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"

void create_fatfs_drive(const TCHAR* drivePath, const MKFS_PARM* opt, const TCHAR* driveLabel, const TCHAR* readmeFileName, const TCHAR* readmeContent)
{
    flash_format();
    flash_init();

    // now create a fatfs on the flash_fs filesystem :-)

    FATFS fs;             /* Filesystem object */
    FIL fil;              /* File object */
    BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

    /* Format the default drive with default parameters */
    printf("making fatfs\n");
    f_mkfs("", 0, work, sizeof work);
    f_mount(&fs, "", 0);
    f_setlabel(FATFS_FLASH_DEFAULT_DRIVE_LABEL);

    if (readmeFileName != NULL && readmeContent != NULL)
    {
        f_open(&fil, readmeFileName, FA_CREATE_NEW | FA_WRITE);
        f_puts(readmeContent, &fil);
        f_close(&fil);
    }

    f_mount(0, "", 0);
}

void create_builtin_flash_drive()
{
    if (FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE)
    {
        create_fatfs_drive("", NULL, FATFS_FLASH_DEFAULT_DRIVE_LABEL, FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE_FILE_NAME, FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE_FILE_CONTENT);
    }
    else
    {
        create_fatfs_drive("", NULL, FATFS_FLASH_DEFAULT_DRIVE_LABEL, NULL, NULL);
    }
}


