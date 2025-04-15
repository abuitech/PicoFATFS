#ifndef FATFS_FLASH_FS_H
#define FATFS_FLASH_FS_H

#include "fatfs_flash_common.h"
#include <stdbool.h>


#ifdef __cplusplus
extern "C"
{
#endif


int flash_fs_mount();
void flash_fs_create();
void flash_fs_sync();
void flash_fs_read_FAT_sector(uint16_t fat_sector, void *buffer);
void flash_fs_write_FAT_sector(uint16_t fat_sector, const void *buffer);
bool flash_fs_verify_FAT_sector(uint16_t fat_sector, const void *buffer);



#ifdef __cplusplus
}
#endif

#endif // FATFS_FLASH_FS_H