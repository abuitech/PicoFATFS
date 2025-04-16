#include <stdio.h>
#include "pico/stdlib.h"

#include "usb_msc_callbacks.h"
#include "fatfs_disk.h"
#include "ff.h"

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
void TestSample()
{
    static bool testDone = false;
    if (testDone)
        return;

    printf("Executing... TestSample()!\n");

    TCHAR str[FF_MAX_LFN];
    FRESULT fr;
    FIL fil;

    f_open(&fil, "/test1.txt", FA_CREATE_NEW | FA_WRITE);
    f_puts("flash:/test1.txt ...", &fil);
    f_close(&fil);

    f_open(&fil, "flash:/test2.txt", FA_CREATE_NEW | FA_WRITE);
    f_puts("flash:/test2.txt ...", &fil);
    f_close(&fil);

    f_mkdir("flash:/test_folder");
    f_open(&fil, "flash:/test_folder/test_in_test_folder.txt", FA_CREATE_NEW | FA_WRITE);
    f_puts("flash:/test_folder/test_in_test_folder.txt ...", &fil);
    f_close(&fil);

    fr = f_getcwd(str, FF_MAX_LFN);
    printf("\tcwd=%s\n", str);      // OUPUT: cwd=flash:/
    f_mount(0, "flash", 0);

    testDone = true;
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

    printf("Executing... mount_fatfs_disk()\n");
    if (!mount_fatfs_disk())
    {
        printf("\tCreating FAT Filesystem disk\n");
        create_fatfs_disk();
    }

    printf("Executing... f_mount(&fs, \"flash\", 0)\n");
    FATFS fs;
    if (f_mount(&fs, "flash", 0) != FR_OK)
    {
        printf("\tError:mount has failed!\n");
    }

    while (true)
    {
        tud_task(); // tinyusb device task
        cdc_task();

        blink_led_task();
        TestSample();
    }
}
