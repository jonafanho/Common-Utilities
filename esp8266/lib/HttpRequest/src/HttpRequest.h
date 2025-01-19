#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <ArduinoJson.h>

int httpGet(const char *url, JsonDocument *jsonDocument)
{
    std::unique_ptr<BearSSL::WiFiClientSecure> wiFiClient(new BearSSL::WiFiClientSecure);
    wiFiClient->setInsecure();
    HTTPClient httpClient;
    httpClient.begin(*wiFiClient, url);
    int httpResponseCode = httpClient.GET();

    if (httpResponseCode > 0)
    {
        deserializeJson(*jsonDocument, httpClient.getString());
    }

    httpClient.end();
    return httpResponseCode;
}

#endif
