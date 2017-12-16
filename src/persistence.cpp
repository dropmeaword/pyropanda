#include <EEPROM.h>
#include <Arduino.h>
/*
#include "persistence.h"
#define VERSION 10

// pointer to current playhead on EEPROM
int position;
config_t config;

template <class T> int EEPROM_xwrite(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);
    return i;
}

template <class T> int EEPROM_xread(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

void persistence_init() {
    EEPROM.begin(EEPROM_SIZE);
    position = 0;
}

void persistence_clear() {
    for(int i = 0; i < EEPROM_SIZE; i) {
        EEPROM.write(i, 0);
    }
}

void persistence_save_settings() {
    EEPROM_xwrite(0, config);
}

void persistence_load_settings() {
    EEPROM_xread(0, config);
    // warn if config was saved by newer version of lumodot
    if(config.cfg_version > VERSION) {
        Serial.println("config is meant for newer version, I read it anyway but config integrity is not guaranteed!");
    }
}


*/
