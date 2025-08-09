/*
 * ToxiRover - Smart IoT Gas Sensing Rover
 * Main Arduino sketch for ESP8266
 * 
 * Features:
 * - FC-22 Gas Sensor monitoring
 * - HC-SR04 Ultrasonic distance sensing
 * - L298N Motor control
 * - Servo motor control
 * - Firebase Realtime Database integration
 * - WiFi connectivity
 */

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#include <NewPing.h>
#include "gas_sensor.h"
#include "motion.h"
#include "servo_control.h"
#include "ultrasonic.h"
#include "firebase.h"

// WiFi Configuration
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Firebase Configuration
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-secret"

// Pin Definitions
#define GAS_SENSOR_PIN A0
#define ULTRASONIC_TRIG_PIN D5
#define ULTRASONIC_ECHO_PIN D6
#define SERVO_PIN D7

// Motor Driver Pins (L298N)
#define ENA D1  // Left motor enable
#define IN1 D2  // Left motor input 1
#define IN2 D3  // Left motor input 2
#define IN3 D4  // Right motor input 3
#define IN4 D8  // Right motor input 4
#define ENB D0  // Right motor enable

// Global Objects
FirebaseData firebaseData;
FirebaseJson json;
Servo gasServo;
NewPing sonar(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, 200);

// Timing variables
unsigned long lastGasReading = 0;
unsigned long lastDistanceReading = 0;
unsigned long lastFirebaseUpdate = 0;
const unsigned long GAS_READ_INTERVAL = 1000;    // 1 second
const unsigned long DISTANCE_READ_INTERVAL = 500; // 500ms
const unsigned long FIREBASE_UPDATE_INTERVAL = 2000; // 2 seconds

// Sensor data
float gasConcentration = 0;
int distance = 0;
String currentMotion = "STOP";
int servoAngle = 90;

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 ToxiRover Starting...");
  
  // Initialize WiFi
  initWiFi();
  
  // Initialize Firebase
  initFirebase();
  
  // Initialize sensors and actuators
  initGasSensor();
  initMotion();
  initServo();
  initUltrasonic();
  
  Serial.println("✅ ToxiRover initialized successfully!");
}

void loop() {
  // Read gas sensor
  if (millis() - lastGasReading >= GAS_READ_INTERVAL) {
    gasConcentration = readGasSensor();
    lastGasReading = millis();
    
    // Check for dangerous gas levels
    if (gasConcentration > GAS_DANGER_THRESHOLD) {
      Serial.println("⚠️ DANGER: High gas concentration detected!");
      triggerGasAlert();
    }
  }
  
  // Read ultrasonic sensor
  if (millis() - lastDistanceReading >= DISTANCE_READ_INTERVAL) {
    distance = readDistance();
    lastDistanceReading = millis();
    
    // Obstacle avoidance
    if (distance < OBSTACLE_THRESHOLD && currentMotion != "STOP") {
      Serial.println("🚫 Obstacle detected! Stopping...");
      stopMotion();
      currentMotion = "STOP";
    }
  }
  
  // Update Firebase
  if (millis() - lastFirebaseUpdate >= FIREBASE_UPDATE_INTERVAL) {
    updateFirebaseData();
    lastFirebaseUpdate = millis();
  }
  
  // Check for motion commands from Firebase
  checkMotionCommands();
  
  // Check for servo commands from Firebase
  checkServoCommands();
  
  delay(10); // Small delay to prevent watchdog reset
}

void initWiFi() {
  Serial.print("📡 Connecting to WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("✅ WiFi connected!");
  Serial.print("📶 IP Address: ");
  Serial.println(WiFi.localIP());
}

void triggerGasAlert() {
  // Send alert to Firebase
  Firebase.setString(firebaseData, "/alerts/last_alert", "HIGH_GAS_LEVEL");
  Firebase.setFloat(firebaseData, "/alerts/gas_level", gasConcentration);
  Firebase.setString(firebaseData, "/alerts/timestamp", String(millis()));
  
  // Optional: Activate servo for gas dispersal
  rotateServo(180);
  delay(1000);
  rotateServo(90);
}

void updateFirebaseData() {
  // Update sensor data
  json.clear();
  json.add("gas_concentration", gasConcentration);
  json.add("distance", distance);
  json.add("motion", currentMotion);
  json.add("servo_angle", servoAngle);
  json.add("timestamp", millis());
  
  Firebase.updateNode(firebaseData, "/sensor_data", json);
  
  // Update individual values for easier access
  Firebase.setFloat(firebaseData, "/gas_data/ppm", gasConcentration);
  Firebase.setInt(firebaseData, "/ultrasonic_distance", distance);
  Firebase.setString(firebaseData, "/motion_command/current", currentMotion);
  Firebase.setInt(firebaseData, "/servo/angle", servoAngle);
}

void checkMotionCommands() {
  String command;
  if (Firebase.getString(firebaseData, "/motion_command/request", command)) {
    if (command != currentMotion) {
      executeMotion(command);
      currentMotion = command;
      Serial.print("🎮 Motion command: ");
      Serial.println(command);
    }
  }
}

void checkServoCommands() {
  int angle;
  if (Firebase.getInt(firebaseData, "/servo/request", angle)) {
    if (angle >= 0 && angle <= 180) {
      rotateServo(angle);
      servoAngle = angle;
      Serial.print("⚙️ Servo angle: ");
      Serial.println(angle);
    }
  }
}

void executeMotion(String command) {
  if (command == "FORWARD") {
    moveForward();
  } else if (command == "BACKWARD") {
    moveBackward();
  } else if (command == "LEFT") {
    turnLeft();
  } else if (command == "RIGHT") {
    turnRight();
  } else if (command == "STOP") {
    stopMotion();
  }
}
