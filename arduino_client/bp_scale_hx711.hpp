#ifndef BP_SCALE_HX711_HPP
#define BP_SCALE_HX711_HPP

#include "HX711.h"
// RobTillaart/HX711 library

// HX711 circuit wiring
const int LOADCELL_DOUT = 4;
const int LOADCELL_SCK = 5;

const float SCALE_FACTOR = 2280.0f;    // this value is obtained by calibrating the scale with known weights

HX711 scale;

void Scale_Setup()
{
     //Serial.println("Initializing the scale");
     // Initialize library with data output pin, clock input pin and gain factor.
     // Channel selection is made by passing the appropriate gain:
     // - With a gain factor of 64 or 128, channel A is selected
     // - With a gain factor of 32, channel B is selected
     // By omitting the gain factor parameter, the library
     // default "128" (Channel A) is used here.
     
     scale.begin(LOADCELL_DOUT, LOADCELL_SCK_PIN);
     
     //Serial.println("Before setting up the scale:");
     scale.set_scale(SCALE_FACTOR);
     scale.tare();                    // reset the scale to 0
    
     //Serial.println("After setting up the scale:");
     
     /*Serial.print("read: \t\t");
     Serial.println(scale.read());                 // print a raw reading from the ADC
     Serial.print("read average: \t\t");
     Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC
     Serial.print("get value: \t\t");
     Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()
     Serial.print("get units: \t\t");
     Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
    // by the SCALE parameter set with set_scale
    */
}

float getWeight()
{
    return scale.get_units() / 10;
}

void printScale()
{
     // weight/scale section
     Serial.print("reading:\t");
     Serial.print(scale.get_units() / 10, 3);
     Serial.print("\t| average:\t");
     Serial.println(scale.get_units(10) / 10, 3);
}

#endif    // !BP_SCALE_HX711_HPP
