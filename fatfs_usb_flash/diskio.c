/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"     /* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */

#include "fatfs_disk.h"

#include <stdio.h>
#include "pico/stdlib.h"

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

    printf("disk_status: pdrv=%i\n", pdrv);

    switch (pdrv)
    {
    case EDriveType_Flash:
        return 0;

    case EDriveType_USB:
        return STA_NOINIT;

    case EDriveType_SD:
        return STA_NOINIT;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    DSTATUS stat;
    int result;

    printf("disk_initialize: pdrv=%i\n", pdrv);

    if (pdrv == EDriveType_Flash)
    {
        return 0;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
    BYTE pdrv,    /* Physical drive nmuber to identify the drive */
    BYTE *buff,   /* Data buffer to store read data */
    LBA_t sector, /* Start sector in LBA */
    UINT count    /* Number of sectors to read */
)
{
    DRESULT res;
    int result;

    if (pdrv == EDriveType_Flash)
    {
        res = fatfs_disk_read((uint8_t *)buff, sector, count);
        return res;
    }

    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
    BYTE pdrv,        /* Physical drive nmuber to identify the drive */
    const BYTE *buff, /* Data to be written */
    LBA_t sector,     /* Start sector in LBA */
    UINT count        /* Number of sectors to write */
)
{
    DRESULT res;
    int result;

    if (pdrv == EDriveType_Flash)
    {
        res = fatfs_disk_write((const uint8_t *)buff, sector, count);
        return res;
    }

    return RES_PARERR;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
    BYTE pdrv, /* Physical drive nmuber (0..) */
    BYTE cmd,  /* Control code */
    void *buff /* Buffer to send/receive control data */
)
{
    DRESULT res;
    int result;

    printf("disk_ioctl: pdrv=%i\n", pdrv);

    if (pdrv == EDriveType_Flash)
    {
        switch (cmd)
        {
        case CTRL_SYNC:
            fatfs_disk_sync();
            return RES_OK;
        case GET_SECTOR_COUNT:
            *(LBA_t *)buff = FATFS_FLASH_NUM_FAT_SECTORS;
            return RES_OK;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = FATFS_FLASH_SECTOR_SIZE;
            return RES_OK;
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = 1;
            return RES_OK;
        case CTRL_TRIM:
            return RES_OK;
        default:
            return RES_PARERR;
        }
    }

    return RES_PARERR;
}
