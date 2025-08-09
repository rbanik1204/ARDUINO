import React from 'react';
import { AlertTriangle, AlertCircle, Info, X } from 'lucide-react';

const AlertPanel = ({ alerts }) => {
  const getAlertIcon = (type) => {
    switch (type) {
      case 'HIGH_GAS_LEVEL':
        return <AlertTriangle className="w-5 h-5 text-red-400" />;
      case 'OBSTACLE_DETECTED':
        return <AlertCircle className="w-5 h-5 text-yellow-400" />;
      default:
        return <Info className="w-5 h-5 text-blue-400" />;
    }
  };

  const getAlertColor = (type) => {
    switch (type) {
      case 'HIGH_GAS_LEVEL':
        return 'alert-danger';
      case 'OBSTACLE_DETECTED':
        return 'alert-warning';
      default:
        return 'alert-success';
    }
  };

  const getAlertTitle = (type) => {
    switch (type) {
      case 'HIGH_GAS_LEVEL':
        return '🚨 High Gas Level Detected';
      case 'OBSTACLE_DETECTED':
        return '⚠️ Obstacle Detected';
      case 'SYSTEM_STARTUP':
        return '✅ System Started';
      case 'CONNECTION_LOST':
        return '🔌 Connection Lost';
      default:
        return 'ℹ️ System Alert';
    }
  };

  const formatTimestamp = (timestamp) => {
    if (!timestamp) return 'Unknown time';
    
    const date = new Date(parseInt(timestamp));
    return date.toLocaleTimeString();
  };

  if (alerts.length === 0) {
    return (
      <div className="card">
        <h3 className="card-header">System Alerts</h3>
        <div className="text-center py-8">
          <Info className="w-12 h-12 text-gray-400 mx-auto mb-4" />
          <p className="text-gray-400">No active alerts</p>
          <p className="text-sm text-gray-500 mt-2">System is operating normally</p>
        </div>
      </div>
    );
  }

  return (
    <div className="card">
      <div className="flex items-center justify-between mb-4">
        <h3 className="card-header">System Alerts</h3>
        <div className="flex items-center space-x-2">
          <div className="w-2 h-2 bg-red-400 rounded-full animate-pulse"></div>
          <span className="text-sm text-gray-400">{alerts.length} active alerts</span>
        </div>
      </div>

      <div className="space-y-3">
        {alerts.slice(-5).reverse().map((alert, index) => (
          <div key={index} className={`alert ${getAlertColor(alert.type)}`}>
            <div className="flex items-start space-x-3">
              {getAlertIcon(alert.type)}
              <div className="flex-1">
                <div className="flex items-center justify-between">
                  <h4 className="font-medium text-sm">
                    {getAlertTitle(alert.type)}
                  </h4>
                  <span className="text-xs opacity-75">
                    {formatTimestamp(alert.timestamp)}
                  </span>
                </div>
                {alert.message && (
                  <p className="text-sm mt-1 opacity-90">
                    {alert.message}
                  </p>
                )}
              </div>
            </div>
          </div>
        ))}
      </div>

      {/* Alert Summary */}
      <div className="mt-4 pt-4 border-t border-gray-700">
        <div className="grid grid-cols-3 gap-4 text-center text-sm">
          <div>
            <div className="text-red-400 font-medium">
              {alerts.filter(a => a.type === 'HIGH_GAS_LEVEL').length}
            </div>
            <div className="text-gray-400">Danger</div>
          </div>
          <div>
            <div className="text-yellow-400 font-medium">
              {alerts.filter(a => a.type === 'OBSTACLE_DETECTED').length}
            </div>
            <div className="text-gray-400">Warning</div>
          </div>
          <div>
            <div className="text-blue-400 font-medium">
              {alerts.filter(a => !['HIGH_GAS_LEVEL', 'OBSTACLE_DETECTED'].includes(a.type)).length}
            </div>
            <div className="text-gray-400">Info</div>
          </div>
        </div>
      </div>

      {/* Alert Actions */}
      <div className="mt-4 flex justify-between items-center">
        <button className="text-sm text-blue-400 hover:text-blue-300 transition-colors">
          View All Alerts
        </button>
        <button className="text-sm text-gray-400 hover:text-gray-300 transition-colors">
          Clear All
        </button>
      </div>
    </div>
  );
};

export default AlertPanel;
