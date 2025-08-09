/*
 * Firebase Integration Implementation for ToxiRover
 * ESP8266 Firebase Realtime Database Functions
 */

#include "firebase.h"

// Global Firebase objects
FirebaseData firebaseDataObj;
FirebaseJson json;

// Global Firebase data
FirebaseData firebaseData = {
  .gasConcentration = 0,
  .distance = 0,
  .motionCommand = "",
  .servoAngle = 90,
  .lastAlert = "",
  .lastUpdate = 0,
  .status = FIREBASE_DISCONNECTED,
  .isConnected = false
};

void initFirebase() {
  Serial.println("🔥 Initializing Firebase connection...");
  
  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  
  // Set database read timeout to 1 minute
  Firebase.setReadTimeout(firebaseDataObj, 1000 * 60);
  
  // Set database write timeout to 1 minute
  Firebase.setwriteSizeLimit(firebaseDataObj, "tiny");
  
  // Test connection
  if (connectToFirebase()) {
    Serial.println("✅ Firebase connected successfully!");
    firebaseData.status = FIREBASE_CONNECTED;
    firebaseData.isConnected = true;
    
    // Initialize Firebase data structure
    initializeFirebaseData();
  } else {
    Serial.println("❌ Firebase connection failed!");
    firebaseData.status = FIREBASE_ERROR;
    firebaseData.isConnected = false;
  }
}

bool connectToFirebase() {
  Serial.print("🔗 Connecting to Firebase: ");
  Serial.println(FIREBASE_HOST);
  
  firebaseData.status = FIREBASE_CONNECTING;
  
  // Test connection by reading a simple value
  if (Firebase.getString(firebaseDataObj, "/test")) {
    firebaseData.status = FIREBASE_CONNECTED;
    firebaseData.isConnected = true;
    return true;
  } else {
    firebaseData.status = FIREBASE_ERROR;
    firebaseData.isConnected = false;
    return false;
  }
}

void initializeFirebaseData() {
  // Initialize sensor data structure
  json.clear();
  json.add("gas_concentration", 0);
  json.add("distance", 0);
  json.add("motion", "STOP");
  json.add("servo_angle", 90);
  json.add("timestamp", millis());
  
  Firebase.updateNode(firebaseDataObj, FIREBASE_SENSOR_DATA, json);
  
  // Initialize status
  Firebase.setString(firebaseDataObj, FIREBASE_STATUS, "ONLINE");
  
  Serial.println("📊 Firebase data structure initialized");
}

void updateFirebaseData() {
  if (!isFirebaseConnected()) {
    Serial.println("⚠️ Firebase not connected, attempting reconnect...");
    reconnectFirebase();
    return;
  }
  
  // Update sensor data
  json.clear();
  json.add("gas_concentration", firebaseData.gasConcentration);
  json.add("distance", firebaseData.distance);
  json.add("motion", firebaseData.motionCommand);
  json.add("servo_angle", firebaseData.servoAngle);
  json.add("timestamp", millis());
  
  Firebase.updateNode(firebaseDataObj, FIREBASE_SENSOR_DATA, json);
  
  // Update individual values for easier access
  Firebase.setFloat(firebaseDataObj, FIREBASE_GAS_DATA "/ppm", firebaseData.gasConcentration);
  Firebase.setInt(firebaseDataObj, FIREBASE_DISTANCE_DATA, firebaseData.distance);
  Firebase.setString(firebaseDataObj, FIREBASE_MOTION_DATA "/current", firebaseData.motionCommand);
  Firebase.setInt(firebaseDataObj, FIREBASE_SERVO_DATA "/angle", firebaseData.servoAngle);
  
  firebaseData.lastUpdate = millis();
}

void sendGasData(float ppm) {
  if (!isFirebaseConnected()) return;
  
  firebaseData.gasConcentration = ppm;
  Firebase.setFloat(firebaseDataObj, FIREBASE_GAS_DATA "/ppm", ppm);
  Firebase.setString(firebaseDataObj, FIREBASE_GAS_DATA "/timestamp", String(millis()));
}

void sendDistanceData(int distance) {
  if (!isFirebaseConnected()) return;
  
  firebaseData.distance = distance;
  Firebase.setInt(firebaseDataObj, FIREBASE_DISTANCE_DATA, distance);
  Firebase.setString(firebaseDataObj, FIREBASE_DISTANCE_DATA "/timestamp", String(millis()));
}

void sendMotionCommand(String command) {
  if (!isFirebaseConnected()) return;
  
  firebaseData.motionCommand = command;
  Firebase.setString(firebaseDataObj, FIREBASE_MOTION_DATA "/current", command);
  Firebase.setString(firebaseDataObj, FIREBASE_MOTION_DATA "/timestamp", String(millis()));
}

