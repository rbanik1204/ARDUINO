# ToxiRover Setup Guide

Complete setup instructions for the ToxiRover IoT gas sensing rover project.

## 🚀 Quick Start

1. **Hardware Assembly** → **ESP8266 Setup** → **Firebase Configuration** → **Web Interface**

## 📋 Prerequisites

### Hardware Requirements
- ESP8266 (NodeMCU)
- FC-22 Gas Sensor
- HC-SR04 Ultrasonic Sensor
- Servo Motor (SG90 or similar)
- L298N Motor Driver
- DC Motors (2x)
- Battery Pack (7.4V or 12V)
- Jumper Wires
- Breadboard
- Chassis/Rover Frame

### Software Requirements
- Arduino IDE 1.8.x or 2.x
- Node.js 16+ and npm
- Git

## 🔧 Hardware Assembly

### 1. Pin Connections

| Component | ESP8266 Pin | Description |
|-----------|-------------|-------------|
| Gas Sensor (FC-22) | A0 | Analog input |
| Ultrasonic Trig | D5 | Digital output |
| Ultrasonic Echo | D6 | Digital input |
| Servo Motor | D7 | PWM output |
| L298N ENA | D1 | Left motor enable |
| L298N IN1 | D2 | Left motor direction 1 |
| L298N IN2 | D3 | Left motor direction 2 |
| L298N IN3 | D4 | Right motor direction 3 |
| L298N IN4 | D8 | Right motor direction 4 |
| L298N ENB | D0 | Right motor enable |

### 2. Power Connections

```
Battery Pack (7.4V/12V)
├── L298N VCC
├── Servo VCC
└── ESP8266 VIN

Ground Connections:
├── L298N GND
├── Servo GND
├── Gas Sensor GND
├── Ultrasonic GND
└── ESP8266 GND
```

### 3. Motor Connections

```
Left Motor:
├── Motor + → L298N OUT1
└── Motor - → L298N OUT2

Right Motor:
├── Motor + → L298N OUT3
└── Motor - → L298N OUT4
```

## 💻 Software Setup

### 1. ESP8266 Setup

