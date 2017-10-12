#ifndef _LWIP_H_
#define _LWIP_H_

#include "lwipthread.h"

// Default is LITTLE_ENDIAN
#define ADDR4_2_U32(a, b, c, d) (((u32_t)((d) & 0xff) << 24) | \
                                 ((u32_t)((c) & 0xff) << 16) | \
                                 ((u32_t)((b) & 0xff) << 8)  | \
                                  (u32_t)((a) & 0xff))

static uint8_t mac_address[6] = {0x80, 0xfa, 0x5b, 0x3d, 0xe8, 0x5b};

const lwipthread_opts_t lwip_opts = {
    mac_address,
    ADDR4_2_U32(137,194,66,61),
    ADDR4_2_U32(255,255,252,0),
    ADDR4_2_U32(137,194,64,254)
};

#endif // _LWIP_H_
