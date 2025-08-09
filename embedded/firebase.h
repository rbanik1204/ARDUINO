/*
 * Firebase Integration Module for ToxiRover
 * ESP8266 Firebase Realtime Database Interface
 * 
 * Features:
 * - Firebase Realtime Database connection
 * - Real-time data synchronization
 * - Command reception from web interface
 * - Alert system integration
 * - Data logging and history
 */

#ifndef FIREBASE_H
#define FIREBASE_H

#include <FirebaseESP8266.h>

// Firebase configuration
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-secret"

// Firebase data paths
#define FIREBASE_GAS_DATA "/gas_data"
#define FIREBASE_DISTANCE_DATA "/ultrasonic_distance"
#define FIREBASE_MOTION_DATA "/motion_command"
#define FIREBASE_SERVO_DATA "/servo"
#define FIREBASE_ALERTS "/alerts"
#define FIREBASE_SENSOR_DATA "/sensor_data"
#define FIREBASE_STATUS "/status"

// Data update intervals
#define FIREBASE_UPDATE_INTERVAL 2000  // 2 seconds
#define FIREBASE_COMMAND_CHECK_INTERVAL 500  // 500ms

// Firebase connection status
enum FirebaseStatus {
  FIREBASE_DISCONNECTED,
  FIREBASE_CONNECTING,
  FIREBASE_CONNECTED,
  FIREBASE_ERROR
};

// Firebase data structure
struct FirebaseData {
  float gasConcentration;
  int distance;
  String motionCommand;
  int servoAngle;
  String lastAlert;
  unsigned long lastUpdate;
  FirebaseStatus status;
  bool isConnected;
};

// Global Firebase data
extern FirebaseData firebaseData;
extern FirebaseData firebaseDataObj;
extern FirebaseJson json;

// Function declarations
void initFirebase();
bool connectToFirebase();
void updateFirebaseData();
void sendGasData(float ppm);
void sendDistanceData(int distance);
void sendMotionCommand(String command);
void sendServoData(int angle);
void sendAlert(String alertType, String message);
void checkFirebaseCommands();
bool isFirebaseConnected();
FirebaseStatus getFirebaseStatus();
void reconnectFirebase();
void logDataToFirebase();

// Advanced Firebase functions
void createFirebaseLog(String event, String data);
void updateFirebaseStatus(String status);
void clearFirebaseCommands();
void setFirebaseUpdateInterval(int interval);
String getFirebaseData(String path);
bool setFirebaseData(String path, String value);

#endif
