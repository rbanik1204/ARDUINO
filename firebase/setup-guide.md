# Firebase Setup Guide for ToxiRover

This guide will help you set up Firebase for the ToxiRover project.

## 1. Create Firebase Project

1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Click "Create a project"
3. Enter project name: `toxirover`
4. Enable Google Analytics (optional)
5. Click "Create project"

## 2. Enable Realtime Database

1. In Firebase Console, go to "Realtime Database"
2. Click "Create Database"
3. Choose a location (closest to your region)
4. Start in test mode (we'll update security rules later)

## 3. Get Firebase Configuration

### For ESP8266 (Embedded Code)

1. Go to Project Settings (gear icon)
2. Scroll down to "Your apps"
3. Click "Add app" → "Web app"
4. Register app with name "ToxiRover-ESP8266"
5. Copy the config object

Update `embedded/toxirover.ino`:
```cpp
#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH "your-database-secret"
```

### For React App

1. In Project Settings, copy the Firebase config
2. Update `client/src/firebase/config.js`:
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

## 4. Database Structure

The Firebase Realtime Database will have this structure:

```
toxirover/
├── gas_data/
│   ├── ppm: 0
│   └── timestamp: "1234567890"
├── ultrasonic_distance/
│   └── value: 0
├── motion_command/
│   ├── current: "STOP"
│   └── request: "FORWARD"
├── servo/
│   ├── angle: 90
│   └── request: 90
├── alerts/
│   ├── last_alert: "HIGH_GAS_LEVEL"
│   ├── last_message: "Gas level exceeded 1000 PPM"
│   └── last_timestamp: "1234567890"
├── sensor_data/
│   ├── gas_concentration: 0
│   ├── distance: 0
│   ├── motion: "STOP"
│   ├── servo_angle: 90
│   └── timestamp: 1234567890
└── status/
    └── value: "ONLINE"
```

## 5. Security Rules

Update Firebase Realtime Database rules:

```json
{
  "rules": {
    ".read": true,
    ".write": true,
    "gas_data": {
      ".read": true,
      ".write": true
    },
    "ultrasonic_distance": {
      ".read": true,
      ".write": true
    },
    "motion_command": {
      ".read": true,
      ".write": true
    },
    "servo": {
      ".read": true,
      ".write": true
    },
    "alerts": {
      ".read": true,
      ".write": true
    },
    "sensor_data": {
      ".read": true,
      ".write": true
    },
    "status": {
      ".read": true,
      ".write": true
    }
  }
}
```

## 6. Testing the Connection

### Test ESP8266 Connection

1. Upload the code to ESP8266
2. Open Serial Monitor
3. Check for Firebase connection messages
4. Verify data is being sent to Firebase

### Test React App Connection

1. Start the React app: `npm start`
2. Open browser to `http://localhost:3000`
3. Check browser console for Firebase connection
4. Verify real-time data updates

## 7. Troubleshooting

### Common Issues

1. **ESP8266 can't connect to Firebase**
   - Check WiFi credentials
   - Verify Firebase host and auth
   - Check internet connection

2. **React app shows "Connection Error"**
   - Verify Firebase config in `config.js`
   - Check Firebase project settings
   - Ensure database rules allow read/write

3. **Data not updating in real-time**
   - Check Firebase database rules
   - Verify data paths match between ESP8266 and React
   - Check browser console for errors

### Debug Commands

ESP8266 Serial Monitor:
```
🚀 ToxiRover Starting...
📡 Connecting to WiFi: YOUR_WIFI_SSID
✅ WiFi connected!
🔥 Initializing Firebase connection...
✅ Firebase connected successfully!
```

React Console:
```
Firebase: Connected to database
Firebase: Data received: {gas_concentration: 0, distance: 0, ...}
```

## 8. Production Deployment

### For React App

1. Build the app: `npm run build`
2. Deploy to Firebase Hosting:
   ```bash
   npm install -g firebase-tools
   firebase login
   firebase init hosting
   firebase deploy
   ```

### For ESP8266

1. Update WiFi credentials for production network
2. Test all sensors and actuators
3. Verify Firebase connection stability
4. Monitor battery life and power consumption

## 9. Monitoring and Analytics

### Firebase Analytics (Optional)

1. Enable Firebase Analytics in console
2. Track user interactions with the web interface
3. Monitor sensor data patterns
4. Set up alerts for unusual activity

### Database Monitoring

1. Use Firebase Console to monitor data
2. Set up email alerts for high gas levels
3. Monitor database usage and costs
4. Backup important data regularly

## 10. Security Best Practices

1. **Never commit Firebase secrets to Git**
2. Use environment variables for sensitive data
3. Implement proper authentication for production
4. Regularly update Firebase SDK versions
5. Monitor database access patterns

## Support

If you encounter issues:
1. Check Firebase Console for error logs
2. Verify network connectivity
3. Test with Firebase's sample apps
4. Consult Firebase documentation
5. Check ESP8266 and React app logs
