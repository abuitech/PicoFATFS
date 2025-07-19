# FatFS path and source
set(PICOFATFS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
set(FATFS_SOURCE_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_0_1_5a/source)
set(FATFS_USB_FLASH_SOURCE_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_usb_flash)
set(FatFsSourceList
# FatFS source files
    # ${FATFS_SOURCE_DIR}/diskio.c
    ${FATFS_SOURCE_DIR}/ff.c
    ${FATFS_SOURCE_DIR}/ffsystem.c
    ${FATFS_SOURCE_DIR}/ffunicode.c
)
set (FatFsUsbImplSourceList
# FatFS USB Flash impl source files
    ${FATFS_USB_FLASH_SOURCE_DIR}/diskio.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/fatfs_disk.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/fatfs_flash.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_descriptors.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_msc_callbacks.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_msc_driver.c
)

