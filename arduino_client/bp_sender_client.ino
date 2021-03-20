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
    
    // Send a message to rf95_server
    String text = createMessage(mData);
    unsigned int text_len = text.length() + 1;  // size_t
    Serial.println("  msg_len: " + text_len);
    char msg_buffer[text_len] = {0};            // MSG_LEN
    text.toCharArray(msg_buffer, text_len);
    msg_buffer[text_len - 1] = 0;
    
    Serial.println("Sending...");
    delay(10);
  
    rf95.send((uint8_t *)msg_buffer, text_len);
    
    //Serial.println("Waiting for packet to complete...");
    delay(10);
    rf95.waitPacketSent();

  /*
  //rfm95_waitForReply();
  // Wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  Serial.println("Waiting for reply ...");
  delay(10);
  
  if (rf95.waitAvailableTimeout(WAIT_TIME_FOR_REPLY))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      //Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
  */
     
   //Delay before repeating the loop.
   delay(10000);  // 6000
   scale.power_up();
}
