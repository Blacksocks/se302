#include "button.h"
#include "hal.h"
#include "chprintf.h"
#include "rtt.h"

#define GET_PUSHBTN()     palReadPad(GPIOA, GPIOA_BUTTON_WKUP)

static void btn_up_handler(void)
{
    chprintf((BaseSequentialStream *)&RTT_stream, "[INFO] btn up\r\n");
}

static void btn_down_handler(void)
{
    chprintf((BaseSequentialStream *)&RTT_stream, "[INFO] btn down\r\n");
}

static void btn_handler(eventid_t id)
{
    (void)id;
    if(GET_PUSHBTN() == PAL_LOW)
        btn_up_handler();
    else
        btn_down_handler();
}

void init_btn(void)
{
    // Link handler
    palPadEnableEventI(GPIOA, GPIOA_BUTTON_WKUP, PAL_MODE_INPUT, btn_handler);
}
