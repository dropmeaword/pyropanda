#ifndef __CONSERVER_H__
#define __CONSERVER_H__

#include <ESP8266WebServer.h>

void conserver_setup();
void conserver_loop();

extern ESP8266WebServer server;

#endif // __CONSERVER_H__