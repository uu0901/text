#include "serial.h"
#include "wifi.h"
#include "oled.h"
#include "ntp.h"
#include "weather.h"
#include "ws2812b.h"

#include <Ticker.h>
Ticker ticker;

#include "ritos.h" 
void ShowAllInfo()
{
  u8g2.clearBuffer();
  u8g2.setCursor(0, 16);
  u8g2.print(NtpGetDate() + " " + NtpGetTime());
  u8g2.setCursor(0, 35);
  u8g2.print("温度 " + WeatherGetInfo().temp + "℃");
  u8g2.setCursor(70, 35);
  u8g2.print("湿度 " + WeatherGetInfo().humidity + "%");
  u8g2.setCursor(0, 55);
  u8g2.print(WeatherGetInfo().weather);
  u8g2.setCursor(70, 55);
  u8g2.print(WeatherGetInfo().city);
  u8g2.sendBuffer();
}
bool SyncNetworkServices()
{
  if(WifiConnect())
  {
    OledClear();
    OledShow(0, 16, "WiFi连接成功");
    OledShow(0, 32, "SSID: " + String(WIFI_SSID));
    OledShow(0, 48, "IP: " + WiFi.localIP().toString());
    delay(1000);
  }
  else
  {
    OledClear();
    OledShow(0, 16, "WiFi连接失败");
    OledShow(0, 32, "SSID: " + String(WIFI_SSID));
    return false;
  }
  OledClear();
  OledShow(0, 16, "正在同步网络时间");
  if(!NtpSync())
  {
    SerialPrint("NTP同步失败，请检查网络连接");
  }
  delay(1000);
  OledClear();
  OledShow(0, 16, "正在获取天气信息");
  if(!WeatherUpdate())
  {
    SerialPrint("天气信息获取失败");
  }
  delay(1000);
  return true;
}
const int buttonPin = 0; // 对于ESP8266来说，这里是指GPIO0也就是IO0
const int relayPin =  14; // 对于ESP8266来说，这里是指GPIO0也就是IO0
bool buttonPressedFlag = false;

Ritos tarefa1; // Thread Task One / 綫程任務1
Ritos tarefa2; // Thread Task Two / 綫程任務2
void setup() {
  // put your setup code here, to run once:
  SerialInit();
  SerialPrint("系统启动");

  OledInit();
  OledShow(0, 16, "系统启动中...");
  
  OledShow(0, 32, "正在初始化WiFi...");
  WifiInit();

  NtpInit();
  WeatherInit();

  OledShow(0, 48, "正在连接WiFi...");
  OledShow(0, 64, "SSID: " + String(WIFI_SSID));
  SyncNetworkServices();

  WS2812B_Init();

  pinMode(buttonPin, INPUT_PULLUP); // 注意：使用INPUT_PULLUP而非INPUT
  pinMode(relayPin, OUTPUT); 

  ticker.attach(0.001, onTimer);

  // Create threads task / 創建线程任務
  tarefa1.task(thread1);  
  tarefa2.task(thread2);
}
uint8_t num;
uint8_t relay_output = 0;
void onTimer()
{
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && !buttonPressedFlag)
  { 
    buttonPressedFlag = true;
    num ++;
    if(num > 7) num = 1;
    Serial.println(num);
  } 
  else if (buttonState == HIGH && buttonPressedFlag) 
  { 
    buttonPressedFlag = false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
volatile int a = 1000, b = 3000;

void thread1() {
  a--;
  
  if ( a <= 0) {
     WS2812B_Set_Color(num);
    
   // Attention! Delay() cannot be used within the thread / 注意！綫程内不允許使用Delay()
   a = 10;// 1500ms. You can be used in this way instead of Delay(1500); 您可以通過此方法延時，代替Delay(1500);
  }
}

void thread2() {
  b--; 
  
  if ( b <= 0 ) {
    static uint32_t LastWeatherUpdate = 0;
    static uint32_t LastDisplayUpdate = 0;
    static uint32_t LastKeyUpdate = 0;
    uint32_t CurrentMillis = millis();
    if(CurrentMillis - LastWeatherUpdate >= 600000)
    {
        if(WeatherUpdate())
        {
          ShowAllInfo();
        }
      LastWeatherUpdate = CurrentMillis;
    }
    if(CurrentMillis - LastDisplayUpdate >= 1000)
    {
        ShowAllInfo();  
      LastDisplayUpdate = CurrentMillis;
    }
    if(CurrentMillis - LastKeyUpdate >= 10000)
    {
      relay_output = !relay_output;
      // digitalWrite(relayPin, relay_output);
      LastKeyUpdate = CurrentMillis;
    }
    
    // b = 4300; // 4300ms
  }
}