#include "oled.h"
#include "serial.h"
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, D1, D2, U8X8_PIN_NONE);
void OledInit()
{
  SerialPrint("初始化OELD屏幕...");
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy14_t_gb2312);
  u8g2.enableUTF8Print();
  u8g2.sendBuffer();
  SerialPrint("初始化OELD屏幕完成");
}
void OledShow(uint8_t x, uint8_t y, String text)
{
  u8g2.setCursor(x, y);
  u8g2.print(text);
  u8g2.sendBuffer();
}
void OledClear()
{
  u8g2.clearBuffer();
  u8g2.sendBuffer();
}