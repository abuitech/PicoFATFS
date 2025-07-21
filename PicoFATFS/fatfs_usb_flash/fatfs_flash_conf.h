#ifndef FATFS_FLASH_CONF_H
#define FATFS_FLASH_CONF_H


#define FATFS_FLASH_STORAGE_BASE  (1024 * 1024)
#define FATFS_FLASH_MAGIC_8_BYTES "RHE!FS30"

#define FATFS_FLASH_NUM_FAT_SECTORS 30716   // 15megs / 512bytes = 30720, but we used 4 records for the header (8 bytes)
#define FATFS_FLASH_NUM_FLASH_SECTORS 3840  // 15megs / 4096bytes = 3840
#define FATFS_FLASH_SECTOR_SIZE 512

#define FATFS_FLASH_USB_VID "PicoFlashFAT"
#define FATFS_FLASH_USB_PID "Mass Storage"
#define FATFS_FLASH_USB_REV "1.0"

#define FATFS_FLASH_DEFAULT_DRIVE_LABEL "PicoFlash"

#define FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE 1
#define FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE_FILE_NAME "README.TXT"
#define FATFS_FLASH_CREATE_FILE_ON_DISK_CREATE_FILE_CONTENT "Pico flash as an FAT disk!\r\nFiles and folders may be added and removed here\r\n"


#endif // FATFS_FLASH_CONF_H