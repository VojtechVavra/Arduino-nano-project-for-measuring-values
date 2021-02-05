// Bakalářská práce 2021. Vojtěch Vávra, VŠB-TUO FEI
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;

HX711 scale;
//--- OLED SSD1306 --------------------------------------
// https://thesolaruniverse.wordpress.com/2019/10/28/how-to-wire-and-run-a-128x32-oled-display-with-ssd1306-driver-with-an-arduino/
// https://eecs.blog/arduino-i2c-oled-screen-tutorial-using-u8glib/
// OLED displej přes I2C 128x32 znaků
// řadič SSD1306
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
//#include <fonts/FreeSerifBoldItalic9pt7b.h>
//#include <U8g2_for_Adafruit_GFX.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);
//U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

// --- BME280 - teplota, vlhkost, tlak
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
// --------

// proměnná pro uchování času poslední obnovy displeje
long int prepis = 0;

void setup() {
  Serial.begin(38400);
  //--- OLED ---------
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("OLED SSD1306 allocation failed"));
      for(;;);
}
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //display.begin(SSD1306_SWITCHCAPVCC);
  //u8g2_for_adafruit_gfx.begin(display);                 // connect u8g2 procedures to Adafruit GFX
  //display.setFont(&FreeSerifBoldItalic9pt7b);
  /////
  
  //Serial.println("Initializing the scale");
  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //Serial.println("Before setting up the scale:");
 
  scale.set_scale(2280.0f);   //2280.0f                   // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0

  //Serial.println("After setting up the scale:");
 
   /*Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
						// by the SCALE parameter set with set_scale
  */
  //Serial.println("Readings:");

  //Set font
  // nastavení písma, další písma zde:
  // https://github.com/olikraus/u8glib/wiki/fontsize
  //oled.setFont(u8g_font_unifont); // font: u8g_font_fub14 // dobre fonty

  // BME280
  if (!bme.begin(0x76)) {
    Serial.println("Error BME280 sensor!");
    //Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void loop() {
  // weight/scale section
  Serial.print("reading:\t");
  Serial.print(scale.get_units() / 10, 3);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10) / 10, 3);

  //oledDrawText();
  if(prepis == 5){
    prepis = 0;
    oledDrawText();
  } else{
    oledDrawText();
    prepis++;
  }
   
  scale.power_down();              // put the ADC in sleep mode


  // ---- BME280 ---
  /*Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println("*C");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println("hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println("m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println("%");

  Serial.println();*/
  //delay(1000);
  // ---------------
  
  //Delay before repeating the loop.
  delay(4000);  // 5000
  scale.power_up();

   // nastavení písma, další písma zde:
  // https://github.com/olikraus/u8glib/wiki/fontsize
  //mujOled.setFont(u8g_font_fub14);
  // nastavení výpisu od souřadnic x=0, y=25; y záleží na velikosti písma
  //mujOled.setPrintPos(0, 25);
  
  // výpis uložené části zprávy na OLED displej
  //mujOled.print(zprava);
   // vytvoření proměnné s celou zprávou, která se bude vypisovat
  //String zprava = " Navody.dratek.cz";
  //String zprava = String(scale.get_units() / 10);
  //zprava += ", cas od spusteni ";
  //zprava += millis()/1000;
  //zprava += " kg";
  // porovnání uloženého a aktuálního času
  // při rozdílu větším než 100 ms se provede
  // přepis displeje, zde je to rychlost posunu zprávy
  //if (millis()-prepis > 100) {
    // uložení posledního času obnovení
    //prepis = millis();
  //}
}

// funkce vykresliText pro výpis textu na OLED od zadané pozice
void vykresliText(String text) {
  // nastavení písma, další písma zde:
  // https://github.com/olikraus/u8glib/wiki/fontsize
  
  // nastavení výpisu od souřadnic x=0, y=25; y záleží na velikosti písma
  //oled.setPrintPos(0, 25);
  // uložení části zprávy - od znaku posun uložíme 15 znaků
  // např. na začátku uložíme znaky 0 až 15
  //String vypis;
  //vypis = text.substring(posun, posun+15);
  // výpis uložené části zprávy na OLED displej
  //oled.print(text);
}

void oledDrawText(void)
{
    String zprava = "";
    if(prepis == 0){
      zprava = String(scale.get_units() / 10);
      zprava += " kg";
    } else if(prepis == 1) {
      zprava = String(bme.readTemperature());
      zprava += " *C";
    } else if(prepis == 2) {
      zprava = String(bme.readPressure() / 100.0F);
      zprava += String(" hPa");
    } else if(prepis == 3) {
      zprava = String(bme.readAltitude(SEALEVELPRESSURE_HPA));
      zprava += " m";
    } else if(prepis == 4) {
      zprava = String(bme.readHumidity());
      zprava += " %";
    }
    //Serial.print("in function oledDrawText()\n");
    
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
