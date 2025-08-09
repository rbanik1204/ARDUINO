import React from 'react';
import { AlertTriangle, Shield, Activity } from 'lucide-react';

const SensorDisplay = ({ title, value, unit, status, type, ultrasonicServoData }) => {
  const getStatusColor = (status) => {
    switch (status?.toUpperCase()) {
      case 'DANGER':
        return 'text-red-500 bg-red-100 border-red-300';
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

  const getGasLevelBar = (value) => {
    const percentage = Math.min((value / 1000) * 100, 100); // Assuming 1000ppm is max
    return (
      <div className="w-full bg-gray-200 rounded-full h-2 mt-2">
        <div 
          className={`h-2 rounded-full transition-all duration-300 ${
            percentage > 80 ? 'bg-red-500' : 
            percentage > 60 ? 'bg-yellow-500' : 'bg-green-500'
          }`}
          style={{ width: `${percentage}%` }}
        />
      </div>
    );
  };

  const getDistanceCircle = (value) => {
    const maxDistance = 200; // cm
    const percentage = Math.min((value / maxDistance) * 100, 100);
    const size = 60 + (percentage * 0.4); // 60-100px based on distance
    
    return (
      <div className="flex items-center justify-center mt-2">
        <div 
          className={`rounded-full border-4 transition-all duration-300 ${
            value < 20 ? 'border-red-500 bg-red-100' :
            value < 50 ? 'border-yellow-500 bg-yellow-100' :
            'border-green-500 bg-green-100'
          }`}
          style={{ width: `${size}px`, height: `${size}px` }}
        />
      </div>
    );
  };

  return (
    <div className="bg-white rounded-lg shadow-md p-6 border-l-4 border-blue-500">
      <div className="flex items-center justify-between mb-4">
        <h3 className="text-lg font-semibold text-gray-800">{title}</h3>
        <div className={`flex items-center px-2 py-1 rounded-full text-xs font-medium ${getStatusColor(status)}`}>
          {getStatusIcon(status)}
          <span className="ml-1">{status || 'NORMAL'}</span>
        </div>
      </div>
      
      <div className="text-3xl font-bold text-gray-900 mb-2">
        {value || 0} <span className="text-sm text-gray-500">{unit}</span>
      </div>
      
      {type === 'gas' && getGasLevelBar(value)}
      {type === 'distance' && getDistanceCircle(value)}
      
      {/* UltrasonicServo specific display */}
      {ultrasonicServoData && (
        <div className="mt-4 p-3 bg-gray-50 rounded-lg">
          <div className="flex items-center justify-between mb-2">
            <span className="text-sm font-medium text-gray-700">UltrasonicServo</span>
            <div className={`px-2 py-1 rounded-full text-xs font-medium ${getStatusColor(ultrasonicServoData.status)}`}>
              {ultrasonicServoData.status}
            </div>
          </div>
          <div className="text-lg font-semibold text-gray-800">
            {ultrasonicServoData.distance?.toFixed(1) || 0} cm
          </div>
          {ultrasonicServoData.obstacleDetected && (
            <div className="mt-2 text-xs text-red-600 font-medium">
              ⚠️ Obstacle detected - Auto-response active
            </div>
          )}
        </div>
      )}
    </div>
  );
};

export default SensorDisplay;
