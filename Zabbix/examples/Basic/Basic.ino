#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ZabbixPassiveWifi.h>

// Variaveis e constantes do Wifi
#define W_SSID "***" 
#define W_KEY  "***"
ESP8266WiFiMulti WiFiMulti;

// Variaveis e constantes do Zabbix
#define ZABBIX_KEY_PING "agent.ping"
#define ZABBIX_KEY_UNAME "system.uname"

#define ZBX_PING "1"
#define ZBX_NAME "Zabbix Passive Agent."

ZabbixPassiveWifi zabbix;

void connectWifi() {
  WiFiMulti.addAP(W_SSID, W_KEY);

  Serial.println();
  Serial.print("Looking for WiFi.");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  delay(500);
};


void setup() { 
  Serial.begin(115200);

  connectWifi();

  zabbix.begin();
  zabbix.onKeyReceive(keyReceived);
  Serial.println("Listening...");
}

void loop() {
  zabbix.listen();
}

void keyReceived() {
  char* key = zabbix.getKey();
  Serial.print("keyReceived: ");
  Serial.println(key);
  if (strcmp(key, ZABBIX_NO_KEY) != 0) {
    if (strcmp(key, ZABBIX_KEY_PING) == 0) {
      zabbix.respond(ZBX_PING);
    } else if (strcmp(key, ZABBIX_KEY_UNAME) == 0) {
      zabbix.respond(ZBX_NAME);
    }
  }
  Serial.print("Free Memory: ");
  Serial.println(ESP.getFreeHeap());
}