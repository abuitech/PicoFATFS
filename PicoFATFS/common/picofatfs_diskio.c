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

#include "fatfs_flash_common.h"
#include "fatfs_flash.h"

#include <stdio.h>
#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

DSTATUS fatfs_sd_disk_status(BYTE pdrv);
DSTATUS fatfs_sd_disk_initialize(BYTE pdrv);
DRESULT fatfs_sd_disk_read(BYTE pdrv,    /* Physical drive number to identify the drive */
                  BYTE *buff,   /* Data buffer to store read data */
                  LBA_t sector, /* Start sector in LBA */
                  UINT count /* Number of sectors to read */
                );
DRESULT fatfs_sd_disk_write(BYTE pdrv,        /* Physical drive number to identify the drive */
                   const BYTE *buff, /* Data to be written */
                   LBA_t sector,     /* Start sector in LBA */
                   UINT count        /* Number of sectors to write */
                );
DRESULT fatfs_sd_disk_ioctl(BYTE pdrv, /* Physical drive number (0..) */
                   BYTE cmd,  /* Control code */
                   void *buff /* Buffer to send/receive control data */
                );
#ifdef __cplusplus
}
#endif


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
    BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
    // printf("disk_status: pdrv=%i\n", pdrv);

    switch (pdrv)
    {
    case EDriveType_Flash:
        return RES_OK;

    case EDriveType_SD:
        return fatfs_sd_disk_status(pdrv);
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
    // printf("disk_initialize: pdrv=%i\n", pdrv);

    switch (pdrv)
    {
    case EDriveType_Flash:
        return flash_init() ? RES_OK : RES_ERROR;

    case EDriveType_SD:
        return fatfs_sd_disk_initialize(pdrv);
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
    switch (pdrv)
    {
    case EDriveType_Flash:
    {
        if (sector > get_lba_count())
        {
            return RES_ERROR;
        }
        for (unsigned int i = 0; i < count; i++)
        {
            flash_read(sector + i, buff + i * get_lba_size());
        }
    return RES_OK;
    }

    case EDriveType_SD:
        return fatfs_sd_disk_read(pdrv, buff, sector, count);
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
    switch (pdrv)
    {
    case EDriveType_Flash:
    {
        if (sector > get_lba_count())
        {
            return RES_ERROR;
        }
        for (unsigned int i = 0; i < count; i++)
        {
            flash_write(sector + i, buff + i * get_lba_size());
        }
        return RES_OK;
    }

    case EDriveType_SD:
        return fatfs_sd_disk_write(pdrv, buff, sector, count);
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
    // printf("disk_ioctl: pdrv=%i\n", pdrv);

    switch (pdrv)
    {
    case EDriveType_Flash:
    {
        switch (cmd)
        {
        case CTRL_SYNC:
            flash_persist();
            return RES_OK;
        case GET_SECTOR_COUNT:
            *(LBA_t *)buff = get_lba_count();
            return RES_OK;
        case GET_SECTOR_SIZE:
            *(WORD *)buff = get_lba_size();
            return RES_OK;
        case GET_BLOCK_SIZE:
            *(DWORD *)buff = get_lba_size();
            return RES_OK;
        case CTRL_TRIM:
        {
            LBA_t *lba = (LBA_t *)buff;
            for (unsigned int i = lba[0]; i < lba[1]; i++)
            {
                flash_trim(i);
            }
            return RES_OK;
        }
        default:
            return RES_PARERR;
        }
    }

    case EDriveType_SD:
        return fatfs_sd_disk_ioctl(pdrv, cmd, buff);
    }

    return RES_PARERR;
}
