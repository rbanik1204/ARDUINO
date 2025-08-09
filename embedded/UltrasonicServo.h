
#ifndef ULTRASONIC_SERVO_H
#define ULTRASONIC_SERVO_H

#include <Arduino.h>
#include <Servo.h>
#include "pin_config.h"

class UltrasonicServo {
  private:
    int trigPin;
    int echoPin;
    int servoPin;
    int motorIn1, motorIn2;
    Servo myServo;
    
    // Obstacle detection parameters
    const int OBSTACLE_THRESHOLD = 20;  // cm
    const int WARNING_THRESHOLD = 50;   // cm
    const int SAFE_DISTANCE = 100;      // cm
    
    // Motor control parameters
    const int MOTOR_SPEED = 255;
    const int ROTATION_DURATION = 500;  // ms
    
    // Status tracking
    bool obstacleDetected;
    float lastDistance;
    unsigned long lastActionTime;
    const unsigned long ACTION_COOLDOWN = 2000; // ms

  public:
    UltrasonicServo(int trig, int echo, int servo, int motorIn1, int motorIn2);
    void begin();
    float getDistance();
    void checkAndAct();
    bool isObstacleDetected();
    bool isWarningDistance();
    bool isSafeDistance();
    String getDistanceStatus();
    float getLastDistance();
    void emergencyStop();
    void setObstacleThreshold(int threshold);
    void setMotorSpeed(int speed);
    void setRotationDuration(int duration);
    void reset();
};

#endif
