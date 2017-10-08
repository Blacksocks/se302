#include "button.h"
#include "hal.h"
#include "event.h"
#include "chprintf.h"
#include "rtt.h"

#define GET_PUSHBTN()     palReadPad(GPIOA, GPIOA_BUTTON_WKUP)

static virtual_timer_t btn_timer;
static unsigned int btn_prev_value = PAL_LOW;

static void btn_event_callback(void * p)
{
    (void)p;
    chSysLockFromISR();
    // Handle push button
    unsigned int btn_curr_value = GET_PUSHBTN();
    if(GET_PUSHBTN() != btn_prev_value) {
        if(btn_curr_value == PAL_LOW)
            chEvtBroadcastI(&e_btn_up);
        else
            chEvtBroadcastI(&e_btn_down);
        btn_prev_value = btn_curr_value;
    }
    // Restart timer
    chVTSetI(&btn_timer, MS2ST(EVENT_REFRESH_TIME), btn_event_callback, NULL);
    chSysUnlockFromISR();
}

void init_btn(void)
{
    // Event timer
    // Read btn value each period of time
    // and broadcast an event if button has been pressed or released
    chVTObjectInit(&btn_timer);
    chVTSet(&btn_timer, MS2ST(EVENT_REFRESH_TIME), btn_event_callback, NULL);
}

void btn_up_handler(eventid_t id)
{
    (void)id;
    chprintf((BaseSequentialStream *)&RTT_stream, "[INFO] btn up\r\n");
}

void btn_down_handler(eventid_t id)
{
    (void)id;
    chprintf((BaseSequentialStream *)&RTT_stream, "[INFO] btn down\r\n");
}
