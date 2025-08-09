# ToxiRover Pin Mapping Guide

## 📍 **Complete Pin Assignment**

| Function | GPIO | NodeMCU Label | Arduino Pin | Purpose | Wire Color (Suggestion) |
|----------|------|---------------|-------------|---------|------------------------|
| **Gas Sensor** | A0 | A0 | A0 | FC-22 Gas Sensor | Red/Black |
| **Ultrasonic TRIG** | 4 | D2 | 4 | HC-SR04 Trigger | Blue |
| **Ultrasonic ECHO** | 5 | D1 | 5 | HC-SR04 Echo | Green |
| **Servo Motor** | 2 | D4 | 2 | Servo Control | Yellow |
| **Motor ENA** | 14 | D5 | 14 | Left Motor Enable | Orange |
| **Motor IN1** | 12 | D6 | 12 | Left Motor Input 1 | Brown |
| **Motor IN2** | 13 | D7 | 13 | Left Motor Input 2 | Purple |
| **Motor IN3** | 15 | D8 | 15 | Right Motor Input 3 | Gray |
| **Motor IN4** | 3 | D9 | 3 | Right Motor Input 4 | White |
| **Motor ENB** | 16 | D0 | 16 | Right Motor Enable | Pink |

## 🔧 **Hardware Connections**

### **1. Gas Sensor (FC-22)**
```
FC-22 Gas Sensor → NodeMCU
VCC → 3.3V
GND → GND
AOUT → A0 (Gas Sensor Reading)
```

### **2. Ultrasonic Sensor (HC-SR04)**
```
HC-SR04 → NodeMCU
VCC → 5V
GND → GND
TRIG → D2 (GPIO 4)
ECHO → D1 (GPIO 5)
```

### **3. Servo Motor**
```
Servo Motor → NodeMCU
VCC → 5V (External power recommended)
GND → GND
Signal → D4 (GPIO 2)
```

### **4. L298N Motor Driver**
```
L298N → NodeMCU
ENA → D5 (GPIO 14) - Left Motor Enable
IN1 → D6 (GPIO 12) - Left Motor Input 1
IN2 → D7 (GPIO 13) - Left Motor Input 2
IN3 → D8 (GPIO 15) - Right Motor Input 3
IN4 → D9 (GPIO 3) - Right Motor Input 4
ENB → D0 (GPIO 16) - Right Motor Enable

L298N → Motors
OUT1 → Left Motor Terminal 1
OUT2 → Left Motor Terminal 2
OUT3 → Right Motor Terminal 1
OUT4 → Right Motor Terminal 2

L298N → Power
VCC → 12V (External power)
GND → GND
```

## ⚠️ **Important Notes**

### **Power Requirements**
- **NodeMCU**: 5V via USB or external power
- **Servo Motor**: 5V external power (recommended)
- **L298N**: 12V external power for motors
- **Sensors**: 3.3V/5V from NodeMCU

### **Pin Conflicts Resolved**
- ✅ **No pin conflicts** - Each function has unique GPIO
- ✅ **Proper voltage levels** - 3.3V for NodeMCU, 5V for sensors
- ✅ **Motor isolation** - L298N provides proper motor control

### **Wiring Tips**
1. **Use different colored wires** for easy identification
2. **Keep motor wires separate** from sensor wires
3. **Use external power** for motors and servo
4. **Add capacitors** (100µF) near motor driver for stability
5. **Use breadboard** for testing before final assembly

## 🔍 **Testing Each Component**

### **1. Test Gas Sensor**
```cpp
void setup() {
  Serial.begin(9600);
}

void loop() {
  int gasValue = analogRead(A0);
  Serial.print("Gas Sensor: ");
  Serial.println(gasValue);
  delay(1000);
}
```

### **2. Test Ultrasonic Sensor**
```cpp
void setup() {
  Serial.begin(9600);
  pinMode(4, OUTPUT);  // TRIG
  pinMode(5, INPUT);   // ECHO
}

void loop() {
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(10);
  digitalWrite(4, LOW);
  
  long duration = pulseIn(5, HIGH);
  int distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(1000);
}
```

### **3. Test Servo**
```cpp
#include <Servo.h>
Servo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(2);  // D4
}

void loop() {
  myServo.write(0);
  delay(1000);
  myServo.write(90);
  delay(1000);
  myServo.write(180);
  delay(1000);
}
```

### **4. Test Motors**
```cpp
void setup() {
  Serial.begin(9600);
  pinMode(14, OUTPUT);  // ENA
  pinMode(12, OUTPUT);  // IN1
  pinMode(13, OUTPUT);  // IN2
  pinMode(15, OUTPUT);  // IN3
  pinMode(3, OUTPUT);   // IN4
  pinMode(16, OUTPUT);  // ENB
}

void loop() {
  // Test forward
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(15, HIGH);
  digitalWrite(3, LOW);
  analogWrite(14, 255);
  analogWrite(16, 255);
  delay(2000);
  
  // Stop
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  digitalWrite(15, LOW);
  digitalWrite(3, LOW);
  analogWrite(14, 0);
  analogWrite(16, 0);
  delay(2000);
}
```

## 🚨 **Troubleshooting**

### **Common Issues**
1. **Motors not moving**: Check power supply and enable pins
2. **Servo jittering**: Use external 5V power supply
3. **Ultrasonic not working**: Check TRIG/ECHO connections
4. **Gas sensor readings**: Ensure proper voltage (3.3V)

### **Debug Commands**
```cpp
// Add to your setup() function
Serial.println("🔧 Pin Configuration:");
Serial.print("Gas Sensor: A0");
Serial.print(" Ultrasonic TRIG: "); Serial.print(4);
Serial.print(" ECHO: "); Serial.println(5);
Serial.print("Servo: "); Serial.print(2);
Serial.print(" Motor ENA: "); Serial.print(14);
Serial.print(" IN1: "); Serial.print(12);
Serial.print(" IN2: "); Serial.print(13);
Serial.print(" IN3: "); Serial.print(15);
Serial.print(" IN4: "); Serial.print(3);
Serial.print(" ENB: "); Serial.println(16);
```

## 📋 **Final Checklist**

- [ ] All pins correctly assigned in `pin_config.h`
- [ ] No pin conflicts between components
- [ ] Proper power supply for each component
- [ ] All wires securely connected
- [ ] Motors tested individually
- [ ] Sensors calibrated and tested
- [ ] Servo movement smooth
- [ ] Ultrasonic distance readings accurate
- [ ] Gas sensor responding to environment
- [ ] All components working together

## 🎯 **Next Steps**

1. **Upload the corrected code** to your ESP8266
2. **Test each component individually** using the test code above
3. **Verify pin assignments** match your wiring
4. **Run the full system** and monitor Serial output
5. **Calibrate sensors** if needed
6. **Test the React UI** for real-time control

Your pin configuration is now **conflict-free** and **properly organized**! 🎉