#### Install Arduino IDE
1. Download [Arduino IDE](https://www.arduino.cc/en/software)
2. Install and open Arduino IDE

#### Add ESP8266 Board Support
1. Go to **File** → **Preferences**
2. Add to Additional Board Manager URLs:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Go to **Tools** → **Board** → **Boards Manager**
4. Search for "ESP8266" and install

#### Install Required Libraries
1. Go to **Sketch** → **Include Library** → **Manage Libraries**
2. Install these libraries:
   - `FirebaseESP8266` by Mobizt
   - `ESP8266WiFi`
   - `Servo`
   - `NewPing`

#### Configure the Code
1. Open `embedded/toxirover.ino`
2. Update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
3. Update Firebase configuration:
   ```cpp
   #define FIREBASE_HOST "your-project.firebaseio.com"
   #define FIREBASE_AUTH "your-firebase-secret"
   ```

#### Upload to ESP8266
1. Select board: **NodeMCU 1.0 (ESP-12E Module)**
2. Select port (COM port on Windows, /dev/ttyUSB0 on Linux)
3. Click **Upload**
4. Open Serial Monitor (115200 baud) to see debug messages

### 2. Firebase Setup

#### Create Firebase Project
1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Click **Create a project**
3. Name it `toxirover`
4. Enable Google Analytics (optional)
5. Click **Create project**

#### Enable Realtime Database
1. In Firebase Console, go to **Realtime Database**
2. Click **Create Database**
3. Choose location (closest to you)
4. Start in **test mode**

#### Get Firebase Configuration
1. Go to **Project Settings** (gear icon)
2. Scroll to **Your apps**
3. Click **Add app** → **Web app**
4. Register app as "ToxiRover-ESP8266"
5. Copy the configuration

#### Update Security Rules
In Firebase Console → Realtime Database → Rules:
```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

### 3. React Web Interface Setup

#### Install Dependencies
```bash
cd client
npm install
```

#### Configure Firebase
1. Update `client/src/firebase/config.js`:
   ```javascript
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

#### Start Development Server
```bash
npm start
```
Open http://localhost:3000

## 🧪 Testing

### 1. Hardware Testing

#### Test Gas Sensor
1. Power on the ESP8266
2. Check Serial Monitor for gas readings
3. Test with different gas sources (lighter, alcohol)

#### Test Ultrasonic Sensor
1. Place objects at different distances
2. Verify distance readings in Serial Monitor
3. Test obstacle detection

#### Test Motors
1. Use motion controls in web interface
2. Verify all directions work correctly
3. Test emergency stop

#### Test Servo
1. Use servo controls in web interface
2. Verify smooth movement
3. Test preset positions

### 2. Software Testing

#### Test Firebase Connection
1. Check ESP8266 Serial Monitor for connection messages
2. Verify data appears in Firebase Console
3. Test real-time updates in web interface

#### Test Web Interface
1. Open http://localhost:3000
2. Check all sensor displays update
3. Test motion controls
4. Test servo controls
5. Verify alerts work

## 🔧 Troubleshooting

### Common Issues

#### ESP8266 Won't Connect to WiFi
- Check WiFi credentials
- Verify network is 2.4GHz (ESP8266 doesn't support 5GHz)
- Check signal strength

#### Firebase Connection Failed
- Verify Firebase host and auth credentials
- Check internet connection
- Ensure Firebase project is active

#### Web Interface Shows "Connection Error"
- Check Firebase configuration in `config.js`
- Verify database rules allow read/write
- Check browser console for errors

#### Motors Not Responding
- Check L298N connections
- Verify motor polarity
- Check power supply voltage

#### Gas Sensor Readings Unstable
- Calibrate sensor in clean air
- Check sensor connections
- Verify power supply stability

### Debug Commands

#### ESP8266 Serial Monitor
```
🚀 ToxiRover Starting...
📡 Connecting to WiFi: YOUR_WIFI_SSID
✅ WiFi connected!
🔥 Initializing Firebase connection...
✅ Firebase connected successfully!
🔬 Initializing gas sensor...
✅ Gas sensor initialized!
🚗 Initializing motion control...
✅ Motion control initialized!
⚙️ Initializing servo motor...
✅ Servo motor initialized!
📏 Initializing ultrasonic sensor...
✅ Ultrasonic sensor initialized!
✅ ToxiRover initialized successfully!
```

#### React Console
```
Firebase: Connected to database
Firebase: Data received: {gas_concentration: 0, distance: 0, ...}
```

## 🚀 Production Deployment

### 1. ESP8266 Production Setup

#### Update WiFi for Production
```cpp
const char* ssid = "PRODUCTION_WIFI_SSID";
const char* password = "PRODUCTION_WIFI_PASSWORD";
```

#### Test All Components
1. Run full system test
2. Verify battery life
3. Test in target environment
4. Monitor for 24 hours

### 2. Web Interface Deployment

#### Build for Production
```bash
cd client
npm run build
```

#### Deploy to Firebase Hosting
```bash
npm install -g firebase-tools
firebase login
firebase init hosting
firebase deploy
```

## 📊 Monitoring

### 1. Firebase Console
- Monitor real-time data
- Check connection status
- View alert history

### 2. Serial Monitor
- Monitor ESP8266 status
- Debug sensor readings
- Check error messages

### 3. Web Interface
- Real-time sensor displays
- Control interface
- Alert notifications

## 🔒 Security Considerations

### 1. Firebase Security
- Update database rules for production
- Implement authentication if needed
- Monitor access patterns

### 2. Network Security
- Use secure WiFi network
- Consider VPN for remote access
- Monitor network traffic

### 3. Physical Security
- Secure hardware components
- Protect against tampering
- Regular maintenance

## 📈 Future Enhancements

### 1. Hardware Upgrades
- Add more sensors (temperature, humidity)
- Implement GPS tracking
- Add camera module
- Upgrade to ESP32 for more features

### 2. Software Features
- Machine learning for gas pattern recognition
- Autonomous navigation
- Mobile app companion
- Multi-rover coordination

### 3. Safety Features
- Automatic emergency shutdown
- Backup power systems
- Redundant sensors
- Fail-safe mechanisms

## 📞 Support

### Documentation
- [ESP8266 Documentation](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/)
- [Firebase Documentation](https://firebase.google.com/docs)
- [React Documentation](https://reactjs.org/docs/)

### Community
- Arduino Forum
- ESP8266 Community
- Firebase Community
- React Community

### Issues
1. Check this setup guide
2. Review troubleshooting section
3. Check component datasheets
4. Search online forums
5. Contact support if needed

---

**Happy Building! 🚀**

The ToxiRover project combines embedded systems, IoT, and web development to create a powerful gas sensing platform. Follow this guide carefully and you'll have a fully functional system!
