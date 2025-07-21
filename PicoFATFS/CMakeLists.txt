include(./shared.cmake)

add_library(PicoFATFS INTERFACE)

target_sources(PicoFATFS INTERFACE
     ${PicoFatFsSourceList}
)

target_include_directories(PicoFATFS INTERFACE
    ${FATFS_SOURCE_DIR}
    ${FATFS_USB_FLASH_SOURCE_DIR}
    ${FATFS_SD_SOURCE_DIR_LIST}
)

target_link_libraries(PicoFATFS INTERFACE
    ${PicoFatFsLibList}
)