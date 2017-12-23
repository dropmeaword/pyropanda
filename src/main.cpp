#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <Arduino.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

#include "config.h"
#include "networking.h"
#include "persistence.h"
#include "conserver.h"

#include "artnet.h"
#include "ledstrip.h"

#include "log.h"

int panda_id = -1;

/*
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  LOG(">> DMX [u: ");
  LOG(universe);
  LOG(" len: ");
  LOG(length);
  LOG(" seq: ");
  LOG(sequence);
  LOG("]");
  LOG_NEW_LINE

  sendFrame = 1;

  // Store which universe has got in
  if ((universe - startUniverse) < maxUniverses)
    universesReceived[universe - startUniverse] = 1;

  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      //Serial.println("Broke");
      sendFrame = 0;
      break;
    }
  }

  // read universe and put into the right part of the display buffer
  for (int i = 0; i < length / 3; i++)
  {
    int led = i + (universe - startUniverse) * (previousDataLength / 3);
    
    if (led < numLeds)
      leds[led] = CRGB(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
    
    // if (led < numLeds)
    //   leds.setPixelColor(led, data[i * 3], data[i * 3 + 1], data[i * 3 + 2]);
  }
  previousDataLength = length;     
  
  if (sendFrame)
  {
    FastLED.show();
    // Reset universeReceived to 0
    memset(universesReceived, 0, maxUniverses);
  }
}
*/

void setup()
{
  Serial.begin(115200);

  pinMode(BUILTIN_LED, OUTPUT);

  persistence_init();
  persistence_load_settings();

  wifi_init();

  artnet_init();

  conserver_setup();

  ledstrip_init();
  ledstrip_test_pattern();
}

void loop()
{
  // @TODO at the moment we only need DNS when running in AP mode
  // but DNS is also necessary for the mDNS entry to work, do we want
  // mDNS over a tighter loop?
  if( config.cfg_mode == WifiMode::ACCESS_POINT) {
    dnsServer.processNextRequest();
  }

  // we call the read function inside the loop
  artnet.read();

  // this runs the webserver loop 
  conserver_loop();
}