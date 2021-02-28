#ifndef BP_MEASUREMENTS_HPP
#define BP_MEASUREMENTS_HPP

struct Measurements { // celkem | 40 + 1 (null terminator)
    unsigned int id;  //        5 chars | 0 to 65,535
    int weight;       // Kg   | 5
    int temperature;  // °C   | 5
    int pressure;     // hPa  | 5
    int humidity;     // %    | 5
    int altitude;     // m    | 5
    unsigned long int time;  // | 10
    //time_t time;
};

#endif    // !BP_MEASUREMENTS_HPP
