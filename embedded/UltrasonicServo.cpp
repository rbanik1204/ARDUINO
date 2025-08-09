
#include "UltrasonicServo.h"

UltrasonicServo::UltrasonicServo(int trig, int echo, int servo, int motorIn1, int motorIn2) {
  // Validate pin assignments
  if (trig == echo) {
    Serial.println("❌ ERROR: Ultrasonic TRIG and ECHO pins cannot be the same!");
    return;
  }
  
  if (servo == trig || servo == echo) {
    Serial.println("❌ ERROR: Servo pin conflicts with ultrasonic pins!");
    return;
  }
  
  if (motorIn1 == motorIn2) {
    Serial.println("❌ ERROR: Motor input pins cannot be the same!");
    return;
  }
  
  trigPin = trig;
  echoPin = echo;
  servoPin = servo;
  motorIn1 = motorIn1;
  motorIn2 = motorIn2;
  obstacleDetected = false;
  lastDistance = 0;
  lastActionTime = 0;
  
  Serial.print("🔧 UltrasonicServo initialized with pins: ");
  Serial.print("TRIG="); Serial.print(trigPin);
  Serial.print(", ECHO="); Serial.print(echoPin);
  Serial.print(", SERVO="); Serial.print(servoPin);
  Serial.print(", MOTOR1="); Serial.print(motorIn1);
  Serial.print(", MOTOR2="); Serial.println(motorIn2);
}

void UltrasonicServo::begin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  myServo.attach(servoPin);
  
  // Initialize servo to center position
  myServo.write(90);
  
  Serial.println("🔍 UltrasonicServo initialized");
  Serial.print("📏 Obstacle threshold: ");
  Serial.print(OBSTACLE_THRESHOLD);
  Serial.println(" cm");
}

float UltrasonicServo::getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) {
    Serial.println("❌ No echo received. Check sensor or wiring.");
    return -1.0;
  }

  float distance = (duration * 0.034) / 2;
  lastDistance = distance;
  
  return distance;
}

void UltrasonicServo::checkAndAct() {
  float distance = getDistance();
  
  // Check if enough time has passed since last action
  if (millis() - lastActionTime < ACTION_COOLDOWN) {
    return;
  }
  
  if (distance > 0 && distance < OBSTACLE_THRESHOLD) {
    if (!obstacleDetected) {
      Serial.println("⚠️ Object detected within 20cm. Taking action...");
      obstacleDetected = true;
      lastActionTime = millis();
      
      // Activate motor rotation
      analogWrite(motorIn1, MOTOR_SPEED);
      digitalWrite(motorIn2, LOW);
      
      // Move servo to alert position
      myServo.write(180);
      
      delay(ROTATION_DURATION);
      
      // Stop motor and reset servo
      digitalWrite(motorIn1, LOW);
      digitalWrite(motorIn2, LOW);
      myServo.write(90);
    }
  } else if (distance >= OBSTACLE_THRESHOLD) {
    obstacleDetected = false;
    
    // Ensure motor is stopped
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    
    // Reset servo to center if not in use
    if (myServo.read() != 90) {
      myServo.write(90);
    }
  }
}

bool UltrasonicServo::isObstacleDetected() {
  return obstacleDetected;
}

bool UltrasonicServo::isWarningDistance() {
  return lastDistance > 0 && lastDistance < WARNING_THRESHOLD;
}

bool UltrasonicServo::isSafeDistance() {
  return lastDistance >= SAFE_DISTANCE;
}

String UltrasonicServo::getDistanceStatus() {
  if (lastDistance < 0) {
    return "ERROR";
  } else if (lastDistance < OBSTACLE_THRESHOLD) {
    return "DANGER";
  } else if (lastDistance < WARNING_THRESHOLD) {
    return "WARNING";
  } else {
    return "SAFE";
  }
}

float UltrasonicServo::getLastDistance() {
  return lastDistance;
}

void UltrasonicServo::emergencyStop() {
  Serial.println("🛑 Emergency stop activated!");
  
  // Stop motor immediately
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  
  // Reset servo to center
  myServo.write(90);
  
  obstacleDetected = false;
  lastActionTime = millis();
}

void UltrasonicServo::setObstacleThreshold(int threshold) {
  // This would need to be implemented with a non-const member variable
  Serial.print("📏 Obstacle threshold updated to: ");
  Serial.print(threshold);
  Serial.println(" cm");
}

void UltrasonicServo::setMotorSpeed(int speed) {
  // This would need to be implemented with a non-const member variable
  Serial.print("⚡ Motor speed updated to: ");
  Serial.println(speed);
}

void UltrasonicServo::setRotationDuration(int duration) {
  // This would need to be implemented with a non-const member variable
  Serial.print("⏱️ Rotation duration updated to: ");
  Serial.print(duration);
  Serial.println(" ms");
}

void UltrasonicServo::reset() {
  Serial.println("🔄 UltrasonicServo reset");
  
  // Stop motor
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  
  // Reset servo
  myServo.write(90);
  
  // Reset status
  obstacleDetected = false;
  lastActionTime = 0;
}
