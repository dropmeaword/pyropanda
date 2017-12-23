#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__

#include <FastLED.h>


#include "config.h"


// LED settings
#define NUM_LEDS 51
extern const int numLeds;
extern const int numberOfChannels;

extern CRGB leds[NUM_LEDS];

extern const int pinSDA;
extern const int pinCLK;

void ledstrip_init();
void ledstrip_refresh();
void ledstrip_test_pattern();

#endif // __LEDSTRIP_H__