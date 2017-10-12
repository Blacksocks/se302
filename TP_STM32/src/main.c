#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "led.h"
#include "usbcfg.h"
#include "shell.h"
#include "shellcfg.h"
#include "rtt.h"
#include "button.h"
#include "event.h"
#include "web.h"
#include "lwipthread.h"

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

    // Init Push Button
    init_btn();

    // Create event handler thread
    chThdCreateStatic(waListenerThread, sizeof(waListenerThread), NORMALPRIO + 1, listenerThread, NULL);

    // Create Web thread
    chThdCreateStatic(waWebThread, sizeof(waWebThread), NORMALPRIO + 1, webThread, NULL);

    while(1) {
        shell = chThdCreateFromHeap(NULL, THD_WORKING_AREA_SIZE(2048), "shell", NORMALPRIO + 1, shellThread, (void *)&shell_cfg1);
        chThdWait(shell);
        chThdSleepMilliseconds(1000);
    }
}
