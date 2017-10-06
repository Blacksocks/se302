#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "led.h"
#include "usbcfg.h"
#include "shell.h"
#include "shellcfg.h"
#include "SEGGER_RTT.h"

int main(void)
{
    /*
     * System initializations.
     * - HAL initialization, this also init512ializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the RTOS is active.
     */
    halInit();
    chSysInit();

    // Init USB communication
    usb_init();

    // Configure Shell
    shellInit();

    // Configure LED
    led_init();

    while(1)
    {
        SEGGER_RTT_WriteString(0, "Hello World from SEGGER!\r\n");
        chThdSleepMilliseconds(1000);
    }
}
