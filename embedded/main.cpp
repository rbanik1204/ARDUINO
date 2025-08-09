#include "wifi_Control.cpp"
#include "WANConnection.cpp"
#include <Arduino.h>
#include "UltrasonicServo.h"
#include "gas_sensor.h"
#include "pin_config.h"

// Create UltrasonicServo object with correct pins
UltrasonicServo ultraServo(ULTRASONIC_SERVO_TRIG, ULTRASONIC_SERVO_ECHO, 
                           ULTRASONIC_SERVO_SERVO, ULTRASONIC_SERVO_MOTOR1, 
                           ULTRASONIC_SERVO_MOTOR2);

void setup() {
  Serial.begin(9600);
  Serial.println("🚀 ToxiRover Starting (Modular Version)...");
  
  // Initialize WiFi control
  setupWiFi();
  
  // Initialize all your modules here:
  initGasSensor();        // from gas_sensor.cpp
  ultraServo.begin();     // Initialize UltrasonicServo
  setupWAN();            // from WANconnection.cpp
  
  Serial.println("✅ ToxiRover initialized successfully!");
}

void loop() {
  // Handle WiFi client requests
  handleClient();
  
  // Handle other modules
  readGasSensor();        // Monitor gas (changed from loopGasSensor)
  ultraServo.checkAndAct(); // Check obstacles and respond
  loopWAN();             // Handle MQTT commands
  
  delay(500);
  
  if (isGasDetected()) {
    Serial.println("⚠️ Gas detected!");
  } else {
    Serial.println("✅ No gas detected.");
  }
}
