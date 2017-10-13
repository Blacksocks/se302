#include "button.h"
#include "hal.h"
#include "led.h"
#include "chprintf.h"
#include "rtt.h"
#include "event.h"

#define GET_PUSHBTN()     palReadPad(GPIOA, GPIOA_BUTTON_WKUP)
#define KICKBACK_DELAY    5
#define BTN_CLICK_DELAY   200

// Used to remove kickback
static int ready = 1;
static virtual_timer_t kickback_timer;
// Used to handle button click
static int click_ready = 1;
static virtual_timer_t click_timer;
// Led control
static const int led_delay = 20;
// led_state:
// 0: intensity increasing
// 1: intensity decreasing
static int led_state = 0;
static virtual_timer_t led_timer;
static int led_value = 0;

/* ==================================================
** Top-level handlers
** Actions done when button is pressed, released or clicked
** Basically control a LED intensity
** Clicking the button set intensity at 1% or 100% (almost alternate)
** Pressing the button makes the intensity slowly increasing until maximum, then decreasing, ...
** ================================================== */

static void led_callback(void * p)
{
    (void)p;
    chSysLockFromISR();
    chVTSetI(&led_timer, MS2ST(led_delay), led_callback, NULL);
    chSysUnlockFromISR();
    if(led_state) {
        if(--led_value < 1) {
            led_value = 2;
            led_state = 0;
        }
    }
    else {
        if(++led_value > 100) {
            led_value = 99;
            led_state = 1;
        }
    }
    led_pwmI(led_value);
}

void btn_up_handler(eventid_t id)
{
    (void)id;
    chprintf(RTT1, "[INFO] btn up\r\n");
    chVTReset(&led_timer);
}

void btn_down_handler(eventid_t id)
{
    (void)id;
    chprintf(RTT1, "[INFO] btn down\r\n");
    chVTSet(&led_timer, MS2ST(led_delay), led_callback, NULL);
}

void btn_click_handler(eventid_t id)
{
    (void)id;
    chprintf(RTT1, "[INFO] btn click\r\n");
    if(led_value == 1 || led_value == 100)
        led_state = 1 - led_state;
    if(led_state)
        led_value = 100;
    else
        led_value = 1;
    led_pwm(led_value);
}

/* ==================================================
** Intermediate callbacks
** Used to handle three cases
** * Button Pressed
** * Button Released
** * Button Clicked
** ================================================== */

/* Triggered when the button is pressed or released without kickback
*/
static void click_callback(void * p)
{
    (void)p;
    chSysLockFromISR();
    if(GET_PUSHBTN() == PAL_LOW) {
        if(click_ready)
            chEvtBroadcastI(&e_btn_up);
        else
            chEvtBroadcastI(&e_btn_click);
    }
    else
        chEvtBroadcastI(&e_btn_down);
    chSysUnlockFromISR();
    click_ready = 1;
}

/* Triggered when the button is pressed or released without kickback
*/
static void btn_callback(void * p)
{
    (void)p;
    // Handle three events; button pressed, released or clicked
    if(click_ready && GET_PUSHBTN() == PAL_HIGH) {
        // Button pressed
        click_ready = 0;
        chSysLockFromISR();
        chVTSetI(&click_timer, MS2ST(BTN_CLICK_DELAY), click_callback, NULL);
        chSysUnlockFromISR();
    }
    else if(GET_PUSHBTN() == PAL_LOW) {
        // Button released
        chSysLockFromISR();
        chVTResetI(&click_timer);                                                        \
        chSysUnlockFromISR();
        click_callback(NULL);
    }
    ready = 1;
}

/* Triggered when the button is pressed or released
*/
static void btn_raw_handler(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;
    // Hide all button event until kickback delay is not reached
    // Therefore, only one button callback function is called when button is pressed or released
    if(ready) {
        chSysLockFromISR();
        chVTSetI(&kickback_timer, MS2ST(KICKBACK_DELAY), btn_callback, NULL);
        chSysUnlockFromISR();
        ready = 0;
    }
}

static const EXTConfig extcfg = {{
    {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART | EXT_MODE_GPIOA, btn_raw_handler},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
}};

/* ==================================================
** Init button event handler using external driver
** ================================================== */

void init_btn(void)
{
    // Start external driver to link pushbutton events to callbacks
    extStart(&EXTD1, &extcfg);
    led_on();
    led_pwm(1);
}
