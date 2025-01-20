#include "HttpRequest.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

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
