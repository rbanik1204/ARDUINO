# ToxiRover UltrasonicServo Integration Guide

## Overview

This guide explains how the modular `UltrasonicServo` component integrates with the existing ToxiRover architecture, providing advanced obstacle avoidance with automatic motor and servo responses.

## Architecture Integration

### 1. File Structure

```
embedded/
├── toxirover.ino                    # Original monolithic approach
├── toxirover_integrated.ino         # Integrated version with UltrasonicServo
├── main.cpp                         # Your modular entry point
├── UltrasonicServo.h               # Enhanced header file
├── UltrasonicServo.cpp             # Enhanced implementation
├── ultrasonic.h                     # Basic ultrasonic sensor interface
├── ultrasonic.cpp                   # Basic ultrasonic implementation
├── servo_control.h                  # Servo motor interface
├── servo_control.cpp                # Servo motor implementation
├── gas_sensor.h                     # Gas sensor interface
├── gas_sensor.cpp                   # Gas sensor implementation
├── motion.h                         # Motor control interface
├── motion.cpp                       # Motor control implementation
├── firebase.h                       # Firebase interface
└── firebase.cpp                     # Firebase implementation
```

### 2. Integration Approaches

#### Option A: Integrated Approach (`toxirover_integrated.ino`)
- Combines both basic ultrasonic sensor and UltrasonicServo
- Uses existing modular functions + UltrasonicServo class
- Maintains backward compatibility
- **Recommended for production**

#### Option B: Modular Approach (`main.cpp`)
- Pure modular architecture
- Each component in separate .cpp/.h files
- More flexible but requires more setup
- **Recommended for development/testing**

## UltrasonicServo Features

### Enhanced Functionality
- **Obstacle Detection**: Automatically detects objects within 20cm
- **Motor Response**: Triggers motor rotation when obstacle detected
- **Servo Response**: Moves servo to alert position (180°) then resets
- **Cooldown System**: Prevents rapid responses with 2-second cooldown
- **Status Tracking**: Maintains obstacle detection state
- **Emergency Stop**: Immediate stop functionality

### Configuration Parameters
```cpp
const int OBSTACLE_THRESHOLD = 20;  // cm
const int WARNING_THRESHOLD = 50;   // cm
const int SAFE_DISTANCE = 100;      // cm
const int MOTOR_SPEED = 255;        // PWM value
const int ROTATION_DURATION = 500;  // ms
const unsigned long ACTION_COOLDOWN = 2000; // ms
```

## Pin Configuration

### UltrasonicServo Pins
```cpp
#define ULTRASONIC_SERVO_TRIG D5    // Trigger pin
#define ULTRASONIC_SERVO_ECHO D6    // Echo pin
#define ULTRASONIC_SERVO_SERVO D7   // Servo control pin
#define ULTRASONIC_SERVO_MOTOR1 D2  // Motor control 1
#define ULTRASONIC_SERVO_MOTOR2 D3  // Motor control 2
```

### Integration with Existing Pins
- **Ultrasonic**: Uses same pins as basic ultrasonic sensor
- **Servo**: Uses same pin as gas dispersal servo
- **Motor**: Uses existing L298N motor driver pins

## Firebase Data Structure

### New UltrasonicServo Data
```json
{
  "sensor_data": {
    "ultrasonic_servo_distance": 15.5,
    "ultrasonic_servo_status": "DANGER",
    "obstacle_detected": true
  },
  "ultrasonic_servo": {
    "distance": 15.5,
    "status": "DANGER",
    "obstacle_detected": true,
    "threshold": 20,
    "motor_speed": 255,
    "rotation_duration": 500
  }
}
```

## UI Integration

### React Components Updated
1. **SensorDisplay**: Shows UltrasonicServo data alongside basic distance
2. **Dashboard**: Displays obstacle detection status
3. **UltrasonicServoControl**: New dedicated control component

### UI Features
- **Real-time Distance**: Live distance readings from UltrasonicServo
- **Status Indicators**: DANGER/WARNING/SAFE status display
- **Obstacle Alerts**: Visual alerts when obstacles detected
- **Configuration Panel**: Adjustable threshold, motor speed, rotation duration
- **Emergency Controls**: Emergency stop and reset functionality

## Usage Instructions

### 1. Choose Your Approach

#### For Production (Recommended):
```cpp
// Use toxirover_integrated.ino
// Upload to ESP8266
// Configure Firebase credentials
// Start React development server
```

