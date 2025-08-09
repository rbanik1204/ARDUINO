import React from 'react';
import { Link } from 'react-router-dom';
import { Wifi, WifiOff, AlertTriangle } from 'lucide-react';

const Navigation = () => {
  const [connectionStatus, setConnectionStatus] = React.useState('online');

  return (
    <nav className="bg-gray-800 border-b border-gray-700">
      <div className="container mx-auto px-4">
        <div className="flex items-center justify-between h-16">
          {/* Logo and Brand */}
          <div className="flex items-center space-x-4">
            <Link to="/" className="flex items-center space-x-2">
              <div className="w-8 h-8 bg-blue-600 rounded-lg flex items-center justify-center">
                <span className="text-white font-bold text-sm">TR</span>
              </div>
              <span className="text-white font-bold text-xl">ToxiRover</span>
            </Link>
          </div>

          {/* Status Indicators */}
          <div className="flex items-center space-x-4">
            {/* Connection Status */}
            <div className="flex items-center space-x-2">
              {connectionStatus === 'online' ? (
                <Wifi className="w-5 h-5 text-green-400" />
              ) : (
                <WifiOff className="w-5 h-5 text-red-400" />
              )}
              <span className={`text-sm ${
                connectionStatus === 'online' ? 'text-green-400' : 'text-red-400'
              }`}>
                {connectionStatus === 'online' ? 'Connected' : 'Disconnected'}
              </span>
            </div>

            {/* Alert Indicator */}
            <div className="flex items-center space-x-2">
              <AlertTriangle className="w-5 h-5 text-yellow-400" />
              <span className="text-sm text-yellow-400">Monitoring</span>
            </div>

            {/* Version */}
            <div className="text-sm text-gray-400">
              v1.0.0
            </div>
          </div>
        </div>
      </div>
    </nav>
  );
};

export default Navigation;
