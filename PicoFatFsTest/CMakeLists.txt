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
set(PICO_BOARD myboard_pico CACHE STRING "Board type")
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR})

set(PICOFATFS_ROOT_DIR ../PicoFATFS)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# PicoFatFS shared cmake
include(${PICOFATFS_ROOT_DIR}/shared.cmake)

# ====================================================================================
#
#

set(TARGET_VERSION v0.3)
set(TARGET_NAME PicoFatFsTest_${TARGET_VERSION})

#
#
# ====================================================================================

project(${TARGET_NAME} C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory(${PICOFATFS_ROOT_DIR} build)


# ====================================================================================
# ====================================================================================

# ====================================================================================
# Include path list
# ====================================================================================
set(IncludePaths
    ${CMAKE_CURRENT_LIST_DIR}
)

# ====================================================================================
# Source files list
# ====================================================================================
set(SourceFileList
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

# ====================================================================================
# Lib Dependencies list
# ====================================================================================
set(LibList
    pico_stdlib
    PicoFATFS
)


# ====================================================================================
# Defines list
# ====================================================================================
set(DefineList
    #HAS_DEV_FLASH_IMPL
    FATFS_FLASH_CONF="fatfs_flash_conf.h"
    PICO_PLATFORM=rp2040
    # SYS_CLK_MHZ=360
    # PLL_SYS_VCO_FREQ_HZ=1500000000
    # PLL_SYS_POSTDIV1=5
    # PLL_SYS_POSTDIV2=2
    # SYS_CLK_VREG_VOLTAGE_MIN=VREG_VOLTAGE_1_30
)


# Add executable. Default name is the project name, version 0.1
add_executable(${TARGET_NAME})

pico_set_program_name(${TARGET_NAME} "PicoFATFS")
pico_set_program_version(${TARGET_NAME} "${TARGET_VERSION}")

target_sources(${TARGET_NAME} PRIVATE ${SourceFileList})

# Add the standard library to the build
target_link_libraries(${TARGET_NAME} ${LibList})

# Add the standard include files to the build
target_include_directories(${TARGET_NAME} PRIVATE ${IncludePaths})

target_compile_definitions(${TARGET_NAME} PRIVATE ${DefineList})

pico_add_extra_outputs(${TARGET_NAME})


# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${TARGET_NAME} 1)
pico_enable_stdio_usb(${TARGET_NAME} 1)
