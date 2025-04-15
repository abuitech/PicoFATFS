# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# ====================================================================================
#
#

set(TARGET_VERSION v0.1)
set(TARGET_NAME PicoFATFS_${TARGET_VERSION})

#
#
# ====================================================================================

project(${TARGET_NAME} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()


# ====================================================================================
# ====================================================================================


# FatFS source path
set(FATFS_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/fatfs_0_1_5a/source)
set(FATFS_USB_FLASH_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/fatfs_usb_flash)


# ====================================================================================
# ====================================================================================

# ====================================================================================
# Include path list
# ====================================================================================
set(IncludePaths
    ${CMAKE_CURRENT_LIST_DIR}
    ${FATFS_SOURCE_DIR}
    ${FATFS_USB_FLASH_SOURCE_DIR}
)

# ====================================================================================
# Source files list
# ====================================================================================
set(SourceFileList
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
# FatFS source files
    # ${FATFS_SOURCE_DIR}/diskio.c
    ${FATFS_SOURCE_DIR}/ff.c
    ${FATFS_SOURCE_DIR}/ffsystem.c
    ${FATFS_SOURCE_DIR}/ffunicode.c
# FatFS USB Flash impl source files
    ${FATFS_USB_FLASH_SOURCE_DIR}/diskio.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/fatfs_disk.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/fatfs_flash.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_descriptors.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_msc_callbacks.c
    ${FATFS_USB_FLASH_SOURCE_DIR}/usb_msc_driver.c
)

# ====================================================================================
# Defines list
# ====================================================================================
set(DefineList
    #HAS_DEV_FLASH_IMPL
)


# Add executable. Default name is the project name, version 0.1
add_executable(${TARGET_NAME})

pico_set_program_name(${TARGET_NAME} "PicoFATFS")
pico_set_program_version(${TARGET_NAME} "${TARGET_VERSION}")

target_sources(${TARGET_NAME} PRIVATE ${SourceFileList})

# Add the standard library to the build
target_link_libraries(${TARGET_NAME}
    pico_stdlib
    tinyusb_additions
    tinyusb_board
    tinyusb_device
    hardware_flash
    hardware_sync
)

# Add the standard include files to the build
target_include_directories(${TARGET_NAME} PRIVATE ${IncludePaths})

target_compile_definitions(${TARGET_NAME} PRIVATE ${DefineList})

pico_add_extra_outputs(${TARGET_NAME})


# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${TARGET_NAME} 1)
pico_enable_stdio_usb(${TARGET_NAME} 1)
