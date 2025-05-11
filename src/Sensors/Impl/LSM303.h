#pragma once

#include "Arduino.h"

#include "../Sensor.h"
#include <Adafruit_LSM303_U.h>

struct LSM303_DATA {
    float accX = 0.0;
    float accY = 0.0;
    float accZ = 0.0;
    float magX = 0.0;
    float magY = 0.0;
    float magZ = 0.0;
};

class LSM303 : public Sensor {
    public:

    LSM303() : Sensor(sizeof(LSM303_DATA), 100), lsm() {}

    LSM303_DATA getData() {
        return *(LSM303_DATA * ) data;
    }

    void debugPrint(Print& p) override {
        p.print("accX: "); p.print(((LSM303_DATA *)data)->accX); p.print(", ");
        p.print("accY: "); p.print(((LSM303_DATA *)data)->accY); p.print(", ");
        p.print("accZ: "); p.print(((LSM303_DATA *)data)->accZ); p.print(", ");
        p.print("magX: "); p.print(((LSM303_DATA *)data)->magX); p.print(", ");
        p.print("magY: "); p.print(((LSM303_DATA *)data)->magY); p.print(", ");
        p.print("magZ: "); p.print(((LSM303_DATA *)data)->magZ); p.println();
   }
    void logCsvHeader(Print& p) override {
        p.print("accX,accY,accZ,magX,magY,magZ");
    }

    void logCsvRow(Print& p) override {
        p.print(((LSM303_DATA *)data)->accX); p.print(",");
        p.print(((LSM303_DATA *)data)->accY); p.print(",");
        p.print(((LSM303_DATA *)data)->accZ); p.print(",");
        p.print(((LSM303_DATA *)data)->magX); p.print(",");
        p.print(((LSM303_DATA *)data)->magY); p.print(",");
        p.print(((LSM303_DATA *)data)->magZ);
    }

    private:
    Adafruit_LSM303_Accel_Unified lsm;
    Adafruit_LSM303_Mag_Unified lsmMag;

    bool init_impl() override {
        if(lsm.begin() && lsmMag.begin()) {
            Serial.println("LSM303 found!");
            return true;
        }
        Serial.println("LSM303 not found");
        return false;

    }

    void *poll() override {

        LSM303_DATA *data = (LSM303_DATA *) data;

        sensors_event_t event;
        lsm.getEvent(&event);
        data->accX = event.acceleration.x;
        data->accY = event.acceleration.y;
        data->accZ = event.acceleration.z;

        lsmMag.getEvent(&event);
        data->magX = event.magnetic.x;
        data->magY = event.magnetic.y;
        data->magZ = event.magnetic.z;

        return data;
    }

};