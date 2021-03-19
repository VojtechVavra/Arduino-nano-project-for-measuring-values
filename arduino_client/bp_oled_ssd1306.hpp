#ifndef BP_OLED_SSD1306_HPP
#define BP_OLED_SSD1306_HPP

//--- OLED SSD1306 --------------------------------------
// https://thesolaruniverse.wordpress.com/2019/10/28/how-to-wire-and-run-a-128x32-oled-display-with-ssd1306-driver-with-an-arduino/
// https://eecs.blog/arduino-i2c-oled-screen-tutorial-using-u8glib/
// OLED displej přes I2C 128x32 znaků
// řadič SSD1306

//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bp_measurements.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C   //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// proměnná pro uchování času poslední obnovy displeje
long int prepis = 0;

void OLED_setup()
{
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {    // Address 0x3D for 128x64
      Serial.println(F("OLED SSD1306 allocation failed"));
      while (1);
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  
  //Set font
  // nastavení písma, další písma zde:
  // https://github.com/olikraus/u8glib/wiki/fontsize
  //oled.setFont(u8g_font_unifont); // font: u8g_font_fub14 // dobre fonty
}

void oledDrawText(Measurements m)
{
    String zprava = "";
    if(prepis == 0){
      zprava = String(m.weight);
      zprava += " kg";
    } else if(prepis == 1) {
      zprava = String(m.temperature);
      zprava += " *C";
    } else if(prepis == 2) {
      zprava = String(m.pressure);
      zprava += String(" hPa");
    } else if(prepis == 3) {
      zprava = String(m.humidity);
      zprava += " %";
    } else if(prepis == 4) {
      zprava = String(m.altitude);
      zprava += " m";
    }

    display.clearDisplay();  // clear the graphcis buffer 
    display.setCursor(0, 10); 
    display.setTextColor (WHITE);
    display.setTextSize (2); // smallest font 
    display.print(zprava);
    //display.print(zprava);

    Serial.print(zprava + "\n");
    
    //u8g2_for_adafruit_gfx.setFontMode(1);                 // use u8g2 transparent mode (this is default)
    //u8g2_for_adafruit_gfx.setFontDirection(0);            // left to right (this is default)
    //u8g2_for_adafruit_gfx.setForegroundColor(WHITE);      // apply Adafruit GFX color
    // nastavení písma, další písma zde:
    // https://github.com/olikraus/u8glib/wiki/fontsize
    //u8g2_for_adafruit_gfx.setFont(u8g2_font_helvR14_tf);  // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
    //u8g2_for_adafruit_gfx.setCursor(0,20);                // start writing at this position
    //u8g2_for_adafruit_gfx.print(zprava);               // UTF-8 string with german umlaut chars
    display.display();
}

#endif    // !BP_OLED_SSD1306_HPP
