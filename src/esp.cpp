#include "persistence.h"
#include "conserver.h"
#include "esp.h"

void firmware_restart() {
//   // Generate the webpage from the variables above
//   String fail = getFlashString(page_head) + getFlashString(firmware_fail);
//   String ok = getFlashString(firmware_success);

  // Send to the client
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/html", (Update.hasError()) ? "fail" : "ok");

  // Restart device
  delay(200);
  ESP.restart();
}

/*
 *  handle firmware upload and update
 */
void firmware_update() {
  HTTPUpload& upload = server.upload();
  
  if(upload.status == UPLOAD_FILE_START){
    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    Serial.printf("Update: %s\n", upload.filename.c_str());
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
    if(!Update.begin(maxSketchSpace)){//start with max available size
      Update.printError(Serial);
    }
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
      Update.printError(Serial);
    }
  } else if(upload.status == UPLOAD_FILE_END){
    if(Update.end(true)){ //true to set the size to the current progress
      Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      
      // Send success page to the client
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", "everything went ok");
    } else {
      Update.printError(Serial);
      
      // Send fail page to the client
      server.sendHeader("Connection", "close");
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/html", "something unexpected happened");
    }
  }
  yield();
}
