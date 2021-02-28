#ifndef BP_BME280_HPP
#define BP_BME280_HPP

// --- BME280 - teplota, vlhkost, tlak
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "bp_measurements.hpp"

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

// ---- BME280 - teplota, vlhkost, tlak
void BME280_Setup()
{
  // BME280
  if (!bme.begin(0x76)) {
    Serial.println("Error BME280 sensor!");
    //Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

Measurements getMeasurements(){
    Measurements m;
    m.temperature = bme.readTemperature();
    m.pressure = bme.readPressure() / 100.0f;
    m.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    m.humidity = bme.readHumidity();
    return m;
}

void printBME280()
{
    float temperature = bme.readTemperature();
    float pressure = bme.readPressure() / 100.0f;
    float altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    float humidity = bme.readHumidity();
    
    Serial.println("Temperature = " + String(temperature) + " *C");
    Serial.println("Pressure = " + String(pressure) + " hPa");
    Serial.println("Approx. Altitude = " + String(altitude) + " m");
    Serial.println("Humidity = " + String(humidity) + " %");
    Serial.println();
}

  
#endif    // !BP_BME280_HPP
