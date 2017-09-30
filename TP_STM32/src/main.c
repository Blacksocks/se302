#include "ch.h"
#include "hal.h"
#include "led.h"

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

    // Configure LED
    led_init();
    led_pwm(10);
    led_blink(1000);

    while(1)
        chThdSleep(TIME_INFINITE);
}
