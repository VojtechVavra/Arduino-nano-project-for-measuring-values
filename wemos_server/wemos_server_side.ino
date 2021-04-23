#include <LoRa.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const int csPin = 15;          // LoRa radio chip select
const int resetPin = 16;       // LoRa radio reset
const int irqPin = 5;          // 5 - DIO0, change for your board; must be a hardware interrupt pin

const long frequency = 868E6;  // LoRa Frequency
const int sync_word_length = 4;

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "SSID"
#define WLAN_PASS       "PASSWD"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
// "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_USERNAME    "Username"
#define AIO_KEY         "aio_eiCm40uquRLGw3MXKdrZjj7MU481"


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
// Setup a feeds for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish hmotnost = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/arduino-1.hmotnost");
Adafruit_MQTT_Publish teplota = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/arduino-1.teplota");
Adafruit_MQTT_Publish vlhkost = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/arduino-1.vlhkost");
Adafruit_MQTT_Publish tlak = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/arduino-1.tlak");

void onReceive(int packetSize);
void sendMessage(String outgoing);
void MQTT_connect();
String getValue(String data, char separator, int index);
rfm95_sendReply();

/*byte sync_word[4];
String v_device;
float v_hmotnost = 0;
float v_teplota = 0;
float v_tlak = 0;
float v_vlhkost = 0;*/


void setup()
{
  Serial.begin(115200);
  delay(1000); 
  /*while (!Serial){
      Serial.println("Serial failed!");
      delay(1000);
  };*/
  //delay(1000); 
  Serial.println("LoRa Receiver init");
  LoRa.setPins(csPin, resetPin, irqPin);
    
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa init succeeded.");

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
}

void loop()
{
  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void onReceive(int packetSize)
{
  // try to parse packet
   if (packetSize == 0) return;      // if there's no packet, return
  
   MQTT_connect();
    
   // received a packet
   Serial.print("Received packet '");
   String receivedText;
   // read packet
   while (LoRa.available()) {
      receivedText += (char)LoRa.read();
      //Serial.print((char)LoRa.read());
   }
   Serial.print(receivedText);
    
   // print RSSI of packet
   Serial.print("' with RSSI ");
   Serial.println(LoRa.packetRssi());

   Serial.println("sync_word message in hexa:");
   Serial.print(receivedText[0], HEX);
   Serial.print(receivedText[1], HEX);
   Serial.print(receivedText[2], HEX);
   Serial.print(receivedText[3], HEX);

   //v_device = getValue(receivedText, ' ', 1);
   //v_hmotnost = getValue(receivedText, ' ', 2).toFloat();
   //v_teplota = getValue(receivedText, ' ', 3).toFloat();
   //v_tlak = getValue(receivedText, ' ', 4).toFloat();
   //v_vlhkost = getValue(receivedText, ' ', 5).toFloat();
  
   /*char device[20];
   char sync_word[10];
   int n = sscanf(receivedText, "%s %s %f %f %f %f", sync_word, v_device, &v_hmotnost, &v_teplota, &v_tlak, &v_vlhkost);
   if (n != 6 || v_device != "arduino_1"){
       return;
   }*/
   
    char v_device[20] = {0};
    float v_hmotnost, v_teplota, v_tlak, v_vlhkost;
    
    int n = sscanf(receivedText.c_str() + sync_word_length, "%s %f %f %f %f", v_device, &v_hmotnost, &v_teplota, &v_tlak, &v_vlhkost);
    if (n != 5 || String(v_device) != "arduino_1"){
        return;
    }
  
   //rfm95_sendReply();
  
   Serial.print(F("\nGot message from device: "));
   Serial.println(v_device);
    
   // Now we can publish stuff!
   Serial.print(F("\nZasilam hmotnost "));
   Serial.println(v_hmotnost);
  
   if (!hmotnost.publish(v_hmotnost)) {
      Serial.println(F("Failed"));
   } else {
      Serial.println(F("OK!"));
   }
    
   Serial.print(F("\nZasilam teplotu "));
   Serial.println(v_teplota);
   if (!teplota.publish(v_teplota)) {
      Serial.println(F("Failed"));
   } else {
      Serial.println(F("OK!"));
   }

   Serial.print(F("\nZasilam tlak "));
   Serial.println(v_tlak);
   if (!tlak.publish(v_tlak)) {
      Serial.println(F("Failed"));
   } else {
      Serial.println(F("OK!"));
   }
    
   Serial.print(F("\nZasilam vlhkost "));
   Serial.println(v_vlhkost);
   if (! vlhkost.publish(v_vlhkost)) {
      Serial.println(F("Failed"));
   } else {
      Serial.println(F("OK!"));
   }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

/*String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}*/

void rfm95_sendReply()
{
    // Send a reply message
    Serial.println("Sending reply message");
    
    byte syncWord[2] = {0xFF, 0xFF};
    String syncWordTex = (char*)syncWord;
    String text = syncWordTex + "  Got message";
    
    Serial.println("Sending reply: " + text);
    delay(10);
    
    LoRa.beginPacket();
    LoRa.print(text);
    LoRa.endPacket();
    //Serial.println("Reply message sent");
}
