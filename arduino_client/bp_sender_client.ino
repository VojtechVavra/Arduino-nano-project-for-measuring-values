// Bakalářská práce 2021. Vojtěch Vávra, VŠB-TUO FEI
// bp_sender_client.ino je zdrojový kód použit na koncovém zařízení Arduino Nano,
// které je dále připojeno k senzorům.
 
#include "bp_oled_ssd1306.hpp"
#include "bp_bme280.hpp"
#include "bp_rfm95.hpp"
#include "bp_scale_hx711.hpp"
 
void setup() {
  //while (!Serial);
  Serial.begin(9600);
  
  // OLED SSD1306
  //OLED_setup();
  
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
     //printScale();
     //printBME280();
     
     Measurements mData = getMeasurements();
     mData.weight = weight;
     
     /*if(prepis == 5){
       prepis = 0;
       oledDrawText(mData);
     } else{
       oledDrawText(mData);
       prepis++;
     } */
     
    scale.power_down();     // put the ADC in sleep mode
    
    rfm95_send(mData);
    Serial.println("Sent");
    
   //Delay before repeating the loop.
   //60 minutes each of 60 seconds each of 1000 milliseconds all unsigned longs
   delay(60UL * 60UL * 1000UL - 5020UL);   // - 20
   //delay(10000);  // 10 * 1000
   scale.power_up();
}
