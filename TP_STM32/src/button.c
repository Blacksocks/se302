#include "button.h"
#include "hal.h"
#include "chprintf.h"
#include "rtt.h"

#define GET_PUSHBTN()     palReadPad(GPIOA, GPIOA_BUTTON_WKUP)
#define KICKBACK_DELAY    5

// Used to remove kickback
static int ready = 1;
static virtual_timer_t kickback_timer;

static void btn_up_handler(void)
{
    chprintf((BaseSequentialStream *)&RTT_stream, "[INFO] btn up\r\n");
}

static void btn_down_handler(void)
{
    chprintf((BaseSequentialStream *)&RTT_stream, "[INFO] btn down\r\n");
}

/* Triggered when the button is pressed or released without kickback
*/
static void btn_callback(void * p)
{
    (void)p;
    if(GET_PUSHBTN() == PAL_LOW)
        btn_up_handler();
    else
        btn_down_handler();
    ready = 1;
}

/* Triggered when the button is pressed or released
*/
static void btn_raw_handler(EXTDriver *extp, expchannel_t channel)
{
    (void)extp;
    (void)channel;
    if(ready) {
        chSysLockFromISR();
        chVTSetI(&kickback_timer, MS2ST(KICKBACK_DELAY), btn_callback, NULL);
        chSysUnlockFromISR();
        ready = 0;
    }
}

static const EXTConfig extcfg = {
  {
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
  }
};

void init_btn(void)
{
    // Start external driver to link pushbutton events to callbacks
    extStart(&EXTD1, &extcfg);
}
