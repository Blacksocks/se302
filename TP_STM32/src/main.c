#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "led.h"
#include "usbcfg.h"
#include "shell.h"
#include "shellcfg.h"

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

    int input;

    while(1)
    {
        // This timeout is needed to find when USB init is finished
        input = chnGetTimeout(&SDU1, MS2ST(3000));
        if (input == Q_TIMEOUT)
            // Run shell
            shellThread(&shell_cfg1);
        chThdSleepMilliseconds(100);
    }
}
