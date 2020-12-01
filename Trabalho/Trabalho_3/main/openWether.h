#ifndef OPEN_WETHER__H
#define OPEN_WETHER__H

#define API_OPEN_WETHER CONFIG_ESP_API_OPEN_WETHER

void http_request_wether(char *lat, int nlat, char *lon, int nlon);

#endif