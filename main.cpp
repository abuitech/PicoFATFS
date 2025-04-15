#include <stdio.h>
#include "pico/stdlib.h"

#include "usb_msc_callbacks.h"
#include "fatfs_disk.h"

#include "bsp/board.h"
#include "tusb.h"




//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
void blink_led_task(void)
{
    const uint32_t interval_ms = 1000;
    static uint32_t start_ms = 0;
    static bool led_state = false;
    if (board_millis() - start_ms < interval_ms)
    {
        return;
    }
    start_ms += interval_ms;

    printf("Led state : %s\n", led_state ? "on" : "off");
    board_led_write(led_state);
    led_state = !led_state;
}


//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
int main()
{
    board_init();
    tud_init(BOARD_TUD_RHPORT);
    stdio_init_all();

    printf("\n\nPico FAT FS\n\n");

    create_fatfs_disk();

    while (true)
    {
        tud_task(); // tinyusb device task
        cdc_task();

        blink_led_task();
    }
}
