import React, { useState } from 'react';
import { set, ref } from 'firebase/database';
import { database } from '../firebase/config';
import { Settings, AlertTriangle, Shield, Activity } from 'lucide-react';
import toast from 'react-hot-toast';

const UltrasonicServoControl = ({ 
  distance, 
  status, 
  obstacleDetected, 
  onEmergencyStop 
}) => {
  const [showSettings, setShowSettings] = useState(false);
  const [threshold, setThreshold] = useState(20);
  const [motorSpeed, setMotorSpeed] = useState(255);
  const [rotationDuration, setRotationDuration] = useState(500);

  const getStatusColor = (status) => {
    switch (status?.toUpperCase()) {
      case 'DANGER':
        return 'text-red-600 bg-red-100 border-red-300';
      case 'WARNING':
        return 'text-yellow-600 bg-yellow-100 border-yellow-300';
      case 'SAFE':
        return 'text-green-600 bg-green-100 border-green-300';
      case 'ERROR':
        return 'text-gray-600 bg-gray-100 border-gray-300';
      default:
        return 'text-blue-600 bg-blue-100 border-blue-300';
    }
  };

  const getStatusIcon = (status) => {
    switch (status?.toUpperCase()) {
      case 'DANGER':
        return <AlertTriangle className="w-4 h-4" />;
      case 'WARNING':
        return <AlertTriangle className="w-4 h-4" />;
      case 'SAFE':
        return <Shield className="w-4 h-4" />;
      default:
        return <Activity className="w-4 h-4" />;
    }
  };

  const sendUltrasonicServoCommand = async (command, value) => {
    try {
      await set(ref(database, `ultrasonic_servo/${command}`), value);
      toast.success(`UltrasonicServo ${command} updated`);
    } catch (error) {
      console.error(`Error updating UltrasonicServo ${command}:`, error);
      toast.error(`Failed to update ${command}`);
    }
  };

  const handleEmergencyStop = async () => {
    try {
      await set(ref(database, 'emergency_stop'), true);
      await set(ref(database, 'motion_command/request'), 'STOP');
      toast.error('Emergency stop activated!');
      if (onEmergencyStop) onEmergencyStop();
    } catch (error) {
      console.error('Error sending emergency stop:', error);
      toast.error('Failed to send emergency stop');
    }
  };

  const resetUltrasonicServo = async () => {
    try {
      await set(ref(database, 'ultrasonic_servo/reset'), true);
      toast.success('UltrasonicServo reset');
    } catch (error) {
      console.error('Error resetting UltrasonicServo:', error);
      toast.error('Failed to reset UltrasonicServo');
    }
  };

  return (
    <div className="bg-white rounded-lg shadow-md p-6 border-l-4 border-orange-500">
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-gray-800">UltrasonicServo Control</h3>
        <button
          onClick={() => setShowSettings(!showSettings)}
          className="p-2 text-gray-500 hover:text-gray-700 transition-colors"
        >
          <Settings className="w-5 h-5" />
        </button>
      </div>

      {/* Status Display */}
      <div className="mb-6">
        <div className="flex items-center justify-between mb-3">
          <span className="text-sm font-medium text-gray-600">Status</span>
          <div className={`flex items-center px-3 py-1 rounded-full text-sm font-medium ${getStatusColor(status)}`}>
            {getStatusIcon(status)}
            <span className="ml-2">{status || 'UNKNOWN'}</span>
          </div>
        </div>
        
        <div className="text-2xl font-bold text-gray-900 mb-2">
          {distance?.toFixed(1) || 0} <span className="text-sm text-gray-500">cm</span>
        </div>

        {obstacleDetected && (
          <div className="mt-3 p-3 bg-red-50 border border-red-200 rounded-lg">
            <div className="flex items-center text-red-700">
              <AlertTriangle className="w-4 h-4 mr-2" />
              <span className="text-sm font-medium">Obstacle Detected - Auto-response Active</span>
            </div>
          </div>
        )}
      </div>

      {/* Control Buttons */}
      <div className="grid grid-cols-2 gap-3 mb-4">
        <button
          onClick={handleEmergencyStop}
          className="px-4 py-2 bg-red-600 text-white rounded-lg hover:bg-red-700 transition-colors font-medium"
        >
          Emergency Stop
        </button>
        <button
          onClick={resetUltrasonicServo}
          className="px-4 py-2 bg-gray-600 text-white rounded-lg hover:bg-gray-700 transition-colors font-medium"
        >
          Reset
        </button>
      </div>

      {/* Settings Panel */}
      {showSettings && (
        <div className="mt-4 p-4 bg-gray-50 rounded-lg">
          <h4 className="text-sm font-medium text-gray-700 mb-3">Configuration</h4>
          
          <div className="space-y-3">
            <div>
              <label className="block text-sm font-medium text-gray-600 mb-1">
                Obstacle Threshold (cm)
              </label>
              <input
                type="range"
                min="10"
                max="50"
                value={threshold}
                onChange={(e) => setThreshold(parseInt(e.target.value))}
                className="w-full"
              />
              <div className="flex justify-between text-xs text-gray-500 mt-1">
                <span>10cm</span>
                <span>{threshold}cm</span>
                <span>50cm</span>
              </div>
            </div>

            <div>
              <label className="block text-sm font-medium text-gray-600 mb-1">
                Motor Speed
              </label>
              <input
                type="range"
                min="100"
                max="255"
                value={motorSpeed}
                onChange={(e) => setMotorSpeed(parseInt(e.target.value))}
                className="w-full"
              />
              <div className="flex justify-between text-xs text-gray-500 mt-1">
                <span>100</span>
                <span>{motorSpeed}</span>
                <span>255</span>
              </div>
            </div>

            <div>
              <label className="block text-sm font-medium text-gray-600 mb-1">
                Rotation Duration (ms)
              </label>
              <input
                type="range"
                min="200"
                max="1000"
                step="100"
                value={rotationDuration}
                onChange={(e) => setRotationDuration(parseInt(e.target.value))}
                className="w-full"
              />
              <div className="flex justify-between text-xs text-gray-500 mt-1">
                <span>200ms</span>
                <span>{rotationDuration}ms</span>
                <span>1000ms</span>
              </div>
            </div>
          </div>

          <button
            onClick={() => {
              sendUltrasonicServoCommand('threshold', threshold);
              sendUltrasonicServoCommand('motor_speed', motorSpeed);
              sendUltrasonicServoCommand('rotation_duration', rotationDuration);
            }}
            className="mt-4 w-full px-4 py-2 bg-blue-600 text-white rounded-lg hover:bg-blue-700 transition-colors font-medium"
          >
            Apply Settings
          </button>
        </div>
      )}

      {/* Information */}
      <div className="mt-4 text-xs text-gray-500">
        <p>• Automatically detects obstacles within {threshold}cm</p>
        <p>• Triggers motor rotation and servo movement</p>
        <p>• Cooldown period prevents rapid responses</p>
      </div>
    </div>
  );
};

export default UltrasonicServoControl;
