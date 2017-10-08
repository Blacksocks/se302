#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "ch.h"

/* Button initialization
*/
void init_btn(void);

/* Button events handlers
*/
void btn_up_handler(eventid_t id);
void btn_down_handler(eventid_t id);

/* Events and listeners
*/
event_source_t   e_btn_up,
                 e_btn_down;
event_listener_t btn_up_listener,
                 btn_down_listener;

#endif // _BUTTON_H_
