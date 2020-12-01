#ifndef IP_STACK__H
#define IP_STACK__H

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define API_IP_STACK CONFIG_ESP_API_IP_STACK

#define TAG_IP_STACK "IP-STACK"
#define MAX_RESPONSE_LEN 1000


void http_request_ip_stack(char* lat, int nlat, char* lon, int nlon);
void ipStack(void *params);
short get_lat_lon(char* lat, int n_lat, char* lon, int n_lon);

#endif