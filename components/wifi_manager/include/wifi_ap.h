#pragma once
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif

// Initialize WiFi AP
void wifi_ap_init(void);

// Initialize TCP server and return the server socket
int tcp_server_init(int port);

// Send data to the connected client
void tcp_send(int client_sock, void *data, size_t size);

#ifdef __cplusplus
}
#endif