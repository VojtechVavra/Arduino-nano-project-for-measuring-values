//#include <SPI.h>
#include <LoRa.h>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


const int csPin = 15;          // LoRa radio chip select
const int resetPin = 16;        // LoRa radio reset
const int irqPin = 5;          // 5 - DIO0, change for your board; must be a hardware interrupt pin

const long frequency = 868E6;  // LoRa Frequency

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Axa"
#define WLAN_PASS       "Axorizer"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
// "...your AIO username (see https://accounts.adafruit.com)..."
#define AIO_USERNAME    "Vooja"
#define AIO_KEY         "aio_eiCm40uquRLGw3MXKdrZjj7MU481"


// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish hmotnost = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hmotnost");
Adafruit_MQTT_Publish teplota = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/teplota");
Adafruit_MQTT_Publish vlhkost = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/vlhkost");
Adafruit_MQTT_Publish tlak = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/tlak");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

void MQTT_connect();

String getValue(String data, char separator, int index);

//int32_t v_hmotnost = 0;
float v_hmotnost = 0;
float v_teplota = 0;
float v_vlhkost = 0;
float v_tlak = 0;

void setup() {
  Serial.begin(115200);
  delay(1000); 
  while (!Serial){
      Serial.println("Serial failed!");
      delay(1000);
      //break;
  };
  delay(1000); 
  Serial.println("LoRa Receiver init");
  LoRa.setPins(csPin, resetPin, irqPin);
    
  if (!LoRa.begin(frequency)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

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

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);
}

void loop() {
  MQTT_connect();
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
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

    String xval = getValue(receivedText, ' ', 0);
    String textHmostnost = String(receivedText[4]) + String(receivedText[5]) + String(receivedText[6]) + String(receivedText[7]);
    v_hmotnost = textHmostnost.toFloat();
    //Serial.println("receivedText: " + );
    //v_hmotnost = (xval.substring(3)).toFloat();
    v_teplota = getValue(receivedText, ' ', 1).toFloat();
    v_vlhkost = getValue(receivedText, ' ', 2).toFloat();
    v_tlak = getValue(receivedText, ' ', 3).toFloat();
    
    // Now we can publish stuff!
    Serial.print(F("\nZasilam hmotnost "));
    Serial.println(v_hmotnost);
  
    if (! hmotnost.publish(v_hmotnost)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
    
    Serial.print(F("\nZasilam teplota "));
    Serial.println(v_teplota);
    if (! teplota.publish(v_teplota)) {
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
  
    Serial.print(F("\nZasilam tlak "));
    Serial.println(v_tlak);
    if (! tlak.publish(v_tlak)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
      
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

String getValue(String data, char separator, int index)
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
}