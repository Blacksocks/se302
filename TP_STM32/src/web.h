#ifndef _WEB_H_
#define _WEB_H_

#include "lwipthread.h"

const lwipthread_opts_t lwip_opts;

/* Thread working area
** Allocate a static thread working area aligned as both position and size
*/
THD_WORKING_AREA(waWebThread, 128);

/* Web thread
*/
THD_FUNCTION(webThread, arg);

#endif // _WEB_H_
