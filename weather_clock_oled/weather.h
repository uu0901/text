#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#define WEATHER_API_KEY "SyWbZlkKQW3IZ8CcS"
#define WEATHER_CITY    "guangzhou"
#define WEATHER_URL     "http://api.seniverse.com/v3/weather/daily.json?key=%s&location=%s&language=zh-Hans&unit=c&start=0&days=1"
struct WeatherInfo{
  String city;
  String weather;
  String temp;
  String humidity;
};
void WeatherInit();
bool WeatherUpdate();
WeatherInfo WeatherGetInfo();
#endif