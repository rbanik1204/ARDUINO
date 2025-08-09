# ToxiRover - Smart IoT Gas Sensing Rover

An intelligent IoT rover that detects harmful gases using an FC-22 sensor, avoids obstacles with ultrasonic sensors, and provides real-time control via a MERN stack web interface.

## 🛠️ Hardware Components

- **ESP8266 (NodeMCU)** - Main microcontroller
- **FC-22 Gas Sensor** - Detects LPG, methane, propane, butane
- **HC-SR04 Ultrasonic Sensor** - Distance measurement for obstacle avoidance
- **Servo Motor** - Steering or gas dispersal mechanism
- **L298N Motor Driver** - DC motor control for car movement
- **Battery Pack** - Power supply

## 📁 Project Structure

```
ToxiRover/
├── embedded/           # ESP8266 C++ code
│   ├── toxirover.ino  # Main Arduino sketch
│   ├── gas_sensor.h   # FC-22 sensor interface
│   ├── motion.h       # L298N motor control
│   ├── servo_control.h # Servo motor control
│   ├── ultrasonic.h   # HC-SR04 distance sensing
│   └── firebase.h     # Firebase Realtime Database
├── server/            # Node.js backend (optional)
│   ├── package.json
│   └── server.js
├── client/            # React frontend
│   ├── package.json
│   ├── public/
│   └── src/
└── firebase/          # Firebase configuration
    └── config.json
```

## 🚀 Features

### Core Functionality
- **Gas Detection**: Real-time monitoring of harmful gases (LPG, methane, etc.)
- **Obstacle Avoidance**: Ultrasonic sensor for distance measurement
- **Motion Control**: Full directional control via web interface
- **Real-time Data**: Live sensor readings displayed on web UI
- **Alert System**: Notifications when gas levels exceed thresholds

### Advanced Features
- **Servo Control**: Rotating mechanism for gas dispersal
- **Data Logging**: Historical data tracking and graphing
- **Mobile Responsive**: Works on all devices
- **Firebase Integration**: Cloud-based data storage and sync

## 🔧 Setup Instructions

### 1. ESP8266 Setup
1. Install Arduino IDE with ESP8266 board support
2. Install required libraries:
   - FirebaseESP8266
   - ESP8266WiFi
   - Servo
   - NewPing
3. Configure WiFi credentials in `embedded/toxirover.ino`
4. Set up Firebase project and update credentials

### 2. Firebase Setup
1. Create Firebase project
2. Enable Realtime Database
3. Set up security rules
4. Update Firebase credentials in embedded code

### 3. Web Interface Setup
```bash
cd client
npm install
npm start
```

## 📊 Data Flow

```
ESP8266 → Firebase Realtime Database → React UI
    ↓              ↓                    ↓
Gas Sensor    Motion Commands    Real-time Display
Ultrasonic    Distance Data      Control Interface
Servo         Alert System       Data Visualization
```

## 🔌 Pin Connections

### ESP8266 Pinout
- **Gas Sensor (FC-22)**: A0 (Analog input)
- **Ultrasonic (HC-SR04)**: D5 (Trig), D6 (Echo)
- **Servo Motor**: D7 (PWM)
- **L298N Motor Driver**:
  - ENA: D1
  - IN1: D2
  - IN2: D3
  - IN3: D4
  - IN4: D8
  - ENB: D0

## 🎯 Usage

1. **Power on** the ToxiRover
2. **Connect** to the web interface
3. **Monitor** real-time gas levels and distance
4. **Control** rover movement using the interface
5. **Receive alerts** when gas levels are dangerous

## 🔒 Safety Features

- Automatic gas level monitoring
- Alert system for dangerous concentrations
- Emergency stop functionality
- Distance-based obstacle avoidance

## 📈 Future Enhancements

- Machine learning for gas pattern recognition
- Autonomous navigation
- Gas dispersal mechanism
- Mobile app companion
- Multi-rover coordination

---

**Built with ❤️ for IoT and Embedded Systems**
