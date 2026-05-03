#ifndef MYBOARD_PICO
#define MYBOARD_PICO

#include "boards/pico.h"

#ifdef PICO_FLASH_SPI_CLKDIV
#undef PICO_FLASH_SPI_CLKDIV
#endif
#define PICO_FLASH_SPI_CLKDIV 4

#ifdef PICO_FLASH_SIZE_BYTES
#undef PICO_FLASH_SIZE_BYTES
#endif
#define PICO_FLASH_SIZE_BYTES (16 * 1024 * 1024)


#endif // MYBOARD_PICO