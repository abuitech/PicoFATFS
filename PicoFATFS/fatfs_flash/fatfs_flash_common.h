#ifndef FATFS_FLASH_COMMON_H
#define FATFS_FLASH_COMMON_H


#if !defined(FATFS_FLASH_CONF)
#include "fatfs_flash_conf.h"
#else
#include FATFS_FLASH_CONF
#endif

/* Definitions of physical drive number for each drive */
enum EDriveType
{
    EDriveType_Flash,
    EDriveType_SD
};


#endif //FATFS_FLASH_COMMON_H