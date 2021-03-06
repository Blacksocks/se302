#include "event.h"
#include "button.h"

static const evhandler_t evhndl[] = {
    btn_up_handler,
    btn_down_handler,
    btn_click_handler
};

THD_FUNCTION(listenerThread, arg)
{
    (void)arg;
    // Init events
    chEvtObjectInit(&e_btn_up);
    chEvtObjectInit(&e_btn_down);
    chEvtObjectInit(&e_btn_click);
    // Link each event to its listener
    chEvtRegister(&e_btn_up, &btn_up_listener, 0);
    chEvtRegister(&e_btn_down, &btn_down_listener, 1);
    chEvtRegister(&e_btn_click, &btn_click_listener, 2);
    while(1) {
        // Wait for any of the events we're registered on and call its handler
        chEvtDispatch(evhndl, chEvtWaitAny(ALL_EVENTS));
        chThdSleepMilliseconds(10);
    }
}
