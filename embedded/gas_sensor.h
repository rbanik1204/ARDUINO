#ifndef GAS_SENSOR_H
#define GAS_SENSOR_H

#include <Arduino.h>
#include "pin_config.h"

// Gas sensor constants
#define GAS_DANGER_THRESHOLD 500  // ppm - adjust based on your sensor
#define GAS_WARNING_THRESHOLD 300 // ppm
#define GAS_SAFE_THRESHOLD 100    // ppm

// Gas sensor pins (updated to avoid conflicts)
#define GAS_DIGITAL_PIN 0         // D3 - FC-22 D0 (digital output) - CHANGED from GPIO 5
#define GAS_ANALOG_PIN A0         // A0 - FC-22 AD (analog output)

// Function declarations
void initGasSensor();
float readGasSensor();
int readGasDigital();
int readGasAnalog();
String getGasLevel();
bool isGasDetected();
bool isGasDangerous();
void calibrateGasSensor();
float getGasConcentration();

// Gas sensor status
enum GasLevel {
  SAFE,
  WARNING,
  DANGER,
  ERROR
};

// Function to get gas level as string
String getGasLevelString(GasLevel level);

#endif