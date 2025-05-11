#pragma once

#include <config.h>

struct Context {

    MAX10S gps;
    LPS22 lps;
    LSM303 lsm;

    SdFs sd;
    File logFile;

    void logCsvHeader() {
        logFile.print("timestamp,");
        gps.logCsvHeader(logFile);
        logFile.print(",");
        lps.logCsvHeader(logFile);
        logFile.print(",");
        lsm.logCsvHeader(logFile);
        logFile.println();
    }

};

