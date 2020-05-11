#ifndef WmsRestClient_h
#define WmsRestClient_h

#include <stdio.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#ifndef DEBUG_MODE
//#define DEBUG_MODE
#endif

#ifdef DEBUG_MODE
	#define DEBUG_SERIAL_INIT(x)		Serial.begin(x);
	#define DEBUG_PRINT(x)				Serial.print(x);
	#define DEBUG_PRINTLN(x)			Serial.println(x);
	#define DEBUG_IF_PRINT(x, y)		if(x) Serial.print(y);	
	#define DEBUG_IF_PRINTLN(x, y)		if(x) Serial.println(y);
	#define DEBUG_IIF_PRINT(x, y, z)	if(x) { Serial.print(y); } else { Serial.print(z); }
	#define DEBUG_IIF_PRINTLN(x, y, z)	if(x) { Serial.println(y); } else { Serial.println(z); }
	#define DEBUG_JSON(x)				serializeJsonPretty(x, Serial); Serial.println();
#else 
	#define DEBUG_SERIAL_INIT(x)
	#define DEBUG_PRINT(x)
	#define DEBUG_PRINTLN(x)
	#define DEBUG_IF_PRINT(x, y)	
	#define DEBUG_IF_PRINTLN(x, y)
	#define DEBUG_IIF_PRINT(x, y, z)
	#define DEBUG_IIF_PRINTLN(x, y, z)
	#define DEBUG_JSON(x)
#endif

#define WIFI_RETRY_DELAY 	500
#define WIFI_MAX_RETRIES 	50
#define SERVER_DEFAULT_PORT 80

#define TServerConfigureHandler 	void (*serverHandler)(ESP8266WebServer*)

class WmsRestService {
	public:
		WmsRestService(const char* wifi_ssid, const char* wifi_passwd, int port);
		~WmsRestService();
		
		bool beginServer(TServerConfigureHandler);
		void handleClient();
		bool isConnected();
		void sendResponse(int code, const JsonDocument& document);
		
	private:
		bool 	_wifi_connected;
		char * 	_wifi_ssid;
		char * 	_wifi_passwd;
		int		_server_port;
		
		ESP8266WebServer* _webServer;
		
		bool beginWifi();
};

#endif