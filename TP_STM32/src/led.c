#include "ch.h"
#include "hal.h"
#include "led.h"

static int led_value = 0;

void init_led(void)
{
    palSetPadMode(GPIOC, GPIOC_LED, PAL_MODE_OUTPUT_PUSHPULL);
    led_value = 0;
}

void led_on(void)
{
    palClearPad(GPIOC, GPIOC_LED);
    led_value = 1;
}

void led_off(void)
{
    palSetPad(GPIOC, GPIOC_LED);
    led_value = 0;
}

void led_toogle(void)
{
    if(led_value)
        led_off();
    else
        led_on();
}