#### For Development:
```cpp
// Use main.cpp approach
// Ensure all .h and .cpp files are in embedded/ folder
// Upload to ESP8266
// Test individual components
```

### 2. Hardware Setup
1. **Connect Ultrasonic Sensor**:
   - TRIG → D5
   - ECHO → D6
   - VCC → 5V
   - GND → GND

2. **Connect Servo Motor**:
   - Signal → D7
   - VCC → 5V
   - GND → GND

3. **Connect Motor Driver**:
   - IN1 → D2
   - IN2 → D3
   - ENA → D1 (if using enable)

### 3. Software Configuration

#### Firebase Setup:
```cpp
// Update in toxirover_integrated.ino
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
#define FIREBASE_HOST "your-project.firebaseio.com"
#define FIREBASE_AUTH "your-firebase-secret"
```

#### React Configuration:
```javascript
// Update in client/src/firebase/config.js
const firebaseConfig = {
  apiKey: "your-api-key",
  authDomain: "your-project.firebaseapp.com",
  databaseURL: "https://your-project.firebaseio.com",
  projectId: "your-project-id",
  storageBucket: "your-project.appspot.com",
  messagingSenderId: "your-sender-id",
  appId: "your-app-id"
};
```

## Testing and Troubleshooting

### 1. Basic Testing
```cpp
// Test UltrasonicServo in isolation
void setup() {
  Serial.begin(115200);
  ultraServo.begin();
}

void loop() {
  ultraServo.checkAndAct();
  delay(100);
}
```

### 2. Common Issues

#### Issue: No distance readings
- **Solution**: Check wiring, ensure TRIG/ECHO pins correct
- **Debug**: Add Serial.println() in getDistance()

#### Issue: Motor not responding
- **Solution**: Verify motor driver connections
- **Debug**: Test motor pins individually

#### Issue: Servo not moving
- **Solution**: Check servo power supply
- **Debug**: Test servo with basic Servo.h example

#### Issue: Firebase not updating
- **Solution**: Verify WiFi credentials and Firebase host
- **Debug**: Check Serial monitor for connection status

### 3. Performance Optimization
- **Reduce update frequency** if experiencing lag
- **Increase cooldown period** if too many false triggers
- **Adjust threshold** based on environment

## Advanced Features

### 1. Custom Thresholds
```cpp
// In UltrasonicServo class
void setObstacleThreshold(int threshold) {
  // Implementation for dynamic threshold
}
```

### 2. Multiple UltrasonicServo Instances
```cpp
UltrasonicServo frontSensor(D5, D6, D7, D2, D3);
UltrasonicServo rearSensor(D8, D9, D10, D4, D11);
```

### 3. Custom Response Patterns
```cpp
// Modify checkAndAct() for custom behaviors
void UltrasonicServo::checkAndAct() {
  // Custom obstacle avoidance logic
}
```

## Safety Features

### 1. Emergency Stop
- **Hardware**: Emergency stop button connected to interrupt
- **Software**: Emergency stop function in UltrasonicServo
- **UI**: Emergency stop button in React interface

### 2. Obstacle Avoidance
- **Automatic**: Stops motion when obstacle detected
- **Manual**: Emergency stop button for immediate stop
- **Recovery**: Automatic reset after cooldown period

### 3. Error Handling
- **Sensor Errors**: Graceful handling of sensor failures
- **Motor Errors**: Protection against motor overload
- **Network Errors**: Offline operation capability

## Future Enhancements

### 1. Machine Learning Integration
- **Pattern Recognition**: Learn obstacle patterns
- **Predictive Avoidance**: Anticipate obstacles
- **Adaptive Thresholds**: Dynamic threshold adjustment

### 2. Advanced Sensors
- **Multiple Ultrasonic**: 360-degree coverage
- **Infrared Sensors**: Additional distance sensing
- **Camera Integration**: Visual obstacle detection

### 3. Autonomous Navigation
- **Path Planning**: Route optimization
- **Mapping**: Environment mapping
- **SLAM**: Simultaneous Localization and Mapping

## Conclusion

The UltrasonicServo integration provides a robust, modular approach to obstacle avoidance in the ToxiRover project. The dual approach (basic + advanced) ensures compatibility while adding sophisticated features for production use.

For questions or issues, refer to the main README.md or create an issue in the project repository.
