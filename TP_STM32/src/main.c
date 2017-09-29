#include "ch.h"
#include "hal.h"
#include "led.h"

int main(void)
{
    /*
     * System initializations.
     * - HAL initialization, this also initializes the configured device drivers
     *   and performs the board-specific initializations.
     * - Kernel initialization, the main() function becomes a thread and the RTOS is active.
     */
    halInit();
    chSysInit();

    // configure LED
    init_led();
    // switch on LED
    led_on();

    while(1);
}
