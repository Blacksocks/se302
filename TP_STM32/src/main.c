#include "ch.h"
#include "hal.h"

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

    // set LED on GPIO C as output using pushpull mode
    // pushpull means that output is either connect to VCC or to GND, no floating state
    palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL);
    // Switch off LED on GPIO C
    palClearPad(GPIOC, GPIOC_LED);

    while(1);
}
