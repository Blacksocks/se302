#ifndef _EVENT_H_
#define _EVENT_H_

#include "ch.h"

#define EVENT_REFRESH_TIME 10

/* Event thread
** Handle events like GPIO inputs
*/
THD_FUNCTION(listenerThread, arg);

#endif // _EVENT_H_
