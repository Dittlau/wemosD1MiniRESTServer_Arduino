#ifndef restServer_h
#define restServer_h

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <SPI.h>

void lightControlGet(WiFiClient);
void lightControlPut(WiFiClient);
void process(WiFiClient);
void errorResponse(WiFiClient,String,String);
void lightStatus(WiFiClient,int);
void lightResponse(WiFiClient,int,String);
void errorResponseLight(WiFiClient,int,String);
void sendHTMLHeader(WiFiClient);

struct light {
  short id;
  bool onStatus;
  short intensity;
};

extern struct light lights[10];

#endif
