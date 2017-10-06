#ifndef _RTT_H_
#define _RTT_H_

#include "hal.h"

extern BaseSequentialStream RTT_stream;

msg_t rtt_put(void *instance, uint8_t b);
msg_t rtt_get(void *instance);
size_t rtt_write(void *instance, const uint8_t *bp, size_t n);
size_t rtt_read(void *instance, uint8_t *bp, size_t n);

#endif // _RTT_H_
