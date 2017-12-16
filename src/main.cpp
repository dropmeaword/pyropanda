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
#include "wifi.h"
#include "persistence.h"
#include "conserver.h"

#include "log.h"

// LED settings
#define NUM_LEDS 51
const int numLeds = NUM_LEDS;
const int numberOfChannels = numLeds * 3;

// pinout for LED strip
const int pinSDA = D3;
const int pinCLK = D2;

// Artnet settings
ArtnetWifi artnet;
const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as 0.

// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
bool universesReceived[maxUniverses];
bool sendFrame = 1;
int previousDataLength = 0;

CRGB leds[NUM_LEDS];

void leds_init() {
  FastLED.addLeds<APA102, pinSDA, pinCLK, BGR>(leds, NUM_LEDS);
  LOG("Initializing LEDs...");
}

void leds_test_pattern()
{
  int waiting = 1000;

  LOG(">>> LEDS test pattern");

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB(127, 0, 0);

  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB(0, 127, 0);

  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB( 0, 0, 127);
  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB(127, 127, 127);

  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB(0, 0, 0);
  
  FastLED.show();
}

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

void setup()
{
  Serial.begin(115200);

  persistence_init();
  persistence_load_settings();

  wifi_connect();
  artnet.begin();

  conserver_setup();

  leds_init();
  leds_test_pattern();

  // DMX packet callback
  artnet.setArtDmxCallback(onDmxFrame);
}

void loop()
{
  // we call the read function inside the loop
  artnet.read();
  conserver_loop();
}