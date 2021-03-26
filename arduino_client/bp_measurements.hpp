#ifndef BP_MEASUREMENTS_HPP
#define BP_MEASUREMENTS_HPP

struct Measurements {   // celkem | 40 + 1 (null terminator)
    float weight;       // Kg   | 5
    float temperature;  // °C   | 5
    float pressure;     // hPa  | 5
    float humidity;     // %    | 5
    float altitude;     // m    | 5
    //unsigned long int time;  // | 10  // time_t
};

String createMessage(Measurements measurements)
{
    String msg = "";
    msg += " ";
    msg += "arduino_1";
    msg += " ";
    msg += measurements.weight;
    msg += " ";
    msg += measurements.temperature;
    msg += " ";
    msg += measurements.pressure;
    msg += " ";
    msg += measurements.humidity;
    //msg += " ";
    //msg += measurements.altitude;
    
    return msg;
}
#endif    // !BP_MEASUREMENTS_HPP
