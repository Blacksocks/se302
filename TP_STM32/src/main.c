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

    // Configure GPIO ports for USB
    // PF11 and PB1 must be configured as input floating but it's by default
    palSetPadMode(GPIOA, GPIOA_USB_HS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    palSetPadMode(GPIOA, GPIOA_OTG_FS_VBUS, PAL_MODE_INPUT_PULLDOWN);
    palSetPadMode(GPIOA, GPIOA_OTG_FS_ID, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DM, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOA, GPIOA_OTG_FS_DP, PAL_MODE_ALTERNATE(10));
    palSetPadMode(GPIOB, GPIOB_USB_FS_BUSON, PAL_MODE_OUTPUT_PUSHPULL);
    // Initialize a serial over USB driver
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    // Start serial connection
    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

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
