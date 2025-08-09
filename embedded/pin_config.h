#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// ========================================
// UNIFIED PIN CONFIGURATION FOR TOXIROVER
// ========================================

// Sensor Pins
#define GAS_SENSOR_PIN A0        // FC-22 Gas Sensor (Analog)
#define GAS_DIGITAL_PIN 0        // D3 - FC-22 D0 (digital output) - CHANGED from GPIO 5
#define ULTRASONIC_TRIG_PIN 4    // D2 - HC-SR04 Trigger
#define ULTRASONIC_ECHO_PIN 5    // D1 - HC-SR04 Echo
#define SERVO_PIN 2              // D4 - Servo Motor

// Motor Driver Pins (L298N) - Used by WiFi Control and WAN Connection
#define ENA 14                   // D5 - Left motor enable
#define IN1 12                   // D6 - Left motor input 1
#define IN2 13                   // D7 - Left motor input 2
#define IN3 15                   // D8 - Right motor input 3
#define IN4 3                    // D9 - Right motor input 4
#define ENB 16                   // D0 - Right motor enable

// UltrasonicServo specific pins (using same pins as main system)
#define ULTRASONIC_SERVO_TRIG ULTRASONIC_TRIG_PIN
#define ULTRASONIC_SERVO_ECHO ULTRASONIC_ECHO_PIN
#define ULTRASONIC_SERVO_SERVO SERVO_PIN
#define ULTRASONIC_SERVO_MOTOR1 IN1  // Use existing motor pins
#define ULTRASONIC_SERVO_MOTOR2 IN2

// WiFi Control Pins (alternative motor control)
#define WIFI_MOTOR1_PIN1 IN1     // Use same pins as main system
#define WIFI_MOTOR1_PIN2 IN2
#define WIFI_MOTOR2_PIN1 IN3
#define WIFI_MOTOR2_PIN2 IN4

// WAN Connection Pins (MQTT control)
#define M1F IN3                  // D8 (Motor 1 Forward) - Right Motor
#define M1B IN4                  // D9 (Motor 1 Backward) - Right Motor
#define M2F IN1                  // D6 (Motor 2 Forward) - Left Motor
#define M2B IN2                  // D7 (Motor 2 Backward) - Left Motor

// Additional Control Pins
#define BUZZER_PIN 16            // D0 - Buzzer pin
#define LED_PIN 5                // D1 - LED pin
#define WIFI_LED_PIN 4           // D2 - WiFi indication LED

// Pin Validation
#if (ULTRASONIC_TRIG_PIN == ULTRASONIC_ECHO_PIN)
#error "Ultrasonic TRIG and ECHO pins cannot be the same!"
#endif

#if (SERVO_PIN == ULTRASONIC_TRIG_PIN || SERVO_PIN == ULTRASONIC_ECHO_PIN)
#error "Servo pin conflicts with ultrasonic pins!"
#endif

#if (IN1 == IN2 || IN3 == IN4)
#error "Motor input pins cannot be the same!"
#endif

#if (GAS_DIGITAL_PIN == ULTRASONIC_TRIG_PIN || GAS_DIGITAL_PIN == ULTRASONIC_ECHO_PIN || GAS_DIGITAL_PIN == SERVO_PIN)
#error "Gas sensor digital pin conflicts with other pins!"
#endif

// Pin mapping for easy reference
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

#endif
