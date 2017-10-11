#ifndef _EVENT_H_
#define _EVENT_H_

#include "ch.h"

#define EVENT_REFRESH_TIME 10

/* Thread working area
** Allocate a static thread working area aligned as both position and size
*/
THD_WORKING_AREA(waListenerThread, 128);

/* Event thread
** Handle events like GPIO inputs
*/
THD_FUNCTION(listenerThread, arg);

#endif // _EVENT_H_
