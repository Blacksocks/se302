#include "web.h"
#include "chprintf.h"
#include "rtt.h"
#include "lwip/api.h"
#include "lwip/dns.h"

#define WEB_PORT                80
// Default is LITTLE_ENDIAN
#define ADDR4_2_U32(a, b, c, d) (((u32_t)((d) & 0xff) << 24) | \
                                 ((u32_t)((c) & 0xff) << 16) | \
                                 ((u32_t)((b) & 0xff) << 8)  | \
                                  (u32_t)((a) & 0xff))
#define DNS_IP                  ADDR4_2_U32(137,194,2,34)

static uint8_t mac_address[6] = {0x80, 0xfa, 0x5b, 0x3d, 0xe8, 0x5b};

static const lwipthread_opts_t lwip_opts = {
    mac_address,
    ADDR4_2_U32(137,194,66,61),
    ADDR4_2_U32(255,255,252,0),
    ADDR4_2_U32(137,194,64,254)
};

static struct netbuf *inbuf;

THD_FUNCTION(webThread, arg)
{
    (void)arg;
    err_t err;
    struct netconn *conn;
    ip_addr_t ip_server;
    ip_addr_t ip_dns;

    lwipInit(&lwip_opts);

    // Init DNS
    dns_init();
    ip_dns.addr = DNS_IP;
    dns_setserver(0, &ip_dns);
    err = netconn_gethostbyname("www.telecom-paristech.fr", &ip_server);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] DNS error: %d\r\n", err);
        return;
    }

    // Create TCP connection
    conn = netconn_new(NETCONN_TCP);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), chThdExit(MSG_RESET););
    // Bind to port 80 (HTTP) with default IP address
    err = netconn_bind(conn, NULL, WEB_PORT);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] Binding error: %d\r\n", err);
        return;
    }
    chprintf(RTT1, "[INFO] [WEB] Binding Success\r\n");

    // Connect to telecom server
    //ip_server.addr = ADDR4_2_U32(137,194,52,7);
    err = netconn_connect(conn, &ip_server, WEB_PORT);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] Connection error: %d\r\n", err);
        return;
    }
    chprintf(RTT1, "[INFO] [WEB] Connection Success\r\n");

    // Get data from server
    err = netconn_recv(conn, &inbuf);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] Receive data error: %d\r\n", err);
        return;
    }
    chprintf(RTT1, "[INFO] [WEB] Success\r\n");
}
