#include <EEPROM.h>
#include <Arduino.h>

#include "persistence.h"
#define VERSION 10

// pointer to current playhead on EEPROM
int position;
config_t config;

template <class T> int EEPROM_xwrite(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    int len = sizeof(value);

    Serial.print("Writing ");
    Serial.print(len);
    Serial.print(" bytes @ address ");
    Serial.print(ee);
    Serial.println();

    for (i = 0; i < len; i++)
          EEPROM.put(ee++, *p++);
    
    EEPROM.commit();

    return i;
}

template <class T> int EEPROM_xread(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    int len = sizeof(value);

    Serial.print("Reading ");
    Serial.print(len);
    Serial.print(" bytes @ address ");
    Serial.print(ee);
    Serial.println();


    for (i = 0; i < len; i++)
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
    Serial.print("Saving config version "); Serial.print(VERSION); Serial.println(" on EEPROM");
    // set version of config format
    config.cfg_version = VERSION;
    int written = EEPROM_xwrite(0, config);
    Serial.print("Wrote "); Serial.print(written); Serial.print(" bytes"); Serial.println();
}

void persistence_load_settings() {
    Serial.println("Loading config from EEPROM");
    int read = EEPROM_xread(0, config);
    Serial.print("Read "); Serial.print(read); Serial.print(" bytes"); Serial.println();
    // warn if config was saved by newer version of lumodot
    if(config.cfg_version > VERSION) {
        Serial.println("config is meant for newer version, I read it anyway but config integrity is not guaranteed!");
    }
}


