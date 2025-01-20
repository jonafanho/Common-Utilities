#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <ArduinoJson.h>

int httpGet(const char *url, JsonDocument *jsonDocument);

#endif
