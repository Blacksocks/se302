#include "rtt.h"
#include "SEGGER_RTT.h"

// Channel put method, blocking
msg_t rtt_put(void *instance, uint8_t b)
{
    (void)instance;
    SEGGER_RTT_Write(0, &b, 1);
    return Q_OK;
}

// Channel get method, blocking
msg_t rtt_get(void *instance)
{
    (void)instance;
    int r;
    do {
        r = SEGGER_RTT_GetKey();
        chThdSleepMilliseconds(10);
    } while (r < 0);
    return r;
}

// Stream write buffer method
size_t rtt_write(void *instance, const uint8_t *bp, size_t n)
{
    (void)instance;
    return SEGGER_RTT_Write(0, bp, n);
}

// Stream read buffer method, blocking
size_t rtt_read(void *instance, uint8_t *bp, size_t n)
{
    uint8_t i = 0;
    while(i < n)
        bp[i++] = rtt_get(instance);
    return n;
}

static const struct BaseSequentialStreamVMT rtt_vmt = {
    rtt_write,
    rtt_read,
    rtt_put,
    rtt_get
};


BaseSequentialStream RTT_stream = {
    &rtt_vmt
};
