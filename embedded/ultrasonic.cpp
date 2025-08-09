#include "ultrasonic.h"

// Global ultrasonic sensor object
extern NewPing sonar;

void initUltrasonic() {
  Serial.println("🔍 Initializing ultrasonic sensor...");
  Serial.print("🔧 Ultrasonic pins: TRIG=");
  Serial.print(ULTRASONIC_TRIG_PIN);
  Serial.print(", ECHO=");
  Serial.println(ULTRASONIC_ECHO_PIN);
}

int readDistance() {
  int distance = sonar.ping_cm();
  if (distance == 0) {
    distance = MAX_DISTANCE; // No obstacle detected
  }
  return distance;
}

int getAverageDistance() {
  int total = 0;
  int readings = 5;
  
  for (int i = 0; i < readings; i++) {
    total += readDistance();
    delay(10);
  }
  
  return total / readings;
}

String getDistanceStatus() {
  int distance = readDistance();
  
  if (distance < OBSTACLE_THRESHOLD) {
    return "DANGER";
  } else if (distance < SAFE_DISTANCE) {
    return "WARNING";
  } else {
    return "SAFE";
  }
}

bool isObstacleDetected() {
  return readDistance() < OBSTACLE_THRESHOLD;
}

bool isDistanceSafe() {
  return readDistance() >= SAFE_DISTANCE;
}
