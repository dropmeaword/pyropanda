#ifndef __WIFI_H__
#define __WIFI_H__

#include <DNSServer.h>
#include <ESP8266mDNS.h>

enum class WifiMode {
    CLIENT = 10,        // station node
    HYBRID_AP = 15,     // station + ap
    ACCESS_POINT = 20,  // ap only
    MESH = 30           // mesh
};

#define DNS_PORT  53

extern DNSServer dnsServer;
extern IPAddress thisip;

void wifi_init();

#endif // __WIFI_H__