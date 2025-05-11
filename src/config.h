#pragma once

#include "Arduino.h"
#include <SdFat.h>

#include "Sensors/Impl/MAX10S.h"
#include "Sensors/Impl/LPS22.h"
#include "Sensors/Impl/LSM303.h"

#define SD_SPI_SPEED SD_SCK_MHZ(50)

#define SD_CS PA4
#define SD_SCLK PA5
#define SD_MISO PA6
#define SD_MOSI PA7

// #define XBEE_CS PA4
// #define XBEE_SCLK PA5
// #define XBEE_MISO PA6
// #define XBEE_MOSI PA7
// #define XBEE_ATTN PD8

#define SENSOR_SCL PB6
#define SENSOR_SDA PB7
// #define GROUNDSTATION_XBEE_ADDRESS 0x0013A200423F474C
