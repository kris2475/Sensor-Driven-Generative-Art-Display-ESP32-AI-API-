/*******************************************************
 * DIYMORE_LCD_TEST.ino
 * Fully self-contained TFT_eSPI test for ESP32/ESP8266
 * Author: Your Name
 *******************************************************/

#include "TFT_eSPI.h"      // Local library in same folder
TFT_eSPI tft = TFT_eSPI(); // Create TFT instance

void setup() {
  Serial.begin(115200);
  Serial.println("DIYMORE LCD Test Starting...");

  // Initialize TFT
  tft.init();
  tft.setRotation(3);           // Adjust orientation if needed
  tft.fillScreen(TFT_BLACK);    // Clear screen

  // Enable backlight if defined in User_Setup_DIYMORE.h
#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
#endif

  // Display test message
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("DIYMORE LCD Test", 10, 10);
}

void loop() {
  // Simple color test
  tft.fillScreen(TFT_RED);
  delay(500);
  tft.fillScreen(TFT_GREEN);
  delay(500);
  tft.fillScreen(TFT_BLUE);
  delay(500);

  // Draw some text in different sizes
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(1);
  tft.drawString("Size 1", 10, 10);
  tft.setTextSize(2);
  tft.drawString("Size 2", 10, 30);
  tft.setTextSize(3);
  tft.drawString("Size 3", 10, 60);

  delay(1000);
}