void sendServoData(int angle) {
  if (!isFirebaseConnected()) return;
  
  firebaseData.servoAngle = angle;
  Firebase.setInt(firebaseDataObj, FIREBASE_SERVO_DATA "/angle", angle);
  Firebase.setString(firebaseDataObj, FIREBASE_SERVO_DATA "/timestamp", String(millis()));
}

void sendAlert(String alertType, String message) {
  if (!isFirebaseConnected()) return;
  
  firebaseData.lastAlert = alertType;
  
  json.clear();
  json.add("type", alertType);
  json.add("message", message);
  json.add("timestamp", millis());
  
  Firebase.pushJSON(firebaseDataObj, FIREBASE_ALERTS, json);
  
  // Also update the latest alert
  Firebase.setString(firebaseDataObj, FIREBASE_ALERTS "/last_alert", alertType);
  Firebase.setString(firebaseDataObj, FIREBASE_ALERTS "/last_message", message);
  Firebase.setString(firebaseDataObj, FIREBASE_ALERTS "/last_timestamp", String(millis()));
  
  Serial.print("🚨 Alert sent to Firebase: ");
  Serial.println(alertType);
}

void checkFirebaseCommands() {
  if (!isFirebaseConnected()) return;
  
  // Check for motion commands
  String motionCommand;
  if (Firebase.getString(firebaseDataObj, FIREBASE_MOTION_DATA "/request", motionCommand)) {
    if (motionCommand != firebaseData.motionCommand && motionCommand != "") {
      firebaseData.motionCommand = motionCommand;
      Serial.print("📡 Motion command received: ");
      Serial.println(motionCommand);
      
      // Clear the request after processing
      Firebase.deleteNode(firebaseDataObj, FIREBASE_MOTION_DATA "/request");
    }
  }
  
  // Check for servo commands
  int servoAngle;
  if (Firebase.getInt(firebaseDataObj, FIREBASE_SERVO_DATA "/request", servoAngle)) {
    if (servoAngle >= 0 && servoAngle <= 180) {
      firebaseData.servoAngle = servoAngle;
      Serial.print("📡 Servo command received: ");
      Serial.println(servoAngle);
      
      // Clear the request after processing
      Firebase.deleteNode(firebaseDataObj, FIREBASE_SERVO_DATA "/request");
    }
  }
}

bool isFirebaseConnected() {
  return firebaseData.isConnected && firebaseData.status == FIREBASE_CONNECTED;
}

FirebaseStatus getFirebaseStatus() {
  return firebaseData.status;
}

void reconnectFirebase() {
  Serial.println("🔄 Attempting Firebase reconnection...");
  
  firebaseData.status = FIREBASE_CONNECTING;
  firebaseData.isConnected = false;
  
  // Wait a bit before reconnecting
  delay(1000);
  
  if (connectToFirebase()) {
    Serial.println("✅ Firebase reconnected successfully!");
    firebaseData.status = FIREBASE_CONNECTED;
    firebaseData.isConnected = true;
  } else {
    Serial.println("❌ Firebase reconnection failed!");
    firebaseData.status = FIREBASE_ERROR;
    firebaseData.isConnected = false;
  }
}

void logDataToFirebase() {
  if (!isFirebaseConnected()) return;
  
  // Create a log entry
  json.clear();
  json.add("gas_ppm", firebaseData.gasConcentration);
  json.add("distance_cm", firebaseData.distance);
  json.add("motion", firebaseData.motionCommand);
  json.add("servo_angle", firebaseData.servoAngle);
  json.add("timestamp", millis());
  
  Firebase.pushJSON(firebaseDataObj, "/logs", json);
}

void createFirebaseLog(String event, String data) {
  if (!isFirebaseConnected()) return;
  
  json.clear();
  json.add("event", event);
  json.add("data", data);
  json.add("timestamp", millis());
  
  Firebase.pushJSON(firebaseDataObj, "/event_logs", json);
}

void updateFirebaseStatus(String status) {
  if (!isFirebaseConnected()) return;
  
  Firebase.setString(firebaseDataObj, FIREBASE_STATUS, status);
}

void clearFirebaseCommands() {
  if (!isFirebaseConnected()) return;
  
  Firebase.deleteNode(firebaseDataObj, FIREBASE_MOTION_DATA "/request");
  Firebase.deleteNode(firebaseDataObj, FIREBASE_SERVO_DATA "/request");
}

String getFirebaseData(String path) {
  if (!isFirebaseConnected()) return "";
  
  String data;
  if (Firebase.getString(firebaseDataObj, path, data)) {
    return data;
  }
  return "";
}

bool setFirebaseData(String path, String value) {
  if (!isFirebaseConnected()) return false;
  
  return Firebase.setString(firebaseDataObj, path, value);
}
