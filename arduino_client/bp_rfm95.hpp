#ifndef BP_RFM95_HPP
#define BP_RFM95_HPP

// LoRa 9x_TX
// Example sketch showing how to create a simple messaging client (transmitter)
// It is designed to work with the other example LoRa9x_RX
// https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/rfm9x-test
// https://learn.adafruit.com/radio-featherwing/using-the-rfm-9x-radio


// Lora RFM_95
#include <SPI.h>
#include <RH_RF95.h>
#include "bp_measurements.hpp"

#define RFM95_CS 10     // ss
#define RFM95_RST 9
#define RFM95_INT 2     // DIO0

// frequency - frequency in Hz (434.0 for Asia, 868.0 for Europe, 915.0 for North America)
#define RF95_FREQ 868.0   // EU_FREQUENCY
#define MSG_LEN 41
#define WAIT_TIME_FOR_REPLY 10000    // 10 sec

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
// end Lora RFM_95


String createMessage(Measurements measurements);
void rfm95_waitForReply();


void RFM95_Setup()
{
    pinMode(RFM95_RST, OUTPUT);
    digitalWrite(RFM95_RST, HIGH);

    Serial.println("Arduino LoRa RFM95 TX");

    // manual reset
    digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    
    while (!rf95.init()) {
        Serial.println("LoRa radio init failed");
        while (1);
    }
    Serial.println("LoRa radio init OK!");
    
    if (!rf95.setFrequency(RF95_FREQ)) {
        Serial.println("setFrequency failed");
        while (1);
    }
    //Serial.print("Set Freq to: " + String(RF95_FREQ));

    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
   
    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
    // you can set transmitter powers from 5 to 23 dBm:
    rf95.setTxPower(11, false); // 23
}

void rfm95_send(Measurements measurements)
{
    // Send a message to rf95_server
    String text = createMessage(measurements);
    unsigned int text_len = text.length() + 1;  // size_t
    Serial.println("  msg_len: " + text_len);
    char msg_buffer[MSG_LEN];
    text.toCharArray(msg_buffer, text_len);
    msg_buffer[text_len] = 0;
    
    Serial.println("Sending...");
    delay(10);
    rf95.send((uint8_t *)msg_buffer, MSG_LEN);
    
    //Serial.println("Waiting for packet to complete...");
    delay(10);
    rf95.waitPacketSent();

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
}

void rfm95_waitForReply()
{
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
  //delay(1000);
}

String createMessage(Measurements measurements)
{
    String msg;
    msg += measurements.weight;
    msg += " ";
    msg += measurements.temperature;
    msg += " ";
    msg += measurements.pressure;
    msg += " ";
    msg += measurements.humidity;
    msg += " ";
    msg += measurements.altitude;
    msg += " ";
    //msg += measurements.time;
    
    return msg;
}

#endif    // !BP_SENDER_HPP
