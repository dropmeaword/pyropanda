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
#include "conserver.h"
#include "persistence.h"
#include "networking.h"

#include "log.h"

IPAddress thisip;
DNSServer dnsServer;

// default network settings when operating as an AP
IPAddress ip_static = IPAddress(10, 0, 0, 1);
IPAddress gw_static = IPAddress(10, 0, 0, 1);
IPAddress sn_static = IPAddress(255, 255, 255, 0);

/**
 * connect to WiFi access point as client station
 * 
 * @param ssid to connect to
 * @param passw password
 * @param timeout is in seconds
 */
bool wifi_connect_as_client(const char *ssid, const char *passw, int timeouts = 10) {
  boolean succeeded = true;

  WiFi.mode(WIFI_STA);  // both hotspot and client are enabled
  WiFi.begin(ssid, passw);
  LOG("Connecting to WiFi ");
  LOG(ssid);
  LOG_NEW_LINE

  // wait for connection
  LOG("Connecting");
  unsigned long tout = millis() + (timeouts * 1000);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    if (millis() > tout) {
      succeeded = false;
      break;
    }
  }
  LOG_NEW_LINE

  // if we are connected we probably were assigned an IP
  if (succeeded) {
    thisip = WiFi.localIP();
    // copy assigned IP to config
    config.cfg_ip[0] = thisip[0];
    config.cfg_ip[1] = thisip[1];
    config.cfg_ip[2] = thisip[2];
    config.cfg_ip[3] = thisip[3];
  }

  config.cfg_mode = WifiMode::CLIENT;

  return succeeded;
}

bool wifi_create_ap(const char *myssid) {
  WiFi.mode(WIFI_AP);

  // optional soft ip config
  LOG("Custom AP IP/GW/Subnet");
  LOG_NEW_LINE
  WiFi.softAPConfig(ip_static, gw_static, sn_static);

  WiFi.softAP(myssid);
  LOG("Creating WiFi access point with name ");
  LOG(myssid);
  LOG_NEW_LINE

  // let a long delay happen here, otherwise we might not get an IP
  delay(1000);

  // setup the DNS server redirecting all the domains to the ap IP
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  config.cfg_mode = WifiMode::ACCESS_POINT;
}

/**
 * start the mDNS responder for device
 */
bool network_create_mdns_entry(const char *entryname) {
  if (!MDNS.begin( entryname )) {
    LOG("couldn't set up the mDNS service");
    LOG_NEW_LINE
  } else {
    LOG("mDNS responder started as");
    LOG(entryname);
    LOG_NEW_LINE
  }
}

void wifi_init()
{
  // first try to connect to a given WiFi AP
  if( wifi_connect_as_client(wifi_ssid, wifi_password) ) {
    LOG("Connected to ");
    LOG(wifi_ssid);
    LOG("IP address: ");

    LOG( cfgutil_get_ip_address() );
    LOG_NEW_LINE
    
    // save IP
    persistence_save_settings();

    // create mDNS entry
    String mdns = "pyropanda" + thisip[3];
    network_create_mdns_entry( mdns.c_str() );
  } else {
    // if we do not succeed at connecting to an existing AP, create our own
    // and setup DNS to channel all connections to the captive portal
    LOG("Connection failed.");
    LOG_NEW_LINE

    wifi_create_ap("PP_UNCONFIGURED");
  }
}
