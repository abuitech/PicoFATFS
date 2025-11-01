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
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

#
# ====================================================================================
#
# ====================================================================================
#

option(PICO_1_GEN "Pico 1 generation (ON) or Pico 2 generation (OFF)" ON)

IF (PICO_1_GEN)
    set(PICO_BOARD myboard_pico CACHE STRING "Board type")
    set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/PicoFatFsTest)
    set(TARGET_PLATFORM rp2040)
    set(PlatformDefineList
        PICO_PLATFORM=${TARGET_PLATFORM}
    )
ELSE (PICO_1_GEN)
    set(PICO_BOARD myboard_pga2350 CACHE STRING "Board type")
    set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/PicoFatFsTest)
    set(TARGET_PLATFORM rp2350)
    set(PlatformDefineList
        PICO_PLATFORM=${TARGET_PLATFORM}
    )
ENDIF (PICO_1_GEN)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(TestUSBDriver C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_compile_options(-Wall -Wunused-variable)

add_subdirectory(PicoFATFS)
add_subdirectory(PicoFatFsTest)
