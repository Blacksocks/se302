#include "ch.h"
#include "hal.h"
#include "led.h"

static virtual_timer_t led_timer;

void led_callback(__attribute__((unused)) void * p)
{
    // Restart timer
    chSysLockFromISR();
    chVTSetI(&led_timer, MS2ST(500), led_callback, NULL);
    chSysUnlockFromISR();
    // Toggle LED
    led_toggle();
}

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

    // Configure LED
    led_init();

    // Init timer
    chVTObjectInit(&led_timer);
    // Start timer
    chVTSet(&led_timer, MS2ST(500), led_callback, NULL);

    while(1);
}
