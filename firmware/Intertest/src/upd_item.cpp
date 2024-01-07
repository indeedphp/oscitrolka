#include "upd_item.h"
#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>


const char * c_ap_name PROGMEM = "CARTOOL";
const char * c_ap_pwd  PROGMEM = "12345678";


WebServer httpServer(80);
HTTPUpdateServer httpUpdater;

void update_init(){

  Serial.println(F("Start AP"));
  String ssid = c_ap_name, password = c_ap_pwd, host = c_ap_name;
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println(F("WiFi failed, retrying."));
  }

  if (MDNS.begin(host)) {
    Serial.println(F("mDNS responder started"));
  }


  httpUpdater.setup(&httpServer);
  httpServer.begin();

  MDNS.addService("http", "tcp", 80);
  Serial.println(F("HTTPUpdateServer ready! Open http://cartool.local/update in your browser\n"));
}

void loop_update(){
  
    httpServer.handleClient();

}