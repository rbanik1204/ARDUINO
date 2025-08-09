#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <Arduino.h>
#include <Servo.h>
#include "pin_config.h"

// Constants
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180
#define SERVO_CENTER_ANGLE 90
#define SERVO_STEP_DELAY 15

// Function declarations
void initServo();
void rotateServo(int angle);
void rotateServoSmooth(int targetAngle);
void setServoAngle(int angle);
void servoGasDispersal();
bool isServoEnabled();
void enableServo();
void disableServo();
void centerServo();
void sweepServo();

#endif
