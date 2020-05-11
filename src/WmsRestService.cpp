#include <WmsRestService.h>

WmsRestService::WmsRestService(const char* wifi_ssid, const char* wifi_passwd, int server_port) {
	_wifi_ssid = const_cast<char*> (wifi_ssid);
	_wifi_passwd = const_cast<char*> (wifi_passwd);
	_server_port = server_port <= 0 ? SERVER_DEFAULT_PORT : server_port;
	_webServer = nullptr;
}

WmsRestService::~WmsRestService() {
	if(_webServer != nullptr) {
		_webServer->close();
		delete _webServer;
		_webServer = nullptr;
	}
}

bool WmsRestService::beginWifi() {
	int retries = 0;
	bool connected = false;
	
	DEBUG_PRINTLN("")
	DEBUG_PRINT("Connecting Wifi Access Point")
	
	WiFi.mode(WIFI_STA);
	WiFi.begin(_wifi_ssid, _wifi_passwd);
	
	while ((WiFi.status() != WL_CONNECTED) && (retries < WIFI_MAX_RETRIES)) {
		retries++;		
		delay(WIFI_RETRY_DELAY);
		
		DEBUG_PRINT(".");
	}
	connected = WiFi.status() == WL_CONNECTED; 
	
	DEBUG_PRINTLN("")
	DEBUG_IIF_PRINT(connected, "Connected to wifi ", "Unable to connect to wifi: ")
	DEBUG_IF_PRINTLN(connected, _wifi_ssid)
	DEBUG_IF_PRINT(connected, "Local IP Address: ")
	DEBUG_IF_PRINTLN(connected, WiFi.localIP())
	
	return connected;
}

bool WmsRestService::beginServer(TServerConfigureHandler, const char* welcome_message) {
	// Check if it's already initialized
	if(_webServer != nullptr)
		return true;
	
	DEBUG_PRINTLN("")
	DEBUG_PRINTLN("Initialize web server...")
	
	_webServer = new ESP8266WebServer(_server_port);	
	_webServer->on("/", HTTP_GET, [&]() {
		_webServer->send(200, "text/html",
			"Welcome to the Arduino Rest Server");
		});
	if(serverHandler != nullptr)
		serverHandler(_webServer);
	
	// Let start remote connections
	_webServer->begin();
	
	return beginWifi();
}

bool WmsRestService::isConnected() {
	if(_webServer == nullptr)
		return false;
	return WiFi.status() == WL_CONNECTED;
}

void WmsRestService::handleClient() {
	if(_webServer != nullptr) {
		//DEBUG_PRINTLN("Handeling client")
		_webServer->handleClient();
	}
}

void WmsRestService::sendResponse(int code, const JsonDocument& document) {
	size_t size = measureJson(document) + 2;
	char buffer[size];
	
	DEBUG_JSON(document)
	serializeJson(document, buffer, size - 1);

	_webServer->send(code, "application/json", buffer);
}