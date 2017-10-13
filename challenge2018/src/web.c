#include <string.h>
#include "web.h"
#include "chprintf.h"
#include "rtt.h"
#include "lwip/api.h"
#include "lwip/dns.h"
#include "usbcfg.h"

#define WEB_PORT                80
// Default is LITTLE_ENDIAN
#define ADDR4_2_U32(a, b, c, d) (((u32_t)((d) & 0xff) << 24) | \
                                 ((u32_t)((c) & 0xff) << 16) | \
                                 ((u32_t)((b) & 0xff) << 8)  | \
                                  (u32_t)((a) & 0xff))
#define DNS_IP                  ADDR4_2_U32(137,194,2,34)
#define WEB_ADDR                "antinea.enst.fr"

static uint8_t mac_address[6] = {0x80, 0xfa, 0x5b, 0x3d, 0xe8, 0x5b};
static struct netconn *conn;
static ip_addr_t ip_server;

static const lwipthread_opts_t lwip_opts = {
    mac_address,
    ADDR4_2_U32(137,194,66,61),
    ADDR4_2_U32(255,255,252,0),
    ADDR4_2_U32(137,194,64,254)
};

static char http_request[512] = "";
static char response[TCP_MSS];
static const char * http_rqst_head = "GET ";
static const char * http_rqst_foot = " HTTP/1.0\r\nHost: antinea.enst.fr\r\n\r\n";

static void request(void)
{
    err_t err;
    struct netbuf *inbuf;

    // Create TCP connection
    conn = netconn_new(NETCONN_TCP);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), chThdExit(MSG_RESET););

    // Connect to telecom server
    err = netconn_connect(conn, &ip_server, WEB_PORT);
    if(err != ERR_OK) {
        chprintf(SDU, "[ERROR] [WEB] Connection error: %d\r\n", err);
        return;
    }

    // Get data from server
    // subtract 1 from the size, since we dont send the \0 in the string
    // NETCONN_NOCOPY: our data is const static, so no need to copy it
    err = netconn_write(conn, (const char *)http_request, strlen(http_request), NETCONN_NOCOPY);
    if(err != ERR_OK) {
        chprintf(SDU, "[ERROR] [WEB] Request error: %d\r\n", err);
        return;
    }

    // Wait for data reception
    err = netconn_recv(conn, &inbuf);
    if(err != ERR_OK) {
        chprintf(SDU, "[ERROR] [WEB] Receive data error: %d\r\n", err);
        return;
    }

    // Read data
    char *buf;
    u16_t buflen;
    netbuf_data(inbuf, (void **)&buf, &buflen);
    strcpy(response, buf);
    response[buflen] = '\0';
    chprintf(SDU, "[INFO] [WEB] Data (%d): %s\r\n", buflen, response);

    // Close and delete the connection
    netconn_close(conn);
    netconn_delete(conn);

    // Delete the buffer (netconn_recv gives us ownership,
    //so we have to make sure to deallocate the buffer)
    netbuf_delete(inbuf);
}

void request_from_url(char * url)
{
    int rqst_idx = strlen(http_rqst_head);
    strcpy(http_request, http_rqst_head);
    int start_cpy = 0;
    for(unsigned int i = 3; i < strlen(url); i++) {
        if(url[i] == '/' && url[i-1] == 'r' && url[i-2] == 'f' && url[i-3] == '.')
            start_cpy = 1;
        if(start_cpy == 1) {
            if(url[i] > 0x7F || url[i] == '\n' || url[i] == '\r' || url[i] == '\0')
                break;
            http_request[rqst_idx++] = url[i];
        }
    }
    if(start_cpy == 0)
    {
        chprintf(SDU, "[ERROR] [WEB] URL parsing\r\n");
        return;
    }
    for(unsigned int i = 0; i < strlen(http_rqst_foot); i++)
        http_request[rqst_idx++] = http_rqst_foot[i];
    http_request[rqst_idx] = '\0';
    chprintf(SDU, "[INFO] [WEB] New request: %s", http_request);
    request();
}

/* Return the content of a http response
*/
static void get_http_content(char * input_str)
{
    static char cpy[TCP_MSS];
    int i = 0;
    strcpy(cpy, (const char *)input_str);
    while(i++ < TCP_MSS - 4)
        if(cpy[i] == '\r' && cpy[i+1] == '\n' && cpy[i+2] == '\r' && cpy[i+3] == '\n')
            break;
    if(i == TCP_MSS - 3) {
        chprintf(SDU, "[ERROR] [WEB] get_http_content: not found\r\n");
        return;
    }
    i += 3;
    int input_str_idx = 0;
    while(i++ < TCP_MSS - 1)
        input_str[input_str_idx++] = cpy[i];
}

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
        chprintf(SDU, "[ERROR] [WEB] DNS error: %d\r\n", err);
        return;
    }

    // Create TCP connection
    conn = netconn_new(NETCONN_TCP);
    LWIP_ERROR("http_server: invalid conn", (conn != NULL), chThdExit(MSG_RESET););

    strcpy(http_request, "GET /challenge/step3?token=277c362d&initials=VG HTTP/1.0\r\nHost: antinea.enst.fr\r\n\r\n");
    request();

}
