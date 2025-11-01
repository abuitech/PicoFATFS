# Generated Cmake Pico project file

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
    ${CMAKE_CURRENT_LIST_DIR}/sd_hw_config.c
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
    ${PlatformDefineList}
    #HAS_DEV_FLASH_IMPL
    # USE_PRINTF=1
    FATFS_FLASH_CONF="fatfs_flash_conf.h"
    # SYS_CLK_MHZ=360
    # PLL_SYS_VCO_FREQ_HZ=1500000000
    # PLL_SYS_POSTDIV1=5
    # PLL_SYS_POSTDIV2=2
    # SYS_CLK_VREG_VOLTAGE_MIN=VREG_VOLTAGE_1_30
)

# ====================================================================================
#
#

set(TARGET_VERSION v0.6)
set(TARGET_NAME PicoFatFsTest_${TARGET_PLATFORM}_${TARGET_VERSION})

#
#
# ====================================================================================


# Add executable. Default name is the project name, version 0.1
add_executable(${TARGET_NAME} ${SourceFileList})

pico_set_linker_script(${TARGET_NAME} ../PicoFATFS/fatfs_usb_flash/memmap_custom.ld)

pico_set_program_name(${TARGET_NAME} "PicoFATFS")
pico_set_program_version(${TARGET_NAME} "${TARGET_VERSION}")

# Add the standard library to the build
target_link_libraries(${TARGET_NAME} ${LibList})

# Add the standard include files to the build
target_include_directories(${TARGET_NAME} PRIVATE ${IncludePaths})

target_compile_definitions(${TARGET_NAME} PRIVATE ${DefineList})

pico_add_extra_outputs(${TARGET_NAME})

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${TARGET_NAME} 1)
pico_enable_stdio_usb(${TARGET_NAME} 1)
