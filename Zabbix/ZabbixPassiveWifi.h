#ifndef ZabbixPassiveWifi_h
#define ZabbixPassiveWifi_h

#include <Arduino.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include "Debug.h"

#define ZABBIX_HEADER "ZBXD\x01"
#define ZABBIX_HEADER_LENGTH 5
#define ZABBIX_DATALEN_LENGTH 8
#define ZABBIX_DEFAULT_PORT 10050

#define MAX_BUFFER_SIZE 60
#define MAX_KEY_SIZE 60
#define MAX_RESPONSE_SIZE 100

#define ZABBIX_NO_KEY ""
#define ZABBIX_STATUS_OK 0
#define ZABBIX_STATUS_NO_KEY 1

extern "C" {
  // callback function
  typedef void (*onKeyReceiveCallback)(void);
}

class ZabbixPassiveWifi {
  private:
    // TODO: to try do make WiFi independent
    WiFiServer _server;
    char _buffer[MAX_BUFFER_SIZE];
    char _response[MAX_RESPONSE_SIZE];
    byte _datalen[8];
    int decodeKey(int size, boolean withHeader);
    onKeyReceiveCallback _callback;

  public:
    ZabbixPassiveWifi();
    ZabbixPassiveWifi(int port);
    char* getKey(void);
    char* getResponse(void);
    void onKeyReceive(onKeyReceiveCallback keyReceived);
    void begin(void);
    void listen(void);
    void respond(String value);
    void respond(char* value);
    void respond(int value);
    void respond(float value, byte decimals);
};

#endif
