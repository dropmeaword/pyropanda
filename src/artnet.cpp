#include <SPI.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArtnetWifi.h>

#include "log.h"
#include "ledstrip.h"
#include "artnet.h"

// see https://github.com/tangophi/Artnet_DMX_SD_Card
// see http://www.instructables.com/id/ESP8266-Artnet-to-DMX/
// see https://github.com/mtongnz/ESP8266_ArtNetNode_DMX


//const int numLeds = 39; // change for your setup

const int startUniverse = 0; // CHANGE FOR YOUR SETUP most software this is 1, some software send out artnet first universe as zero.
//const int numberOfChannels = numLeds * 3; // Total number of channels you want to receive (1 led = 3 channels)
//byte channelBuffer[numberOfChannels]; // Combined universes into a single array

// Artnet settings
ArtnetWifi artnet;

// SD card
File datafile;

char fileNameFull[10] = "";
int  fileNameSuffix   = 0;

volatile bool startRecord = false;
volatile bool stopRecord  = false;
volatile bool recording   = false;


// Check if we got all universes
const int maxUniverses = numberOfChannels / 512 + ((numberOfChannels % 512) ? 1 : 0);
//bool universesReceived[maxUniverses];
bool storeFrame = 1;
bool sendFrame = 1;
int previousDataLength = 0;

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

  // // Store which universe has got in
  // if ((universe - startUniverse) < maxUniverses)
  //   universesReceived[universe - startUniverse] = 1;

  // for (int i = 0 ; i < maxUniverses ; i++)
  // {
  //   if (universesReceived[i] == 0)
  //   {
  //     //Serial.println("Broke");
  //     sendFrame = 0;
  //     break;
  //   }
  // }

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
    ledstrip_refresh();
    
    // Reset universeReceived to 0
    // memset(universesReceived, 0, maxUniverses);
  }
}

void artnet_init() {
  artnet.begin();
  // DMX packet callback
  artnet.setArtDmxCallback(onDmxFrame);
}

void handle_redcording_start()
{
  if (!recording && !startRecord)
  {
    startRecord = true;
    Serial.println("Start button pressed.");
  }
}

void handle_recording_stop()
{
  if (recording && !stopRecord)
  {
    stopRecord = true;
    Serial.println("Stop button pressed.");
  }
}

void recording_loop() {
  // Open a file for writing when the start button is pressed
  // and also set recording to true so that incoming DMX frames
  // are written to the file.
  if (startRecord && !recording)
  {
    datafile = SD.open(fileNameFull, FILE_WRITE);
    
    Serial.print("Opening ");
    Serial.println(fileNameFull);

    startRecord = false;
    recording = true;
  }

  // Stop the recording when the stop button is pressed and close 
  // the current file which was earlier opened for writing.  Also 
  // increment the fileNameFull variable.
  if (recording && stopRecord)
  {
    recording = false;
    delay(30);
    Serial.print("Closing ");
    Serial.println(fileNameFull);

    datafile.close();
    sprintf(fileNameFull, "data%d", ++fileNameSuffix);  // fileNameSuffix is incremened for the next filename.
    stopRecord = false;
  }
}

/*
// This function is called for every packet received.  It will contain data for only
// one universe.  Hence, wait till data for all universes are received before 
// writing a full frame to the file.
void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data)
{
  storeFrame = 1;

  // Store which universe has got in
  if (universe < maxUniverses)
    universesReceived[universe] = 1;

  // See if data for all universes is received.  If it is, then storeFrame will still be 1 and in the next
  // code block, the full DMX frame (containing data for all the universes) will be written to the file.
  for (int i = 0 ; i < maxUniverses ; i++)
  {
    if (universesReceived[i] == 0)
    {
      //Serial.println("Broke");
      storeFrame = 0;
      break;
    }
  }
  
  // read universe and put into the right part of the display buffer
  for (int i = 0 ; i < length ; i++)
  {
    int bufferIndex = i + ((universe - startUniverse) * length);
    if (bufferIndex < numberOfChannels) // to verify
      channelBuffer[bufferIndex] = byte(data[i]);
  }
  
  // Write data to the file if a full DMX frame containing data for all the universes is received and if we 
  // are still recording
  if (recording && storeFrame)
  {    
    datafile.write(channelBuffer, numberOfChannels);
    memset(universesReceived, 0, maxUniverses);
  } 
}
*/