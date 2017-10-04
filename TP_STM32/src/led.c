#include "ch.h"
#include "hal.h"
#include "led.h"

/* Macros used to switch on/off, toggle and init LED
*/
#define LED_ON()      palClearPad(GPIOC, GPIOC_LED)
#define LED_OFF()     palSetPad(GPIOC, GPIOC_LED)
#define LED_TOGGLE()  palTogglePad(GPIOC, GPIOC_LED)
#define LED_SET_PIN() palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL)

// Blinking period
static int period = 1000;

/* State of the LED;
** 0: off
** 1: on
*/
static int state = 0;

static virtual_timer_t led_timer;

/* PWM callbacks functions
*/
static void pwm_callback(__attribute__((unused)) PWMDriver *pwmp)
{
    if(state)
        LED_ON();
}

static void channel_callback(__attribute__((unused)) PWMDriver *pwmp)
{
    LED_OFF();
}

/* LED blinking callback
*/
static void led_blink_callback(__attribute__((unused)) void * p)
{
    // Restart timer
    chSysLockFromISR();
    chVTSetI(&led_timer, MS2ST(period), led_blink_callback, NULL);
    chSysUnlockFromISR();
    // Toggle LED
    led_toggle();
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

void led_init(void)
{
    LED_SET_PIN();
    led_off();
    // Init timer
    chVTObjectInit(&led_timer);
    // Start timer
    chVTSet(&led_timer, MS2ST(period), led_blink_callback, NULL);
}

void led_on(void)
{
    LED_ON();
    state = 1;
}

void led_off(void)
{
    LED_OFF();
    state = 0;
}

void led_toggle(void)
{
    LED_TOGGLE();
    state = 1 - state;
}

int led_pwm(unsigned int intensity)
{
    if(intensity > 100)
        return 1;
    pwmStart(&PWMD3, &pwmcfg);
    // Set PWM duty cycle
    pwmEnableChannel(&PWMD3, 0, intensity);
    // Enable channel callbacks
    pwmEnableChannelNotification(&PWMD3, 0);
    // Enable PWM callbacks
    pwmEnablePeriodicNotification(&PWMD3);
    return 0;
}

int led_blink(unsigned int m_period)
{
    if(m_period < 10 || m_period > 10000)
        return 1;
    period = m_period / 2;
    return 0;
}
