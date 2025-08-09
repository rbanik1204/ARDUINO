// Firebase Configuration for ToxiRover React Frontend
import { initializeApp } from 'firebase/app';
import { getDatabase, ref } from 'firebase/database';

// Your Firebase configuration
// Replace with your actual Firebase project config
// Get these values from your Firebase Console: https://console.firebase.google.com/
const firebaseConfig = {
  apiKey: process.env.REACT_APP_FIREBASE_API_KEY || "your-api-key",
  authDomain: process.env.REACT_APP_FIREBASE_AUTH_DOMAIN || "your-project.firebaseapp.com",
  databaseURL: process.env.REACT_APP_FIREBASE_DATABASE_URL || "https://your-project.firebaseio.com",
  projectId: process.env.REACT_APP_FIREBASE_PROJECT_ID || "your-project-id",
  storageBucket: process.env.REACT_APP_FIREBASE_STORAGE_BUCKET || "your-project.appspot.com",
  messagingSenderId: process.env.REACT_APP_FIREBASE_MESSAGING_SENDER_ID || "your-sender-id",
  appId: process.env.REACT_APP_FIREBASE_APP_ID || "your-app-id"
};

// Initialize Firebase
let app;
let database;

try {
  app = initializeApp(firebaseConfig);
  database = getDatabase(app);
  console.log('✅ Firebase initialized successfully');
} catch (error) {
  console.warn('⚠️ Firebase initialization failed:', error.message);
  console.log('📝 Please update your Firebase configuration in src/firebase/config.js');
  console.log('📖 See client/FIREBASE_SETUP.md for setup instructions');
  
  // Create a mock database for development
  app = null;
  database = null;
}

// Database references (with null checks)
export const gasDataRef = database ? ref(database, 'gas_data') : null;
export const distanceDataRef = database ? ref(database, 'ultrasonic_distance') : null;
export const motionCommandRef = database ? ref(database, 'motion_command') : null;
export const servoDataRef = database ? ref(database, 'servo') : null;
export const alertsRef = database ? ref(database, 'alerts') : null;
export const sensorDataRef = database ? ref(database, 'sensor_data') : null;
export const statusRef = database ? ref(database, 'status') : null;

// Export database for components that need direct access
export { database };

export default app;
