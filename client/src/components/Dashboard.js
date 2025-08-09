import React, { useState, useEffect } from 'react';
import { onValue, ref, set } from 'firebase/database';
import { database } from '../firebase/config';
import SensorDisplay from './SensorDisplay';
import MotionControls from './MotionControls';
import ServoControl from './ServoControl';
import UltrasonicServoControl from './UltrasonicServoControl';
import DataChart from './DataChart';
import AlertPanel from './AlertPanel';
import toast from 'react-hot-toast';

const Dashboard = () => {
  const [sensorData, setSensorData] = useState({
    gasConcentration: 0,
    distance: 0,
    motion: 'STOP',
    servoAngle: 90,
    ultrasonicServoDistance: 0,
    ultrasonicServoStatus: 'SAFE',
    obstacleDetected: false
  });
  const [connectionStatus, setConnectionStatus] = useState('connecting');
  const [alerts, setAlerts] = useState([]);

  useEffect(() => {
    // Check if Firebase is configured
    if (!database) {
      console.warn('⚠️ Firebase not configured. Running in demo mode.');
      setConnectionStatus('demo');
      toast.info('Running in demo mode. Configure Firebase for real data.');
      return;
    }

    // Listen for real-time sensor data
    const sensorDataRef = ref(database, 'sensor_data');
    const alertsRef = ref(database, 'alerts');

    const unsubscribeSensor = onValue(sensorDataRef, (snapshot) => {
      const data = snapshot.val();
      if (data) {
        setSensorData({
          gasConcentration: data.gas_concentration || 0,
          distance: data.distance || 0,
          motion: data.motion || 'STOP',
          servoAngle: data.servo_angle || 90,
          ultrasonicServoDistance: data.ultrasonic_servo_distance || 0,
          ultrasonicServoStatus: data.ultrasonic_servo_status || 'SAFE',
          obstacleDetected: data.obstacle_detected || false
        });
        setConnectionStatus('connected');
      }
    }, (error) => {
      console.error('Error reading sensor data:', error);
      setConnectionStatus('error');
      toast.error('Failed to connect to sensor data');
    });

    const unsubscribeAlerts = onValue(alertsRef, (snapshot) => {
      const alertData = snapshot.val();
      if (alertData) {
        const newAlert = {
          id: Date.now(),
          type: alertData.last_alert || 'INFO',
          message: `Gas level: ${alertData.gas_level || 0} PPM`,
          timestamp: new Date().toLocaleTimeString(),
          gasLevel: alertData.gas_level || 0
        };
        setAlerts(prev => [newAlert, ...prev.slice(0, 9)]); // Keep last 10 alerts
      }
    });

    return () => {
      unsubscribeSensor();
      unsubscribeAlerts();
    };
  }, []);

  const sendMotionCommand = async (command) => {
    if (!database) {
      toast.info('Demo mode: Motion command simulated');
      return;
    }
    try {
      await set(ref(database, 'motion_command/request'), command);
      toast.success(`Motion command sent: ${command}`);
    } catch (error) {
      console.error('Error sending motion command:', error);
      toast.error('Failed to send motion command');
    }
  };

  const sendServoCommand = async (angle) => {
    if (!database) {
      toast.info('Demo mode: Servo command simulated');
      return;
    }
    try {
      await set(ref(database, 'servo/request'), angle);
      toast.success(`Servo command sent: ${angle}°`);
    } catch (error) {
      console.error('Error sending servo command:', error);
      toast.error('Failed to send servo command');
    }
  };

  const sendEmergencyStop = async () => {
    if (!database) {
      toast.info('Demo mode: Emergency stop simulated');
      return;
    }
    try {
      await set(ref(database, 'emergency_stop'), true);
      await set(ref(database, 'motion_command/request'), 'STOP');
      toast.error('Emergency stop activated!');
    } catch (error) {
      console.error('Error sending emergency stop:', error);
      toast.error('Failed to send emergency stop');
    }
  };

  const getGasStatus = () => {
    const ppm = sensorData.gasConcentration;
    if (ppm > 500) return 'DANGER';
    if (ppm > 200) return 'WARNING';
    return 'SAFE';
  };

  const getDistanceStatus = () => {
    const distance = sensorData.distance;
    if (distance < 20) return 'DANGER';
    if (distance < 50) return 'WARNING';
    return 'SAFE';
  };

  return (
    <div className="min-h-screen bg-gray-50 p-6">
      <div className="max-w-7xl mx-auto">
        {/* Header */}
        <div className="mb-8">
          <h1 className="text-3xl font-bold text-gray-900 mb-2">
            ToxiRover Dashboard
          </h1>
          <div className="flex items-center space-x-4">
            <div className={`flex items-center space-x-2 ${
              connectionStatus === 'connected' ? 'text-green-600' : 
              connectionStatus === 'error' ? 'text-red-600' : 'text-yellow-600'
            }`}>
              <div className={`w-3 h-3 rounded-full ${
                connectionStatus === 'connected' ? 'bg-green-500' : 
                connectionStatus === 'error' ? 'bg-red-500' : 'bg-yellow-500'
              }`} />
              <span className="text-sm font-medium">
                {connectionStatus === 'connected' ? 'Connected' : 
                 connectionStatus === 'error' ? 'Connection Error' : 'Connecting...'}
              </span>
            </div>
            {sensorData.obstacleDetected && (
              <div className="flex items-center space-x-2 text-red-600">
                <span className="text-sm font-medium">⚠️ Obstacle Detected</span>
              </div>
            )}
          </div>
        </div>

        {/* Sensor Displays */}
        <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6 mb-8">
          <SensorDisplay
            title="Gas Concentration"
            value={sensorData.gasConcentration}
            unit="PPM"
            status={getGasStatus()}
            type="gas"
          />
          <SensorDisplay
            title="Distance Sensor"
            value={sensorData.distance}
            unit="cm"
            status={getDistanceStatus()}
            type="distance"
            ultrasonicServoData={{
              distance: sensorData.ultrasonicServoDistance,
              status: sensorData.ultrasonicServoStatus,
              obstacleDetected: sensorData.obstacleDetected
            }}
          />
          <div className="bg-white rounded-lg shadow-md p-6 border-l-4 border-purple-500">
            <h3 className="text-lg font-semibold text-gray-800 mb-4">Current Status</h3>
            <div className="space-y-3">
              <div className="flex justify-between">
                <span className="text-gray-600">Motion:</span>
                <span className={`font-medium ${
                  sensorData.motion === 'STOP' ? 'text-red-600' : 'text-green-600'
                }`}>
                  {sensorData.motion}
                </span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-600">Servo Angle:</span>
                <span className="font-medium text-blue-600">
                  {sensorData.servoAngle}°
                </span>
              </div>
              <div className="flex justify-between">
                <span className="text-gray-600">UltrasonicServo:</span>
                <span className={`font-medium ${
                  sensorData.ultrasonicServoStatus === 'DANGER' ? 'text-red-600' :
                  sensorData.ultrasonicServoStatus === 'WARNING' ? 'text-yellow-600' :
                  'text-green-600'
                }`}>
                  {sensorData.ultrasonicServoStatus}
                </span>
              </div>
            </div>
          </div>
        </div>

        {/* Controls */}
        <div className="grid grid-cols-1 lg:grid-cols-3 gap-6 mb-8">
          <MotionControls
            currentMotion={sensorData.motion}
            onMotionCommand={sendMotionCommand}
            onEmergencyStop={sendEmergencyStop}
          />
          <ServoControl
            currentAngle={sensorData.servoAngle}
            onServoCommand={sendServoCommand}
          />
          <UltrasonicServoControl
            distance={sensorData.ultrasonicServoDistance}
            status={sensorData.ultrasonicServoStatus}
            obstacleDetected={sensorData.obstacleDetected}
            onEmergencyStop={sendEmergencyStop}
          />
        </div>

        {/* Charts and Alerts */}
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-6">
          <DataChart sensorData={sensorData} />
          <AlertPanel alerts={alerts} />
        </div>
      </div>
    </div>
  );
};

export default Dashboard;
