#ifndef WS2812B_H
#define WS2812B_H

#include <Adafruit_NeoPixel.h>
#define PIN 12
enum
{
    ColorWipe = 1,      // 递进渐变
    TheaterChase,       // 交叉突变
    Rainbow,            // 整体渐变
    RainbowCycle,       // 呼吸灯效果(rainbow进阶版本)
    TheaterChaseRainbow,// 交叉突变 + 整体渐变(rainbow进阶版本)
    Meteor,             // 流星
    None,             // 流星
}; 
void WS2812B_Init();
void WS2812B_Set_Color(uint8_t color);

#endif