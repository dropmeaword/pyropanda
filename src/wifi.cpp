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
#include "persistence.h"

#include "log.h"

IPAddress thisip;

// connect to wifi – returns true if successful or false if not
bool wifi_connect()
{
  boolean state = true;
  int i = 0;

  WiFi.begin(wifi_ssid, wifi_password);
  LOG("Connecting to WiFi ");
  LOG(wifi_ssid);
  LOG_NEW_LINE

  // Wait for connection
  LOG("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    LOG(".");
    if (i > 20){
      state = false;
      break;
    }
    i++;
  }
  LOG_NEW_LINE

  if (state){
    LOG("Connected to ");
    LOG(wifi_ssid);
    LOG("IP address: ");
    thisip = WiFi.localIP();
    Serial.print(thisip);
    LOG_NEW_LINE

    // set assigned IP on config
    config.cfg_ip[0] = thisip[0];
    config.cfg_ip[1] = thisip[1];
    config.cfg_ip[2] = thisip[2];
    config.cfg_ip[3] = thisip[3];

    // LOG("Config IP: ");
    // LOG(config.cfg_ip[0]);
    // LOG(".");
    // LOG(config.cfg_ip[1]);
    // LOG(".");
    // LOG(config.cfg_ip[2]);
    // LOG(".");
    // LOG(config.cfg_ip[3]);
    // LOG_NEW_LINE
    
    // save IP
    persistence_save_settings();
  } else {
    LOG("Connection failed.");
  }
  
  return state;
}
