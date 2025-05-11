#pragma once

#include "Arduino.h"

#include "../Sensor.h"
#include <Adafruit_LPS2X.h>

struct LPS_DATA {
    float pressure = 0.0;
    float temperature = 0.0;
};

class LPS22 : public Sensor {
    public:

    LPS22() : Sensor(sizeof(LPS_DATA), 25), lps() {}

    LPS_DATA getData() {
        return *(LPS_DATA * ) data;
    }

    void debugPrint(Print& p) override {
        p.print("pressure: "); p.print(((LPS_DATA *)data)->pressure); p.print(", ");
        p.print("temperature: "); p.print(((LPS_DATA *)data)->temperature); p.println();
   }

   void logCsvHeader(Print& p) override {
        p.print("pressure,temperature");
   }

   void logCsvRow(Print& p) override {
        p.print(((LPS_DATA *) data)->pressure); p.print(",");
        p.print(((LPS_DATA *) data)->temperature); p.print(",");
   }

   private:
    Adafruit_LPS22 lps;

    bool init_impl() override {
        if(!lps.begin_I2C(0x5C)) {
            Serial.println("LPS22 not found");
            return false;
        }
        Serial.println("LPS22 found!");
        lps.setDataRate(LPS22_RATE_25_HZ);
        
        return true;
    }

    void *poll() override {

        LPS_DATA *data = (LPS_DATA *) data;

        sensors_event_t pressure, temperature;
        lps.getEvent(&pressure, &temperature);

        data->pressure = pressure.pressure;
        data->temperature = temperature.temperature;
        return data;
    }
    
};