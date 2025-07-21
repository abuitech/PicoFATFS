#define disk_status fatfs_sd_disk_status
#define disk_initialize fatfs_sd_disk_initialize
#define disk_read fatfs_sd_disk_read
#define disk_write fatfs_sd_disk_write
#define disk_ioctl fatfs_sd_disk_ioctl

#include "glue.c"

#undef disk_status
#undef disk_initialize
#undef disk_read
#undef disk_write
#undef disk_ioctl
