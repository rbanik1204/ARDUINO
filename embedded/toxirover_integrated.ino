/*
 * ToxiRover - Smart IoT Gas Sensing Rover (Integrated Version)
 * Main Arduino sketch for ESP8266 with UltrasonicServo integration
 * 
 * Features:
 * - FC-22 Gas Sensor monitoring
 * - HC-SR04 Ultrasonic distance sensing with UltrasonicServo class
 * - L298N Motor control (via WiFi and WAN systems)
 * - Servo motor control
 * - Firebase Realtime Database integration
 * - WiFi connectivity
 * - Obstacle avoidance with automatic motor/servo response
 */

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#include <NewPing.h>
#include "gas_sensor.h"
#include "servo_control.h"
#include "ultrasonic.h"
#include "firebase.h"
#include "UltrasonicServo.h"
#include "pin_config.h"

// WiFi Configuration
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Firebase Configuration
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-secret"

// Global Objects
FirebaseData firebaseData;
FirebaseJson json;
Servo gasServo;
NewPing sonar(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, 200);

// UltrasonicServo object for advanced obstacle avoidance
UltrasonicServo ultraServo(ULTRASONIC_SERVO_TRIG, ULTRASONIC_SERVO_ECHO, 
                           ULTRASONIC_SERVO_SERVO, ULTRASONIC_SERVO_MOTOR1, 
                           ULTRASONIC_SERVO_MOTOR2);

// Timing variables
unsigned long lastGasReading = 0;
unsigned long lastDistanceReading = 0;
unsigned long lastFirebaseUpdate = 0;
unsigned long lastUltrasonicServoCheck = 0;
const unsigned long GAS_READ_INTERVAL = 1000;    // 1 second
const unsigned long DISTANCE_READ_INTERVAL = 500; // 500ms
const unsigned long FIREBASE_UPDATE_INTERVAL = 2000; // 2 seconds
const unsigned long ULTRASONIC_SERVO_CHECK_INTERVAL = 100; // 100ms

// Sensor data
float gasConcentration = 0;
int distance = 0;
String currentMotion = "STOP";
int servoAngle = 90;

// UltrasonicServo status
bool obstacleDetected = false;
String ultrasonicServoStatus = "SAFE";
float ultrasonicServoDistance = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 ToxiRover Starting (Integrated Version)...");
  
  // Print pin configuration
  Serial.println("🔧 Pin Configuration:");
  Serial.print("  Gas Sensor: A0");
  Serial.print("  Ultrasonic TRIG: "); Serial.print(ULTRASONIC_TRIG_PIN);
  Serial.print("  ECHO: "); Serial.println(ULTRASONIC_ECHO_PIN);
  Serial.print("  Servo: "); Serial.print(SERVO_PIN);
  Serial.print("  Motor ENA: "); Serial.print(ENA);
  Serial.print("  IN1: "); Serial.print(IN1);
  Serial.print("  IN2: "); Serial.print(IN2);
  Serial.print("  IN3: "); Serial.print(IN3);
  Serial.print("  IN4: "); Serial.print(IN4);
  Serial.print("  ENB: "); Serial.println(ENB);
  
  // Initialize WiFi
  initWiFi();
  
  // Initialize Firebase
  initFirebase();
  
  // Initialize sensors and actuators
  initGasSensor();
  initServo();
  initUltrasonic();
  
  // Initialize UltrasonicServo
  ultraServo.begin();
  
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
  
  // Read ultrasonic sensor (basic)
  if (millis() - lastDistanceReading >= DISTANCE_READ_INTERVAL) {
    distance = readDistance();
    lastDistanceReading = millis();
    
    // Basic obstacle avoidance
    if (distance < OBSTACLE_THRESHOLD && currentMotion != "STOP") {
      Serial.println("🚫 Obstacle detected! Stopping...");
      // Note: Motor control is handled by WiFi/WAN systems
      currentMotion = "STOP";
    }
  }
  
  // Check UltrasonicServo (advanced obstacle avoidance)
  if (millis() - lastUltrasonicServoCheck >= ULTRASONIC_SERVO_CHECK_INTERVAL) {
    ultraServo.checkAndAct();
    lastUltrasonicServoCheck = millis();
    
    // Update status variables
    obstacleDetected = ultraServo.isObstacleDetected();
    ultrasonicServoStatus = ultraServo.getDistanceStatus();
    ultrasonicServoDistance = ultraServo.getLastDistance();
    
    // Log status changes
    if (obstacleDetected) {
      Serial.println("🚨 UltrasonicServo: Obstacle detected and action taken!");
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
  
  // Add UltrasonicServo data
  json.add("ultrasonic_servo_distance", ultrasonicServoDistance);
  json.add("ultrasonic_servo_status", ultrasonicServoStatus);
  json.add("obstacle_detected", obstacleDetected);
  
  Firebase.updateNode(firebaseData, "/sensor_data", json);
  
  // Update individual values for easier access
  Firebase.setFloat(firebaseData, "/gas_data/ppm", gasConcentration);
  Firebase.setInt(firebaseData, "/ultrasonic_distance", distance);
  Firebase.setString(firebaseData, "/motion_command/current", currentMotion);
  Firebase.setInt(firebaseData, "/servo/angle", servoAngle);
  
  // UltrasonicServo specific data
  Firebase.setFloat(firebaseData, "/ultrasonic_servo/distance", ultrasonicServoDistance);
  Firebase.setString(firebaseData, "/ultrasonic_servo/status", ultrasonicServoStatus);
  Firebase.setBool(firebaseData, "/ultrasonic_servo/obstacle_detected", obstacleDetected);
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
  // Note: Actual motor control is handled by WiFi/WAN systems
  // This function just updates the status
  if (command == "FORWARD") {
    currentMotion = "FORWARD";
  } else if (command == "BACKWARD") {
    currentMotion = "BACKWARD";
  } else if (command == "LEFT") {
    currentMotion = "LEFT";
  } else if (command == "RIGHT") {
    currentMotion = "RIGHT";
  } else if (command == "STOP") {
    currentMotion = "STOP";
  }
}

// Emergency stop function that can be called from Firebase
void emergencyStop() {
  Serial.println("🛑 Emergency stop activated from Firebase!");
  currentMotion = "STOP";
  ultraServo.emergencyStop();
  
  // Update Firebase
  Firebase.setString(firebaseData, "/motion_command/current", "STOP");
  Firebase.setBool(firebaseData, "/emergency_stop", true);
}
