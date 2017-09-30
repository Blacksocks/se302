#include "ch.h"
#include "hal.h"
#include "led.h"

static void pwm_callback(__attribute__((unused)) PWMDriver *pwmp)
{
    led_on();
}

static void channel_callback(__attribute__((unused)) PWMDriver *pwmp)
{
    led_off();
}

/* Configure a pwm at 120Hz
** It's a common value in order to be confortable for eyes
** 12000 was choosen for frequency because this assertions must be verified:
** clk / frequency - 1 < 0xFFFF, with clk = 84000000, i.e frequency > 1282Hz
** Moreover, I choosed frequency which verify 120*10^x for clarity
*/
static PWMConfig pwmcfg = {
    12000,                          // frequency
    100,                            // PWM period
    pwm_callback,                   // callback called each period
   {{PWM_OUTPUT_DISABLED, channel_callback}, // first channel init, material output disabled but callback called each rising edge
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL},
    {PWM_OUTPUT_DISABLED, NULL}},
    0,                              // TIM CR2 register init data
    0                               // TIM DIER register init data
};

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

    pwmStart(&PWMD3, &pwmcfg);
    // Set PWM duty cycle
    pwmEnableChannel(&PWMD3, 0, 20); // 20% of PWM period
    // Enable channel callbacks
    pwmEnableChannelNotification(&PWMD3, 0);
    // Enable PWM callbacks
    pwmEnablePeriodicNotification(&PWMD3);

    while(1)
        chThdSleep(TIME_INFINITE);
}
