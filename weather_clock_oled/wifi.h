#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID          "MOI"
#define WIFI_PASSWORD      "msy12345"

void WifiInit();
bool WifiConnect(const char *ssid =  WIFI_SSID, const char *password = WIFI_PASSWORD);
#endif