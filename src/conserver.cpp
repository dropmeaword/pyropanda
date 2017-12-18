/**
 * configuration server over HTTP
 * tested on Wemos D1 lite
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

#include "config.h"
#include "conserver.h"
#include "persistence.h"
#include  "website.h"

ESP8266WebServer server( 80 );

// String createPage(){
//   String page = "<html lang=en_US><head><meta http-equiv='refresh' content='10'/>";
//   page += "<title>lumo</title>";
//   page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
//   page += "</head><body><h1>HEY! HELLO!</h1>";
//   page += "<form><input type='radio' name='LED' value='1'>ON";
//   page += "<input type='radio' name='LED' value='0'>OFF</li></ul>";
//   page += "<input type='submit' value='Update'></form>";
//   page += "<br><br><p class='footer'><a href='https://derfunke.net'>derfunke.net</p>";
//   page += "</body></html>";
//   return page;
// }


// void handleSubmit() {
//   String LEDValue;
  
//   LEDValue = server.arg("LED");
//   Serial.print(">> setting LED = "); Serial.println(LEDValue);
  
//   if ( LEDValue == "1" ) {
//     digitalWrite(BUILTIN_LED, HIGH);
//     server.send ( 200, "text/html", createPage() );
//   } else if ( LEDValue == "0" ) {
//     digitalWrite(BUILTIN_LED, LOW);
//     server.send ( 200, "text/html", createPage() );
//   } else {
//     Serial.println("bad form");
//   }
// }

// void handleRoot() { 
//   // Serial.println(">> REQ received");
//   if ( server.hasArg("LED") ) {
//     // Serial.println(">> handle form");
//     handleSubmit();
//   } else {
//     // Serial.println(">> plain page");
//     server.send ( 200, "text/html", createPage() );
//   }  
// }

void handle_config_get() {

  Serial.println(">> GET CONFIG");

  DynamicJsonBuffer jsonBuffer(4096);
  JsonObject &root = jsonBuffer.createObject();

  root["version"] = config.cfg_version;
  root["ipaddress"] = cfgutil_get_ip_address();
  root["net"] = config.cfg_net;
  root["subnet"] = config.cfg_subnet;
  //root["universes"] = config.cfg_universes;
  root["myssid"] = config.cfg_myssid;
  // root["wifi_ssid"] = config.cfg_wifi_ssid;
  // root["wifi_password"] = config.cfg_wifi_password;
  root["mesh_prefix"] = config.cfg_mesh_prefix;
  root["mode"] = config.cfg_mode;

  // render json to string
  String tmp;
  root.printTo(tmp);
  
  size_t jsonlength = root.measureLength();
  server.setContentLength(jsonlength); 
  server.send ( 200, "text/json", tmp );
}

void handle_config_post() {
}

void action_install_handlers() {
    Serial.println(">> CONFIG handlers installed");

  server.on ( "/config", HTTP_POST, handle_config_post );
  server.on ( "/config", HTTP_GET, handle_config_get );
}

void conserver_setup() {
  // server.on ( "/", handleRoot );
  
  // set web resources handlers
  setupPageHandlers();
  // setup handlers for config action methods
  action_install_handlers();

  server.begin();
  Serial.println ( "HTTP server is listening" );
}

void conserver_loop() {
  server.handleClient();
}
