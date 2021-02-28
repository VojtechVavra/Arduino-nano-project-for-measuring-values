// Bakalářská práce 2021. Vojtěch Vávra, VŠB-TUO FEI

#include "bp_oled_ssd1306.hpp"
#include "bp_bme280.hpp"
#include "bp_rfm95.hpp"
#include "bp_scale_hx711.hpp"


void setup() {
  while (!Serial);
  Serial.begin(9600);
  
  // OLED SSD1306
  OLED_setup();
  
  // Scale HX711
  Scale_Setup();
  
  // BME280 - teplota, vlhkost, tlak
  BME280_Setup();
  
  // RFM95 LoRa
  RFM95_Setup();
}

void loop()
{
     float weight = getWeight();
     printScale();
     
     Measurements mData = getMeasurements();
     mData.weight = weight;
     
     if(prepis == 5){
       prepis = 0;
       oledDrawText(mData);
     } else{
       oledDrawText(mData);
       prepis++;
     }   
     scale.power_down();     // put the ADC in sleep mode

     rfm95_send(mData);

     
     //Delay before repeating the loop.
     delay(6000);  // 5000
     scale.power_up();
}
