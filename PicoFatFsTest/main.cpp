#include <stdio.h>
#include "pico/stdlib.h"

#include "usb_msc_callbacks.h"
#include "fatfs_flash.h"
#include "fatfs_disk.h"
#include "ff.h"

#include "bsp/board.h"
#include "tusb.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"

#include <algorithm>

extern bool UsbConnected;

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

    printf(">> Frequencies :\n");
    printf(">> \tpll_sys  = %dkHz\n", f_pll_sys);
    printf(">> \tpll_usb  = %dkHz\n", f_pll_usb);
    printf(">> \trosc     = %dkHz\n", f_rosc);
    printf(">> \tclk_sys  = %dkHz\n", f_clk_sys);
    printf(">> \tclk_peri = %dkHz\n", f_clk_peri);
    printf(">> \tclk_usb  = %dkHz\n", f_clk_usb);
    printf(">> \tclk_adc  = %dkHz\n", f_clk_adc);
#ifdef CLOCKS_FC0_SRC_VALUE_CLK_RTC
    printf(">> \tclk_rtc  = %dkHz\n", f_clk_rtc);
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

    // printf(">> Led state : %s\n", led_state ? "on" : "off");
    board_led_write(led_state);
    led_state = !led_state;
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
void CopyTest(const TCHAR* srcFilepath, const TCHAR* dstFilepath)
{
    const UINT BufferSize{256};
    uint8_t buffer[BufferSize];
    FRESULT fr;
    FIL srcFil, dstFil;

    printf("\n>> CopyTest: copy '%s' to '%s'\n", srcFilepath, dstFilepath);

    FILINFO fno;

    if ((fr = f_stat(dstFilepath, &fno)) == FR_OK)
    {
        printf(">> \tTarget file '%s' exists. Deleting target file.\n", dstFilepath);
        if (f_unlink(dstFilepath) != FR_OK)
        {
            printf(">> \tCopyTest has failed! Error deleting target file.\n");
            return;
        }
    }

    tud_task(); // tinyusb device task
    cdc_task();

    if ((fr = f_stat(srcFilepath, &fno)) != FR_OK)
    {
        printf(">> \tCopyTest has failed! Error on f_stat('%s') (fr=%i)\n", srcFilepath, fr);
        return;
    }

    tud_task(); // tinyusb device task
    cdc_task();

    if ((fr = f_open(&srcFil, srcFilepath, FA_OPEN_EXISTING | FA_READ)) != FR_OK)
    {
        printf(">> \tCopyTest has failed! Error on opening '%s' (fr=%i)\n", srcFilepath, fr);
        return;
    }

    if ((fr = f_open(&dstFil, dstFilepath, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
    {
        printf(">> \tCopyTest has failed! Error on opening '%s' (fr=%i)\n", dstFilepath, fr);
        f_close(&srcFil);
        return;
    }

    tud_task(); // tinyusb device task
    cdc_task();

    FSIZE_t copyCount{0};
    while (copyCount < fno.fsize)
    {
        UINT nToRead{(UINT)std::min((FSIZE_t)BufferSize, fno.fsize - copyCount)};
        UINT nRead{0}, nWrite{0};
        fr = f_read(&srcFil, buffer, nToRead, &nRead);
        if (fr != FR_OK || nRead != nToRead)
        {
            printf(">> \tCopyTest has failed! Error on reading '%s' (fr=%i, copyCount=%llu, nToRead=%u, nRead=%u)\n", dstFilepath, fr, copyCount, nToRead, nRead);
            goto CopyTest_exit;
        }

        tud_task(); // tinyusb device task
        cdc_task();

        fr = f_write(&dstFil, buffer, nRead, &nWrite);
        if (fr != FR_OK || nWrite != nRead)
        {
            printf(">> \tCopyTest has failed! Error on writing '%s' (fr=%i, copyCount=%llu, nToWrite=%u, nWrite=%u)\n", dstFilepath, fr, copyCount, nRead, nWrite);
            goto CopyTest_exit;
        }

        copyCount += nWrite;


        tud_task(); // tinyusb device task
        cdc_task();
    }

    printf(">> \tCopyTest done : %llu bytes are copied (source file size:%llu).\n", copyCount, fno.fsize);

CopyTest_exit:
    f_close(&dstFil);
    f_close(&srcFil);
}

void TestCreateAndWriteFile(const TCHAR* filepath, const TCHAR* text)
{
    FIL fil;

    f_open(&fil, filepath, FA_CREATE_NEW | FA_WRITE);
    f_puts(text, &fil);
    f_close(&fil);
}

//--------------------------------------------------------------------+
//
//--------------------------------------------------------------------+
void TestSample(const TCHAR* drive)
{
    printf("\n>> Executing... TestSample() on drive '%s'!\n", drive);

    flash_init();

    printf(">> \tf_mount(&fs, \"%s\", 1)\n", drive);
    FATFS fs;
    if (f_mount(&fs, drive, 1) != FR_OK)
    {
        printf(">> \tError: mount has failed!\n");
        return;
    }

    TCHAR fpath1[FF_MAX_LFN];
    TCHAR fpath2[FF_MAX_LFN];
    TCHAR str[FF_MAX_LFN+3];
    FRESULT fr;

    sprintf(fpath1, "%s/test1.txt", drive);
    sprintf(str, "%s...", fpath1);
    TestCreateAndWriteFile(fpath1, str);

    sprintf(fpath1, "%s/test_folder", drive);
    fr = f_mkdir(fpath1);
    if (fr == FR_OK)
    {
        sprintf(fpath2, "%s/test_folder/test_in_test_folder.txt", drive);
        sprintf(str, "%s...", fpath2);
        TestCreateAndWriteFile(fpath2, str);
    }
    else
    {
        printf(">> \tf_mkdir(\"%s\") has failed ! (err=%i)\n", fpath1, fr);
    }

    fr = f_chdir(fpath1);
    if (fr == FR_OK)
    {
        fr = f_getcwd(fpath1, FF_MAX_LFN);
        printf(">> \tcwd=%s\n", fpath1); // OUPUT: cwd=flash:/
    }
    else
    {
        printf(">> \tf_chdir(\"%s\") has failed ! (err=%i)\n", fpath1, fr);
    }

    sprintf(fpath1, "%s/copytest_source.bin", drive);
    sprintf(fpath2, "%s/copytest_target.bin", drive);
    CopyTest(fpath1, fpath2);

    f_mount(0, drive, 0);
}

enum class ETestStep
{
    WaitUsbConnected,
    Flash,
    SD,
    done
};

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
    FATFS filesystem;
    if (f_mount(&filesystem, "", 1) != FR_OK)
    {
        printf("\tCreating FAT Filesystem disk\n");
        create_builtin_flash_drive();
        if (f_mount(&filesystem, "", 1) != FR_OK)
        {
            printf("\tError : FAT Filesystem disk creation has failed !\n");
        }
    }


    ETestStep testStep{ETestStep::WaitUsbConnected};

    while (true)
    {
        tud_task(); // tinyusb device task
        cdc_task();

        switch (testStep)
        {
        case ETestStep::WaitUsbConnected:
            if (UsbConnected)
                testStep = ETestStep::Flash;
            break;

        case ETestStep::Flash:
            TestSample("FLASH:");
            testStep = ETestStep::SD;
            break;

        case ETestStep::SD:
            TestSample("SD:");
            testStep = ETestStep::done;
            break;

        default:
            break;
        }

        blink_led_task();
    }
}

/* Implement library message callbacks */
void put_out_error_message(const char *s)
{
    printf("*** Error:%s\n", s);
}
void put_out_info_message(const char *s)
{
    printf("*** Info:%s\n", s);
}
// This will not be called unless build_flags include "-D USE_DBG_PRINTF":
void put_out_debug_message(const char *s)
{
    printf("*** Debug:%s\n", s);
}
