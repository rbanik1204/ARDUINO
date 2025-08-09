#include <Arduino.h>
#include "gas_sensor.h"

// Global variables
static float gasConcentration = 0;
static int gasDigitalValue = 0;
static int gasAnalogValue = 0;
static GasLevel currentGasLevel = SAFE;

void initGasSensor() {
  Serial.println("🌬️ Initializing gas sensor...");
  
  // Initialize gas sensor pins
  pinMode(GAS_DIGITAL_PIN, INPUT);
  
  Serial.print("🔧 Gas sensor pins: ");
  Serial.print("Digital="); Serial.print(GAS_DIGITAL_PIN);
  Serial.print(", Analog="); Serial.println(GAS_ANALOG_PIN);
  
  // Initial reading
  gasAnalogValue = analogRead(GAS_ANALOG_PIN);
  gasDigitalValue = digitalRead(GAS_DIGITAL_PIN);
  
  Serial.println("✅ Gas sensor initialized successfully!");
}

float readGasSensor() {
  // Read analog value
  gasAnalogValue = analogRead(GAS_ANALOG_PIN);
  
  // Read digital value
  gasDigitalValue = digitalRead(GAS_DIGITAL_PIN);
  
  // Convert analog reading to PPM (approximate conversion)
  // This conversion depends on your specific sensor model
  gasConcentration = (float)gasAnalogValue * 2.0; // Adjust multiplier based on your sensor
  
  // Update gas level
  if (gasConcentration >= GAS_DANGER_THRESHOLD) {
    currentGasLevel = DANGER;
  } else if (gasConcentration >= GAS_WARNING_THRESHOLD) {
    currentGasLevel = WARNING;
  } else if (gasConcentration >= GAS_SAFE_THRESHOLD) {
    currentGasLevel = SAFE;
  } else {
    currentGasLevel = SAFE;
  }
  
  return gasConcentration;
}

int readGasDigital() {
  gasDigitalValue = digitalRead(GAS_DIGITAL_PIN);
  return gasDigitalValue;
}

int readGasAnalog() {
  gasAnalogValue = analogRead(GAS_ANALOG_PIN);
  return gasAnalogValue;
}

String getGasLevel() {
  return getGasLevelString(currentGasLevel);
}

bool isGasDetected() {
  return gasDigitalValue == HIGH || gasConcentration > GAS_SAFE_THRESHOLD;
}

bool isGasDangerous() {
  return currentGasLevel == DANGER;
}

void calibrateGasSensor() {
  Serial.println("🔧 Calibrating gas sensor...");
  
  // Take multiple readings and average them
  int totalReading = 0;
  int numReadings = 10;
  
  for (int i = 0; i < numReadings; i++) {
    totalReading += analogRead(GAS_ANALOG_PIN);
    delay(100);
  }
  
  int averageReading = totalReading / numReadings;
  Serial.print("📊 Average reading: ");
  Serial.println(averageReading);
  
  // You can store this as baseline for future comparisons
  // EEPROM.write(100, averageReading); // Example storage
}

float getGasConcentration() {
  return gasConcentration;
}

String getGasLevelString(GasLevel level) {
  switch (level) {
    case SAFE: return "SAFE";
    case WARNING: return "WARNING";
    case DANGER: return "DANGER";
    case ERROR: return "ERROR";
    default: return "UNKNOWN";
  }
}

// Additional utility functions
void printGasSensorInfo() {
  Serial.println("🌬️ Gas Sensor Information:");
  Serial.print("  Digital Value: "); Serial.println(gasDigitalValue);
  Serial.print("  Analog Value: "); Serial.println(gasAnalogValue);
  Serial.print("  Concentration: "); Serial.print(gasConcentration); Serial.println(" ppm");
  Serial.print("  Level: "); Serial.println(getGasLevel());
}

bool isGasWarning() {
  return currentGasLevel == WARNING;
}

void resetGasSensor() {
  gasConcentration = 0;
  gasDigitalValue = 0;
  gasAnalogValue = 0;
  currentGasLevel = SAFE;
  Serial.println("🔄 Gas sensor reset");
}