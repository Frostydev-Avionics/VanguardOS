#include <Arduino.h>

#include <Wire.h>
#include <SPI.h>

#include "config.h"
#include "Context.h"

#include "SensorManager.h"

Context ctx = {
    .gps = MAX10S(),
    .lps = LPS22(),
    .lsm = LSM303(),
    .sd = SdFs()
};

long lastTimeSlow = 0;
long lastTimeFast = 0;
bool sdInitialized = false;

Sensor *sensors[] = {
    &ctx.gps,
    &ctx.lps,
    &ctx.lsm,
};

SensorManager<decltype(&millis), sizeof(sensors)/sizeof(Sensor *)> sensorManager(sensors, millis);

void setup() {

    Serial.begin(115200);

    while(!Serial) {
        delay(10);
    }

    // Configure SPI Pins
    Wire.setSCL(SENSOR_SCL);
    Wire.setSDA(SENSOR_SDA);

    Wire.begin();
    Wire.setClock(400000); // 400kHz

    SPI.setSCLK(SD_SCLK);
    SPI.setMISO(SD_MISO);
    SPI.setMOSI(SD_MOSI);
    SPI.begin();

    sdInitialized = ctx.sd.begin(SD_CS, SD_SPI_SPEED);

    if (!sdInitialized) {
        Serial.println("[SD] ERROR: Initialization failed!");
        ctx.sd.printSdError(&Serial);
    }

    if(sdInitialized) {
        int fileIdx = 0;
        char fileName[100];

        while(fileIdx < 100) {
            sprintf(fileName, "flightData_%d.csv", fileIdx++);

            if(!ctx.sd.exists(fileName)) {
                ctx.logFile = ctx.sd.open(fileName, O_RDWR | O_CREAT | O_TRUNC);
                break;
            }
        }
    }

    sensorManager.sensorInit();

    if(ctx.logFile) {
        ctx.logCsvHeader();
    }

    lastTimeSlow = millis();
    lastTimeFast = millis();
}

void loop() {
    long now = millis();
    sensorManager.loop();

    if(now - lastTimeSlow >= 1000) {
        ctx.gps.debugPrint(Serial);
        ctx.lps.debugPrint(Serial);
        ctx.lsm.debugPrint(Serial);

        ctx.logFile.flush();

        lastTimeSlow = now;
    }

    if(now - lastTimeFast >= 10) {
        // Serial.println("Hello world");
        if(sdInitialized && ctx.logFile) {
            if(ctx.logFile) {
                ctx.logFile.print(now);
                ctx.logFile.print(",");
                ctx.gps.logCsvRow(ctx.logFile);
                ctx.logFile.print(",");
                ctx.lps.logCsvRow(ctx.logFile);
                ctx.logFile.print(",");
                ctx.lsm.logCsvRow(ctx.logFile);
                ctx.logFile.println();
            }
        }

        lastTimeFast = now;
    }

    delay(10);
}