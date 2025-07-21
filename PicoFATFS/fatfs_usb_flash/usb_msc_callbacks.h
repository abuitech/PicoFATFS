#ifndef USB_MSC_CALLBACKS_H
#define USB_MSC_CALLBACKS_H


#include <stdio.h>
#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void cdc_task(void);

#ifdef __cplusplus
}
#endif


#endif // USB_MSC_CALLBACKS_H