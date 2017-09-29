#include "ch.h"
#include "hal.h"
#include "led.h"

static virtual_timer_t led_timer;

void pwm_callback(__attribute__((unused)) PWMDriver *pwmp)
{
    led_toggle();
}

static PWMConfig pwmcfg = {
    1024,                           // frequency
    1024,                           // PWM period
    pwm_callback,                   // callback called each period
   {{PWM_OUTPUT_ACTIVE_HIGH, NULL}, // first channel init
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL}},
    0,                              // TIM CR2 register init data
    0                               // TIM DIER register init data
};


void led_callback(__attribute__((unused)) void * p)
{
    // Restart timer
    chSysLockFromISR();
    chVTSetI(&led_timer, MS2ST(500), led_callback, NULL);
    chSysUnlockFromISR();
    // Toggle LED
    //led_toggle();
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

    pwmStart(&PWMD3, &pwmcfg);
    pwmEnableChannel(&PWMD3, 0, 512);

    while(1)
        chThdSleep(TIME_INFINITE);
}
