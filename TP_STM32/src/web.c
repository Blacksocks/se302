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
#define WEB_ADDR                "www.telecom-paristech.fr"

static uint8_t mac_address[6] = {0x80, 0xfa, 0x5b, 0x3d, 0xe8, 0x5b};
static struct netconn *conn;
static ip_addr_t ip_server;

static const lwipthread_opts_t lwip_opts = {
    mac_address,
    ADDR4_2_U32(137,194,66,61),
    ADDR4_2_U32(255,255,252,0),
    ADDR4_2_U32(137,194,64,254)
};

static struct netbuf *inbuf;
static const char http_request[] = "GET / HTTP/1.0\r\nHost: www.telecom-paristech.fr\r\n\r\n";

static void init(void)
{
    // Init lwIP
    lwipInit(&lwip_opts);
    // Init DNS
    ip_addr_t ip_dns = {DNS_IP};
    dns_init();
    dns_setserver(0, &ip_dns);
}

THD_FUNCTION(webThread, arg)
{
    (void)arg;
    err_t err;

    init();

    // Get server IP using DNS
    err = netconn_gethostbyname(WEB_ADDR, &ip_server);
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

    // Connect to telecom server
    err = netconn_connect(conn, &ip_server, WEB_PORT);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] Connection error: %d\r\n", err);
        return;
    }
    chprintf(RTT1, "[INFO] [WEB] Connection to "WEB_ADDR": Success\r\n");

    // Get data from server
    // subtract 1 from the size, since we dont send the \0 in the string
    // NETCONN_NOCOPY: our data is const static, so no need to copy it
    err = netconn_write(conn, http_request, sizeof(http_request)-1, NETCONN_NOCOPY);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] Request error: %d\r\n", err);
        return;
    }

    // Wait for data reception
    err = netconn_recv(conn, &inbuf);
    if(err != ERR_OK) {
        chprintf(RTT1, "[ERROR] [WEB] Receive data error: %d\r\n", err);
        return;
    }

    // Read data
    char *buf;
    u16_t buflen;
    netbuf_data(inbuf, (void **)&buf, &buflen);
    chprintf(RTT1, "[INFO] [WEB] Data: %s\r\n", buf);

    netconn_close(conn);
    netbuf_delete(inbuf);
}
