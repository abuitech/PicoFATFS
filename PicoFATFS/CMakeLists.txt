# ====================================================================================
# FatFS path and source
set(PICOFATFS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
# ====================================================================================


# ====================================================================================
# ====================================================================================
#
# FatFS with internal flash
#

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
#
# ====================================================================================
# ====================================================================================


#
#
#


# ====================================================================================
# ====================================================================================
#
# FatFS with USB source files
#

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
#
# ====================================================================================
# ====================================================================================


#
#
#



# ====================================================================================
# ====================================================================================
#
# FatFS with SD card reader
#

# ... source root folder
set(FATFS_SD_SOURCE_ROOT_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_sd)
set(FATFS_SD_SOURCE_SPI_ROOT_DIR ${PICOFATFS_SOURCE_DIR}/fatfs_sd/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src)
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
# ... source folders
set(FATFS_SD_INCLUDE_DIR_LIST
    ${FATFS_SD_SOURCE_SPI_INCLUDE_DIR}
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}
)

# ... source files
set(FatFsSdSourceList
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/dma_interrupts.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/sd_card.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/sd_timeouts.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/SDIO/rp2040_sdio.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/SDIO/sd_card_sdio.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/SPI/my_spi.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/SPI/sd_card_spi.c
    ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/SPI/sd_spi.c
    #
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/crash.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/crc.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/f_util.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/ff_stdio.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/file_stream.c
    # ${FATFS_SD_SOURCE_SPI_SRC_DIR}/glue.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/my_debug.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/my_rtc.c
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}/util.c
    #
    ${FATFS_SD_SOURCE_ROOT_DIR}/fatfs_sd_glue.c
)

if(PICO_RISCV)
    set(HWDEP_LIBS hardware_watchdog)
else()
    set(HWDEP_LIBS cmsis_core)
endif()

# .. lib dependencies
set (FatFsSdLibList
    hardware_dma
    hardware_pio
    hardware_spi
    pico_aon_timer
    ${HWDEP_LIBS}
)

#
#
# ====================================================================================
# ====================================================================================

# ====================================================================================
# ====================================================================================
#
#   PicoFATFS common/core
#

# ... source folder
set(PICOFATFS_COMMON_SOURCE_DIR ${PICOFATFS_SOURCE_DIR}/common)

# ... source files
set(PicoFatFsCommonSourceList
    ${PICOFATFS_COMMON_SOURCE_DIR}/picofatfs_diskio.c
)

#
# ====================================================================================
# ====================================================================================


#
#
#
# ====================================================================================
# ====================================================================================
# ====================================================================================
#
#
#

add_library(PicoFATFS INTERFACE)

pico_generate_pio_header(PicoFATFS ${FATFS_SD_SOURCE_SPI_SD_DRIVER_DIR}/SDIO/rp2040_sdio.pio)

target_compile_definitions(PicoFATFS INTERFACE 
    PICO_MAX_SHARED_IRQ_HANDLERS=8u
    # PICOFATFS_DEBUG_LOG
)

target_sources(PicoFATFS INTERFACE
    ${PicoFatFsCommonSourceList}
    ${FatFsSourceList}
    ${FatFsUsbImplSourceList}
    ${FatFsSdSourceList}
)

target_include_directories(PicoFATFS INTERFACE
    ${FATFS_SOURCE_DIR}
    ${FATFS_USB_FLASH_SOURCE_DIR}
    ${FATFS_SD_INCLUDE_DIR_LIST}
    ${FATFS_SD_SOURCE_SPI_SRC_DIR}
)


target_link_libraries(PicoFATFS INTERFACE
    ${FatFsLibList}
    ${FatFsUsbLibList}
    ${FatFsSdLibList}
    pico_stdlib
)