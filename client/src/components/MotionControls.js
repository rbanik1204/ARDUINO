import React from 'react';
import { 
  ArrowUp, 
  ArrowDown, 
  ArrowLeft, 
  ArrowRight, 
  Square,
  Play,
  Pause
} from 'lucide-react';

const MotionControls = ({ currentMotion, onMotionCommand, disabled }) => {
  const handleMotionCommand = (command) => {
    if (!disabled) {
      onMotionCommand(command);
    }
  };

  const getButtonClass = (command) => {
    const baseClass = "control-btn";
    const isActive = currentMotion === command;
    
    if (disabled) {
      return `${baseClass} bg-gray-600 cursor-not-allowed opacity-50`;
    }
    
    if (isActive) {
      return `${baseClass} bg-blue-600 border-2 border-blue-400`;
    }
    
    return `${baseClass} bg-gray-700 hover:bg-gray-600`;
  };

  return (
    <div className="card">
      <h3 className="card-header">Motion Controls</h3>
      
      <div className="space-y-4">
        {/* Current Status */}
        <div className="text-center">
          <div className="text-sm text-gray-400 mb-2">Current Motion</div>
          <div className="text-xl font-bold text-white">
            {currentMotion}
          </div>
        </div>

        {/* Control Grid */}
        <div className="flex justify-center">
          <div className="grid grid-cols-3 gap-4">
            {/* Top Row */}
            <div></div>
            <button
              className={getButtonClass('FORWARD')}
              onClick={() => handleMotionCommand('FORWARD')}
              disabled={disabled}
              title="Move Forward"
            >
              <ArrowUp className="w-6 h-6" />
            </button>
            <div></div>

            {/* Middle Row */}
            <button
              className={getButtonClass('LEFT')}
              onClick={() => handleMotionCommand('LEFT')}
              disabled={disabled}
              title="Turn Left"
            >
              <ArrowLeft className="w-6 h-6" />
            </button>
            
            <button
              className={getButtonClass('STOP')}
              onClick={() => handleMotionCommand('STOP')}
              disabled={disabled}
              title="Stop"
            >
              <Square className="w-6 h-6" />
            </button>
            
            <button
              className={getButtonClass('RIGHT')}
              onClick={() => handleMotionCommand('RIGHT')}
              disabled={disabled}
              title="Turn Right"
            >
              <ArrowRight className="w-6 h-6" />
            </button>

            {/* Bottom Row */}
            <div></div>
            <button
              className={getButtonClass('BACKWARD')}
              onClick={() => handleMotionCommand('BACKWARD')}
              disabled={disabled}
              title="Move Backward"
            >
              <ArrowDown className="w-6 h-6" />
            </button>
            <div></div>
          </div>
        </div>

        {/* Quick Actions */}
        <div className="flex justify-center space-x-2">
          <button
            className="btn-primary text-sm"
            onClick={() => handleMotionCommand('FORWARD')}
            disabled={disabled}
          >
            <Play className="w-4 h-4 mr-1" />
            Start
          </button>
          
          <button
            className="btn-danger text-sm"
            onClick={() => handleMotionCommand('STOP')}
            disabled={disabled}
          >
            <Pause className="w-4 h-4 mr-1" />
            Emergency Stop
          </button>
        </div>

        {/* Status Indicator */}
        <div className="text-center">
          <div className={`inline-flex items-center px-3 py-1 rounded-full text-xs font-medium ${
            currentMotion === 'STOP' 
              ? 'bg-gray-700 text-gray-300' 
              : 'bg-green-700 text-green-300'
          }`}>
            {currentMotion === 'STOP' ? '⏹️ Stopped' : '▶️ Moving'}
          </div>
        </div>

        {/* Connection Status */}
        {disabled && (
          <div className="text-center text-sm text-red-400">
            ⚠️ Disconnected from ToxiRover
          </div>
        )}
      </div>
    </div>
  );
};

export default MotionControls;
