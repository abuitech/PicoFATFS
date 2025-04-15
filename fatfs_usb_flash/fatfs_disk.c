#include "ff.h"
#include "diskio.h"
#include "fatfs_disk.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"

bool flashfs_is_mounted = false;

bool mount_fatfs_disk()
{
    int err = flash_fs_mount();
    if (err)
        return false;

    flashfs_is_mounted = true;
    return true;
}

bool fatfs_is_mounted() { return flashfs_is_mounted; }

void create_fatfs_disk()
{
    flash_fs_create();
    flashfs_is_mounted = true;

    // now create a fatfs on the flash_fs filesystem :-)

    FATFS fs;             /* Filesystem object */
    FIL fil;              /* File object */
    FRESULT res;          /* API result code */
    BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */

    /* Format the default drive with default parameters */
    printf("making fatfs\n");
    res = f_mkfs("", 0, work, sizeof work);
    f_mount(&fs, "", 0);
    f_setlabel(FATFS_FLASH_DEFAULT_DRIVE_LABEL);

    if (FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE)
    {
        res = f_open(&fil, FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE_FILE_NAME, FA_CREATE_NEW | FA_WRITE);
        f_puts(FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE_FILE_CONTENT, &fil);
        f_close(&fil);
    }

    f_mount(0, "", 0);
}

uint32_t fatfs_disk_read(uint8_t *buff, uint32_t sector, uint32_t count)
{
    //	printf("fatfs_disk_read sector=%d, count=%d\n", sector, count);
    if (!flashfs_is_mounted)
        return RES_ERROR;
    if (sector < 0 || sector >= FATFS_FLASH_NUM_FAT_SECTORS)
        return RES_PARERR;

    /* copy data to buffer */
    for (int i = 0; i < count; i++)
        flash_fs_read_FAT_sector(sector + i, buff + (i * FATFS_FLASH_SECTOR_SIZE));
    return RES_OK;
}

uint32_t fatfs_disk_write(const uint8_t *buff, uint32_t sector, uint32_t count)
{
    // 	printf("fatfs_disk_write sector=%d, count=%d\n", sector, count);
    if (!flashfs_is_mounted)
        return RES_ERROR;
    if (sector < 0 || sector >= FATFS_FLASH_NUM_FAT_SECTORS)
        return RES_PARERR;

    /* copy data to buffer */
    for (int i = 0; i < count; i++)
    {
        flash_fs_write_FAT_sector(sector + i, buff + (i * FATFS_FLASH_SECTOR_SIZE));
        // verify
        if (!flash_fs_verify_FAT_sector(sector + i, buff + (i * FATFS_FLASH_SECTOR_SIZE)))
        {
            printf("VERIFY ERROR!");
            return RES_ERROR;
        }
    }
    return RES_OK;
}

void fatfs_disk_sync()
{
    flash_fs_sync();
}