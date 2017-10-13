#ifndef _WEB_H_
#define _WEB_H_

#include "lwipthread.h"

/* Thread working area
** Allocate a static thread working area aligned as both position and size
*/
THD_WORKING_AREA(waWebThread, 1024);

/* Web thread
*/
THD_FUNCTION(webThread, arg);

#endif // _WEB_H_
