#include "ledstrip.h"
#include "log.h"

CRGB leds[NUM_LEDS];

const int numLeds = NUM_LEDS;
const int numberOfChannels = numLeds * 3;

// pinout for LED strip
const int pinSDA = D3;
const int pinCLK = D2;


// ESPIChipsets CHIPSET,  uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER > static CLEDController &addLeds(struct CRGB *data, int nLedsOrOffset, int nLedsIfOffset = 0) {


void ledstrip_init() { //(const int ledcount, ESPIChipsets chipset, uint8_t  psda, uint8_t  pclk, EOrder rgbcomp) {
  FastLED.addLeds<APA102, pinSDA, pinCLK, BGR>(leds, NUM_LEDS);
  LOG("Initializing LEDs...");
  LOG_NEW_LINE
}

void ledstrip_refresh() {
    FastLED.show();
}

void ledstrip_test_pattern()
{
  int waiting = 1000;

  LOG(">>> pumping test pattern");
  LOG_NEW_LINE

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
    leds[i] = CRGB( 0, 0, 0);
  FastLED.show();
}

