# ====================================================================================
# FatFS path and source
set(PICOFATFS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})


# ====================================================================================
# FatFS with internal flash

# ... source folder
set(FATFS_SOURCE_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_0_1_5a/source)

# ... source files
set(FatFsSourceList
    # ${FATFS_SOURCE_DIR}/diskio.c
    ${FATFS_SOURCE_DIR}/ff.c
    ${FATFS_SOURCE_DIR}/ffsystem.c
    ${FATFS_SOURCE_DIR}/ffunicode.c
)

# ... lib dependencies
set (FatFsLibList
    hardware_flash
    hardware_sync
)

#
# ====================================================================================

#
#
#

# ====================================================================================
# FatFS with USB source files

# ... source folder
set(FATFS_USB_FLASH_SOURCE_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_usb_flash)

# ... source files
set (FatFsUsbImplSourceList
    ${FATFS_USB_FLASH_SOURCE_DIR}/fatfs_disk.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/fatfs_flash.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_descriptors.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_msc_callbacks.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_msc_driver.c
)

# .. lib dependencies
set (FatFsUsbLibList
    tinyusb_additions
    tinyusb_board
    tinyusb_device
)

#
# ====================================================================================

#
#
#

# ====================================================================================
# FatFS with SD card reader

# ... source root folder
set(FATFS_SD_SOURCE_ROOT_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_sd)
set(FATFS_SD_SOURCE_SPI_ROOT_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_sd/no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI)
set(FATFS_SD_SOURCE_SPI_INCLUDE_DIR ${FATFS_SD_SOURCE_SPI_ROOT_DIR}/include)
set(FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR ${FATFS_SD_SOURCE_SPI_ROOT_DIR}/sd_driver)
set(FATFS_SD_SOURCE_SPI_SRC_DIR ${FATFS_SD_SOURCE_SPI_ROOT_DIR}/src)

# ... source folders
set(FATFS_SD_SOURCE_DIR_LIST
    ${FATFS_SD_SOURCE_ROOT_DIR}
    ${FATFS_SD_SOURCE_SPI_INCLUDE_DIR}
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}
)

# ... source files
set(FatFsSdSourceList
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/sd_spi.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/demo_logging.c
#    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/hw_config.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/spi.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/sd_card.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/crc.c
    #
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/f_util.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/ff_stdio.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/my_debug.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/rtc.c
    #
    ${FATFS_SD_SOURCE_ROOT_DIR}/fatfs_sd_glue.c
)

# .. lib dependencies
set (FatFsSdLibList
    hardware_spi
    hardware_dma
    hardware_rtc
)

#
# ====================================================================================



# ====================================================================================
#
#   Common
#

# ... source folder
set(PICOFATFS_COMMON_SOURCE_DIR ${PICOFATFS_SOURCE_DIR}/common)

# ... source files
set(PicoFatFsCommonSourceList
    ${PICOFATFS_COMMON_SOURCE_DIR}/diskio.c
)

#
# ### All together
#
set(PicoFatFsSourceList
    ${PicoFatFsCommonSourceList}
    ${FatFsSourceList}
    ${FatFsUsbImplSourceList}
    ${FatFsSdSourceList}
)

set(PicoFatFsLibList
    ${FatFsLibList}
    ${FatFsUsbLibList}
    ${FatFsSdLibList}
)


#
# ====================================================================================

