#include "ws2812b.h"
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ400);
 
void colorWipe(uint32_t c, uint8_t wait) {
        for(uint16_t i=0; i<strip.numPixels(); i++) {
                strip.setPixelColor(i, c);
                strip.show();
                delay(wait);
        }
}
 
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
 
void rainbow(uint8_t wait) {
    uint16_t i, j;
 
    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}
 
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;
 
    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        // delay(wait);
    }
}
 
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {    //do 10 cycles of chasing
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, c);        //turn every third pixel on
            }
            strip.show();
 
            delay(wait);
 
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);
            }
        }
    }
}
 
void theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) {
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));        //turn every third pixel on
            }
            strip.show();
 
            delay(wait);
 
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);                //turn every third pixel off
            }
        }
    }
}
 
void clear(){
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, 0); 
        delay(10);
    }
}
 
void meteor(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
    const uint8_t num = 15;
    uint8_t max_color = red;
    if(green > max_color)
        max_color = green;
    if(blue > max_color)
        max_color = blue;
    uint8_t instance = (max_color-200)/num;
    for(uint16_t i=0; i<strip.numPixels() + num; i++) {
        for(uint8_t j = 0; j < num; j ++){
            if(i - j >= 0 && i - j < strip.numPixels()){
                int red_after = red - (instance * j);
                int green_after = green - (instance * j);
                int blue_after = blue - (instance * j);
                
                if(j>=1){
                    red_after -= 200;
                    green_after -= 200;
                    blue_after -= 200;
                }
                strip.setPixelColor(i - j, strip.Color(red_after >= 0 ? red_after : 0, green_after >= 0 ? green_after : 0, blue_after >= 0 ? blue_after : 0));
            }
        }
        if(i - num >= 0 && i-num < strip.numPixels())
            strip.setPixelColor(i-num, 0); 
 
        strip.show();
        delay(wait);
    }
}
 
void meteor_overturn(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
 
    const uint8_t num = 15;
    uint8_t max_color = red;
    if(green > max_color)
        max_color = green;
    if(blue > max_color)
        max_color = blue;
    uint8_t instance = (max_color-200)/num;
    for(int i=strip.numPixels() - 1; i>=-num; i--) {
        for(uint8_t j = 0; j < num; j ++){
            if(i + j >= 0 && i + j < strip.numPixels()){
                int red_after = red - instance * j;
                int green_after = green - instance *  j;
                int blue_after = blue - instance *  j;
                if(j>=1){
                    red_after -= 200;
                    green_after -= 200;
                    blue_after -= 200;
                }
                strip.setPixelColor(i + j, strip.Color(red_after >= 0 ? red_after : 0, green_after >= 0 ? green_after : 0, blue_after >= 0 ? blue_after : 0));
            }
        }
        if(i + num >= 0 && i+num < strip.numPixels())
            strip.setPixelColor(i+num, 0); 
 
        strip.show();
        delay(wait);
    }
}
 
 
 
 
void WS2812B_Init()
{
    strip.begin();
    strip.setBrightness(100);
    strip.show(); // Initialize all pixels to 'off'
}

void WS2812B_Set_Color(uint8_t color) 
{
    switch(color)
    {
      case ColorWipe : 
                      {
                        colorWipe(strip.Color(255, 0, 0), 20); // Red
                        colorWipe(strip.Color(0, 255, 0), 20); // Green
                        colorWipe(strip.Color(0, 0, 255), 20); // Blue
                      }break;
      case TheaterChase : 
                      {
                        theaterChase(strip.Color(127, 127, 127), 50); // White
                        theaterChase(strip.Color(127, 0, 0), 50);     // Red
                        theaterChase(strip.Color(0, 0, 127), 50);     // Blue
                      }break;
      case Rainbow : 
                      {
                         rainbow(12);
                      }break;
      case RainbowCycle : 
                      {
                         rainbowCycle(10);
                      }break;
      case TheaterChaseRainbow : 
                      {
                         theaterChaseRainbow(50);
                      }break;
      case Meteor : 
                      {
                         clear();
                         meteor(255, 0, 0, 10);
                         meteor_overturn(255, 0, 0, 10);
                      }break;
      case None   :
                      {
                         strip.show(); // Initialize all pixels to 'off'
                      }break;
    }
}