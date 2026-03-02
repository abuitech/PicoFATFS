#ifndef FATFS_DISK_H
#define FATFS_DISK_H

#include "fatfs_flash.h"
#include "ff.h"

#ifdef __cplusplus
extern "C"
{
#endif

void create_fatfs_drive(const TCHAR *drivePath, const MKFS_PARM *opt, const TCHAR *driveLabel, const TCHAR *readmeFileName, const TCHAR *readmeContent);
void create_builtin_flash_drive();

#ifdef __cplusplus
}
#endif

#endif // FATFS_DISK_H