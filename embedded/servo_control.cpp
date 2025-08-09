#include "servo_control.h"

// Global servo object
extern Servo gasServo;

void initServo() {
  Serial.println("⚙️ Initializing servo motor...");
  Serial.print("🔧 Servo pin: ");
  Serial.println(SERVO_PIN);
  
  gasServo.attach(SERVO_PIN);
  gasServo.write(SERVO_CENTER_ANGLE);
  
  Serial.println("✅ Servo motor initialized successfully!");
}

void rotateServo(int angle) {
  if (angle >= SERVO_MIN_ANGLE && angle <= SERVO_MAX_ANGLE) {
    gasServo.write(angle);
    Serial.print("⚙️ Servo rotated to: ");
    Serial.print(angle);
    Serial.println("°");
  } else {
    Serial.print("❌ Invalid servo angle: ");
    Serial.println(angle);
  }
}

void rotateServoSmooth(int targetAngle) {
  int currentAngle = gasServo.read();
  int step = (targetAngle > currentAngle) ? 1 : -1;
  
  while (currentAngle != targetAngle) {
    currentAngle += step;
    gasServo.write(currentAngle);
    delay(SERVO_STEP_DELAY);
  }
}

void setServoAngle(int angle) {
  rotateServo(angle);
}

void servoGasDispersal() {
  Serial.println("🧪 Activating gas dispersal mechanism...");
  
  // Sweep servo to disperse gas
  for (int angle = 0; angle <= 180; angle += 30) {
    gasServo.write(angle);
    delay(200);
  }
  
  // Return to center
  gasServo.write(SERVO_CENTER_ANGLE);
}

bool isServoEnabled() {
  return gasServo.attached();
}

void enableServo() {
  if (!gasServo.attached()) {
    gasServo.attach(SERVO_PIN);
  }
}

void disableServo() {
  if (gasServo.attached()) {
    gasServo.detach();
  }
}

void centerServo() {
  rotateServo(SERVO_CENTER_ANGLE);
}

void sweepServo() {
  for (int angle = 0; angle <= 180; angle += 10) {
    gasServo.write(angle);
    delay(50);
  }
  
  for (int angle = 180; angle >= 0; angle -= 10) {
    gasServo.write(angle);
    delay(50);
  }
  
  centerServo();
}
