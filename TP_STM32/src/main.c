#include "ch.h"
#include "hal.h"
#include "led.h"
#include "usbcfg.h"

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

    // Initialize a serial over USB driver
    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg);
    usbStart(serusbcfg.usbp, &usbcfg);

    usbDisconnectBus(serusbcfg.usbp);
    chThdSleepMilliseconds(1500);
    usbStart(serusbcfg.usbp, &usbcfg);
    usbConnectBus(serusbcfg.usbp);

    // Configure Shell
    //shellInit();

    // Configure LED
    led_init();
    led_pwm(10);
    led_blink(1000);

    while(1)
        chThdSleep(TIME_INFINITE);
}
