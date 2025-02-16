#include "weather.h"
#include "serial.h"
WeatherInfo CurrentWeather;
void WeatherInit()
{
  SerialPrint("初始化天气模块...");
  CurrentWeather.city = "未知";
  CurrentWeather.weather = "未知";
  CurrentWeather.temp = "0";
  CurrentWeather.humidity = "0";
  SerialPrint("初始化天气模块完成");
}
bool WeatherUpdate()
{
  SerialPrint("正在请求天气信息...");
  char url[200];
  sprintf(url, WEATHER_URL, WEATHER_API_KEY, WEATHER_CITY);
  SerialPrint("请求URL：" + String(url));
  WiFiClient client;
  HTTPClient http;
  if(http.begin(client, url))
  {
    SerialPrint("开始HTTP请求...");
    int HttpCode = http.GET();
    SerialPrint("HTTP请求码：" + String(HttpCode));
    if(HttpCode == HTTP_CODE_OK)
    {
        String PayLoad = http.getString();
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, PayLoad);
        if(!error)
        {
            JsonObject results_0 = doc["results"][0];
            JsonObject location = results_0["location"];
            JsonArray daily = results_0["daily"];
            JsonObject today = daily[0];
            if(!today.isNull())
            {
              CurrentWeather.city = location["name"].as<String>();
              CurrentWeather.weather = today["text_day"].as<String>();
              CurrentWeather.temp = today["high"].as<String>();
              CurrentWeather.humidity = today["humidity"].as<String>();

              SerialPrint("城市：" + CurrentWeather.city);
              SerialPrint("天气：" + CurrentWeather.weather);
              SerialPrint("温度：" + CurrentWeather.temp);
              SerialPrint("湿度：" + CurrentWeather.humidity);
              http.end();
              return true;
            }
            else
            {
              SerialPrint("JSON解析失败，找不到今日数据");
            }
        }
        else
        {
            SerialPrint("JSON解析错误：" + String(error.c_str()));
        }
    }
    http.end();
  }
  SerialPrint("天气更新失败");
  return false;
}

WeatherInfo WeatherGetInfo()
{
  return CurrentWeather;
}