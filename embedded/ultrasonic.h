#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>
#include <NewPing.h>
#include "pin_config.h"

// Constants
#define MAX_DISTANCE 200
#define OBSTACLE_THRESHOLD 20
#define SAFE_DISTANCE 50

// Function declarations
void initUltrasonic();
int readDistance();
int getAverageDistance();
String getDistanceStatus();
bool isObstacleDetected();
bool isDistanceSafe();

#endif
