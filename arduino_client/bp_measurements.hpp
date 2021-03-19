#ifndef BP_MEASUREMENTS_HPP
#define BP_MEASUREMENTS_HPP

struct Measurements { // celkem | 40 + 1 (null terminator)
    float weight;       // Kg   | 5
    float temperature;  // °C   | 5
    float pressure;     // hPa  | 5
    float humidity;     // %    | 5
    float altitude;     // m    | 5
    unsigned long int time;  // | 10
    //time_t time;
};

#endif    // !BP_MEASUREMENTS_HPP
