#ifndef FATFS_DISK_H
#define FATFS_DISK_H

#include "fatfs_flash.h"

#ifdef __cplusplus
extern "C"
{
#endif

void create_fatfs_disk();
bool mount_fatfs_disk();
bool fatfs_is_mounted();
uint32_t fatfs_disk_read(uint8_t* buff, uint32_t sector, uint32_t count);
uint32_t fatfs_disk_write(const uint8_t* buff, uint32_t sector, uint32_t count);
void fatfs_disk_sync();


#ifdef __cplusplus
}
#endif

#endif // FATFS_DISK_H