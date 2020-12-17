#ifndef WIFI_H
#define WIFI_H

#define WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define WIFI_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

// #define WIFI_SSID      ""
// #define WIFI_PASS      ""
// #define WIFI_MAXIMUM_RETRY  5


#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define WIFI_TAG "WIFI-CONNECTION"


void wifi_start();

#endif
