#pragma once

#include "Arduino.h"
#include "../Sensor.h"
#include <SparkFun_u-blox_GNSS_v3.h>

struct GPS_DATA {
    float lat = 0.0;
    float lon = 0.0;
    float altMSL = 0.0;
    float altWGS = 0.0;
    uint32_t epochTime  = 0;
    uint8_t satellites  = 0;
    uint8_t fixType     = 0;
    float velN         = 0.0;
    float velE         = 0.0;
    float velD         = 0.0;
};

class MAX10S : public Sensor {
    public:

    MAX10S() : Sensor(sizeof(GPS_DATA), 25), GPS() {}

    GPS_DATA getData() {
        return *(GPS_DATA * ) data;
    }

    void debugPrint(Print& p) override {
        p.print("lat: "); p.print(((GPS_DATA *)data)->lat); p.print(", ");
        p.print("lon: "); p.print(((GPS_DATA *)data)->lon); p.print(", ");
        p.print("altMSL: "); p.print(((GPS_DATA *)data)->altMSL); p.print(", ");
        p.print("altEll: "); p.print(((GPS_DATA *)data)->altWGS); p.print(", ");
        p.print("velN: "); p.print(((GPS_DATA *)data)->velN); p.print(", ");
        p.print("velE: "); p.print(((GPS_DATA *)data)->velE); p.print(", ");
        p.print("velD: "); p.print(((GPS_DATA *)data)->velD); p.print(", ");
        p.print("epochTime: "); p.print(((GPS_DATA *)data)->epochTime); p.print(", ");
        p.print("satellites: "); p.print(((GPS_DATA *)data)->satellites); p.print(", ");
        p.print("gpsLockType: "); p.print(((GPS_DATA *)data)->fixType); p.println();
   }

   void logCsvHeader(Print& p) override {
        p.print("lat,lon,altMSL,altEll,velN,velE,velD,epochTime,satellites,gpsLockType");
   }

   void logCsvRow(Print& p) override {
        p.print(((GPS_DATA *)data)->lat); p.print(",");
        p.print(((GPS_DATA *)data)->lon); p.print(",");
        p.print(((GPS_DATA *)data)->altMSL); p.print(",");
        p.print(((GPS_DATA *)data)->altWGS); p.print(",");
        p.print(((GPS_DATA *)data)->velN); p.print(",");
        p.print(((GPS_DATA *)data)->velE); p.print(",");
        p.print(((GPS_DATA *)data)->velD); p.print(",");
        p.print(((GPS_DATA *)data)->epochTime); p.print(",");
        p.print(((GPS_DATA *)data)->satellites); p.print(",");
        p.print(((GPS_DATA *)data)->fixType);
   }

    private:
    SFE_UBLOX_GNSS GPS;

    bool init_impl() override {
        if(GPS.begin()) {
            Serial.println("[GPS] Initialized!");

            GPS.setNavigationFrequency(40);
            GPS.setAutoPVT(true);

            return true;
        }

        return false;
    }

    void *poll() override {
        GPS_DATA *gpsData = (GPS_DATA *) data;

        if(GPS.getPVT()) {
            gpsData->lat = GPS.getLatitude();
            gpsData->lon = GPS.getLongitude();
            gpsData->altMSL = GPS.getAltitudeMSL();
            gpsData->altWGS = GPS.getAltitude();
            gpsData->epochTime = GPS.getUnixEpoch();
            gpsData->satellites = GPS.getSIV();
            gpsData->fixType = GPS.getFixType();
        }

        return data;
    }

};