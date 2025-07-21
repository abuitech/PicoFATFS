#include <stdio.h>
#include "pico/stdlib.h"

#include "usb_msc_callbacks.h"
#include "fatfs_disk.h"
#include "ff.h"

#include "bsp/board.h"
#include "tusb.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include <algorithm>



//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
void measure_freqs(void)
{
    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
    uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);
#endif

    printf("Frequencies :\n");
    printf("\tpll_sys  = %dkHz\n", f_pll_sys);
    printf("\tpll_usb  = %dkHz\n", f_pll_usb);
    printf("\trosc     = %dkHz\n", f_rosc);
    printf("\tclk_sys  = %dkHz\n", f_clk_sys);
    printf("\tclk_peri = %dkHz\n", f_clk_peri);
    printf("\tclk_usb  = %dkHz\n", f_clk_usb);
    printf("\tclk_adc  = %dkHz\n", f_clk_adc);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
    printf("\tclk_rtc  = %dkHz\n", f_clk_rtc);
#endif

    // Can't measure clk_ref / xosc as it is the ref

    printf("\n\n");
}

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

    // printf("Led state : %s\n", led_state ? "on" : "off");
    board_led_write(led_state);
    led_state = !led_state;
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
void CopyTest(const char* srcFilepath, const char* dstFilepath)
{
    const UINT BufferSize{1024};
    uint8_t buffer[BufferSize];
    FRESULT fr;
    FIL srcFil, dstFil;

    printf("CopyTest: copy '%s' to '%s'\n", srcFilepath, dstFilepath);
    
    FILINFO fno;
    if ((fr = f_stat(srcFilepath, &fno)) != FR_OK)
    {
        printf("CopyTest has failed! Error on f_stat('%s') (fr=%i)\n", srcFilepath, fr);
        return;
    }

    if ((fr = f_open(&srcFil, srcFilepath, FA_OPEN_EXISTING | FA_READ)) != FR_OK)
    {
        printf("CopyTest has failed! Error on opening '%s' (fr=%i)\n", srcFilepath, fr);
        return;
    }

    if ((fr = f_open(&dstFil, dstFilepath, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
    {
        printf("CopyTest has failed! Error on opening '%s' (fr=%i)\n", dstFilepath, fr);
        f_close(&srcFil);
        return;
    }

    FSIZE_t copyCount{0};
    while (copyCount < fno.fsize)
    {
        UINT nToRead{std::min((FSIZE_t)BufferSize, fno.fsize - copyCount)};
        UINT nRead{0}, nWrite{0};
        fr = f_read(&srcFil, buffer, nToRead, &nRead);
        if (fr != FR_OK || nRead != nToRead)
        {
            printf("CopyTest has failed! Error on reading '%s' (fr=%i, copyCount=%lu, nToRead=%lu, nRead=%lu)\n", dstFilepath, fr, copyCount, nToRead, nRead);
            goto CopyTest_exit;
        }

        fr = f_write(&dstFil, buffer, nRead, &nWrite);
        if (fr != FR_OK || nWrite != nRead)
        {
            printf("CopyTest has failed! Error on writing '%s' (fr=%i, copyCount=%lu, nToWrite=%lu, nWrite=%lu)\n", dstFilepath, fr, copyCount, nRead, nWrite);
            goto CopyTest_exit;
        }

        copyCount += nWrite;

        tud_task(); // tinyusb device task
        cdc_task();
    }

    printf("CopyTest done.");

CopyTest_exit:
    f_close(&srcFil);
    f_close(&dstFil);
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

    CopyTest("/copytest_source.bin", "/copytest_target.bin");

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

    measure_freqs();

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
