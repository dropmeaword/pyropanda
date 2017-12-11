#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>
#include <FastLED.h>

#include "config.h"
#include "conserver.h"

// connect to wifi – returns true if successful or false if not
bool wifi_connect()
{
  boolean state = true;
  int i = 0;

  WiFi.begin(wifi_ssid, wifi_password);
  Serial.print("Connecting to WiFi ");
  Serial.println(wifi_ssid);
  
  // Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(wifi_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
