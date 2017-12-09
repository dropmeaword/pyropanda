/**
 * configuration server over HTTP
 * tested on Wemos D1 lite
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "config.h"
#include "conserver.h"

ESP8266WebServer server( 80 );

String createPage(){
  String page = "<html lang=en_US><head><meta http-equiv='refresh' content='10'/>";
  page += "<title>lumo</title>";
  page += "<style> body { background-color: #fffff; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  page += "</head><body><h1>HEY! HELLO!</h1>";
  page += "<form><input type='radio' name='LED' value='1'>ON";
  page += "<input type='radio' name='LED' value='0'>OFF</li></ul>";
  page += "<input type='submit' value='Update'></form>";
  page += "<br><br><p class='footer'><a href='https://derfunke.net'>derfunke.net</p>";
  page += "</body></html>";
  return page;
}


void handleSubmit() {
  String LEDValue;
  
  LEDValue = server.arg("LED");
  Serial.print(">> setting LED = "); Serial.println(LEDValue);
  
  if ( LEDValue == "1" ) {
    digitalWrite(BUILTIN_LED, HIGH);
    server.send ( 200, "text/html", createPage() );
  } else if ( LEDValue == "0" ) {
    digitalWrite(BUILTIN_LED, LOW);
    server.send ( 200, "text/html", createPage() );
  } else {
    Serial.println("bad form");
  }
}

void handleRoot() { 
  // Serial.println(">> REQ received");
  if ( server.hasArg("LED") ) {
    // Serial.println(">> handle form");
    handleSubmit();
  } else {
    // Serial.println(">> plain page");
    server.send ( 200, "text/html", createPage() );
  }  
}

void conserver_setup() {
  pinMode(BUILTIN_LED, OUTPUT);

  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server is listening" );
}

void conserver_loop() {
  server.handleClient();
}
