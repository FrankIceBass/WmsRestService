// WmsRestService (info@francescodirienzo.it)
// Copyright Francesco Di Rienzo 2020
// MIT License

#pragma once

#include <stdio.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#ifndef DEBUG_MODE
//#define DEBUG_MODE
#endif

#ifdef DEBUG_MODE
	#define __DEBUG_SERIAL_INIT(x)		Serial.begin(x);
	#define __DEBUG_PRINT(x)				Serial.print(x);
	#define __DEBUG_PRINTLN(x)			Serial.println(x);
	#define __DEBUG_IF_PRINT(x, y)		if(x) Serial.print(y);	
	#define __DEBUG_IF_PRINTLN(x, y)		if(x) Serial.println(y);
	#define __DEBUG_IIF_PRINT(x, y, z)	if(x) { Serial.print(y); } else { Serial.print(z); }
	#define __DEBUG_IIF_PRINTLN(x, y, z)	if(x) { Serial.println(y); } else { Serial.println(z); }
	#define __DEBUG_JSON(x)				serializeJsonPretty(x, Serial); Serial.println();
#else 
	#define __DEBUG_SERIAL_INIT(x)
	#define __DEBUG_PRINT(x)
	#define __DEBUG_PRINTLN(x)
	#define __DEBUG_IF_PRINT(x, y)	
	#define __DEBUG_IF_PRINTLN(x, y)
	#define __DEBUG_IIF_PRINT(x, y, z)
	#define __DEBUG_IIF_PRINTLN(x, y, z)
	#define __DEBUG_JSON(x)
#endif

#define WIFI_RETRY_DELAY 	500
#define WIFI_MAX_RETRIES 	50
#define SERVER_DEFAULT_PORT 80

#define TServerConfigureHandler 	void (*serverHandler)(ESP8266WebServer*)

namespace WMSRESTSERVICE_NAMESPACE {
class RestService {
	public:
		RestService(const char* wifi_ssid, const char* wifi_passwd, int port);
		~RestService();
		
		bool beginServer(TServerConfigureHandler, const char* welcome_message);
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

RestService::RestService(const char* wifi_ssid, const char* wifi_passwd, int server_port) {
	_wifi_ssid = const_cast<char*> (wifi_ssid);
	_wifi_passwd = const_cast<char*> (wifi_passwd);
	_server_port = server_port <= 0 ? SERVER_DEFAULT_PORT : server_port;
	_webServer = nullptr;
}

RestService::~RestService() {
	if(_webServer != nullptr) {
		_webServer->close();
		delete _webServer;
		_webServer = nullptr;
	}
}

bool RestService::beginWifi() {
	int retries = 0;
	bool connected = false;
	
	__DEBUG_PRINTLN("")
	__DEBUG_PRINT("Connecting Wifi Access Point")
	
	WiFi.mode(WIFI_STA);
	WiFi.begin(_wifi_ssid, _wifi_passwd);
	
	while ((WiFi.status() != WL_CONNECTED) && (retries < WIFI_MAX_RETRIES)) {
		retries++;		
		delay(WIFI_RETRY_DELAY);
		
		__DEBUG_PRINT(".");
	}
	connected = WiFi.status() == WL_CONNECTED; 
	
	__DEBUG_PRINTLN("")
	__DEBUG_IIF_PRINT(connected, "Connected to wifi ", "Unable to connect to wifi: ")
	__DEBUG_IF_PRINTLN(connected, _wifi_ssid)
	__DEBUG_IF_PRINT(connected, "Local IP Address: ")
	__DEBUG_IF_PRINTLN(connected, WiFi.localIP())
	
	return connected;
}

bool RestService::beginServer(TServerConfigureHandler, const char* welcome_message) {
	// Check if it's already initialized
	if(_webServer != nullptr)
		return true;
	
	__DEBUG_PRINTLN("")
	__DEBUG_PRINTLN("Initialize web server...")
	
	_webServer = new ESP8266WebServer(_server_port);	
	_webServer->on("/", HTTP_GET, [&]() {
		_webServer->send(200, "text/html", welcome_message);
		});
	if(serverHandler != nullptr)
		serverHandler(_webServer);
	
	// Let start remote connections
	_webServer->begin();
	
	return beginWifi();
}

bool RestService::isConnected() {
	if(_webServer == nullptr)
		return false;
	return WiFi.status() == WL_CONNECTED;
}

void RestService::handleClient() {
	if(_webServer != nullptr) {
		//DEBUG_PRINTLN("Handeling client")
		_webServer->handleClient();
	}
}

void RestService::sendResponse(int code, const JsonDocument& document) {
	size_t size = measureJson(document) + 2;
	char buffer[size];
	
	__DEBUG_JSON(document)
	serializeJson(document, buffer, size - 1);

	_webServer->send(code, "application/json", buffer);
}
}