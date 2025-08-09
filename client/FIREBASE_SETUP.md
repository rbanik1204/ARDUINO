# Firebase Setup Guide

## 🔥 **Setting Up Firebase for ToxiRover**

### **Step 1: Create Firebase Project**
1. Go to [Firebase Console](https://console.firebase.google.com/)
2. Click "Create a project"
3. Enter project name: `toxirover-gas-sensing`
4. Enable Google Analytics (optional)
5. Click "Create project"

### **Step 2: Enable Realtime Database**
1. In Firebase Console, go to "Realtime Database"
2. Click "Create Database"
3. Choose "Start in test mode" (for development)
4. Select a location close to you
5. Click "Done"

### **Step 3: Get Configuration**
1. Go to Project Settings (gear icon)
2. Scroll down to "Your apps"
3. Click "Add app" → "Web"
4. Register app with name: `ToxiRover Dashboard`
5. Copy the config object

### **Step 4: Update Configuration**
Replace the values in `src/firebase/config.js`:

```javascript
const firebaseConfig = {
  apiKey: "your-actual-api-key",
  authDomain: "your-project.firebaseapp.com",
  databaseURL: "https://your-project.firebaseio.com",
  projectId: "your-project-id",
  storageBucket: "your-project.appspot.com",
  messagingSenderId: "your-sender-id",
  appId: "your-app-id"
};
```

### **Step 5: Database Rules**
In Firebase Console → Realtime Database → Rules:

```json
{
  "rules": {
    ".read": true,
    ".write": true
  }
}
```

**⚠️ Note: These rules allow full access. For production, set proper authentication rules.**

### **Step 6: Test Connection**
1. Start the React app: `npm start`
2. Open browser to `http://localhost:3000`
3. Check browser console for Firebase connection status

## 🎯 **Expected Database Structure**

Your Firebase Realtime Database should have these paths:
- `gas_data` - Gas sensor readings
- `ultrasonic_distance` - Distance measurements
- `motion_command` - Motor control commands
- `servo` - Servo position data
- `alerts` - System alerts
- `sensor_data` - Combined sensor data
- `status` - System status

## 🚀 **Next Steps**
1. Update ESP8266 code with your Firebase credentials
2. Upload code to ESP8266
3. Test real-time data flow
4. Configure alerts and notifications
