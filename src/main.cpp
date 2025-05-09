#include <Arduino.h>

#include "Wire.h"
#include <SPI.h>

#include "config.h"


long lastTime = 0;

void setup() {

    Serial.begin(115200);

    // Configure SPI Pins
    Wire.setSCL(SENSOR_SCL);
    Wire.setSDA(SENSOR_SDA);

    Wire.begin();
    Wire.setClock(400000);

    SPI.setSCLK(SD_SCLK);
    SPI.setMISO(SD_MISO);
    SPI.setMOSI(SD_MOSI);
    SPI.begin();

// #if defined(MARS)
//     // P_Good pins
//     pinMode(PE0, OUTPUT); // PG3V3_LED
//     pinMode(PE1, OUTPUT); // PG5V_LED
//     pinMode(PA3, INPUT);  // PG3V3
//     pinMode(PC4, INPUT);  // PG5V

//     digitalWrite(PE0, digitalRead(PA3));
//     digitalWrite(PE1, digitalRead(PC4));
// #endif
//     Serial.begin(9600);

//     ctx.airbrakes.init();

//     Wire.setSCL(SENSOR_SCL);
//     Wire.setSDA(SENSOR_SDA);
//     Wire.begin();

// #if defined(MARS)
//     SPI.setSCLK(SD_SCLK);
// #elif defined(POLARIS)
//     SPI.setSCK(SD_SCLK);
// #endif
//     SPI.setMISO(SD_MISO);
//     SPI.setMOSI(SD_MOSI);
//     SPI.begin();

//     // while (!Serial)
//     //     delay(5);

//     stateMachine.initialize();
//     sensorManager.sensorInit();

//     Wire.setClock(400000);

//     pinMode(LED_PIN, OUTPUT);

// #if defined(MARS)
//     sd_initialized = ctx.sd.begin(SD_CS, SD_SPI_SPEED);
//     error_code = ctx.sd.card()->errorCode();
// #elif defined(POLARIS)
//     sd_initialized = SD.begin(SD_CS);
//     // error_code = SD.sdfs.card()->errorCode();
// #endif

//     // Serial.printf("%d %d\n", sd_initialized, error_code);

//     if (sd_initialized) {
//         int fileIdx = 0;
//         char filename[100];
//         while (fileIdx < 100) {
//             sprintf(filename, "flightData%d.csv", fileIdx++);

//             Serial.printf("Trying file `%s`\n", filename);
// #if defined(MARS)
//             if (!ctx.sd.exists(filename)) {
//                 ctx.logFile = ctx.sd.open(filename, O_RDWR | O_CREAT | O_TRUNC);
//                 break;
//             }
// #elif defined(POLARIS)
//             if (!SD.exists(filename)) {
//                 ctx.logFile = SD.open(filename, FILE_WRITE_BEGIN);
//                 break;
//             }
// #endif
//         }
//     }

//     if (ctx.logFile) {
//         ctx.logCsvHeader();
//     }

// #if defined(MARS)
//     xbee_spi.begin();
// #endif

    lastTime = millis();
}

void loop() {
    long now = millis();

    if(now - lastTime >= 100) {
        lastTime = now;

        Serial.println("Hello world");
    }

    delay(1);
}