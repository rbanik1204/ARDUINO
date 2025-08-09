import React, { useState } from 'react';
import { RotateCcw, RotateCw, Settings } from 'lucide-react';

const ServoControl = ({ currentAngle, onAngleChange, disabled }) => {
  const [sliderValue, setSliderValue] = useState(currentAngle);

  const handleSliderChange = (e) => {
    const value = parseInt(e.target.value);
    setSliderValue(value);
  };

  const handleSliderRelease = () => {
    if (!disabled) {
      onAngleChange(sliderValue);
    }
  };

  const setPresetAngle = (angle) => {
    if (!disabled) {
      setSliderValue(angle);
      onAngleChange(angle);
    }
  };

  const getAngleColor = (angle) => {
    if (angle === currentAngle) {
      return 'bg-blue-600 text-white';
    }
    return 'bg-gray-700 hover:bg-gray-600 text-gray-300';
  };

  return (
    <div className="card">
      <h3 className="card-header">Servo Control</h3>
      
      <div className="space-y-4">
        {/* Current Angle Display */}
        <div className="text-center">
          <div className="text-sm text-gray-400 mb-2">Current Angle</div>
          <div className="text-3xl font-bold text-white">
            {currentAngle}°
          </div>
        </div>

        {/* Angle Slider */}
        <div className="space-y-2">
          <label className="block text-sm text-gray-400">
            Angle Control (0° - 180°)
          </label>
          <input
            type="range"
            min="0"
            max="180"
            value={sliderValue}
            onChange={handleSliderChange}
            onMouseUp={handleSliderRelease}
            onTouchEnd={handleSliderRelease}
            disabled={disabled}
            className="servo-slider"
          />
          <div className="flex justify-between text-xs text-gray-400">
            <span>0°</span>
            <span>90°</span>
            <span>180°</span>
          </div>
        </div>

        {/* Preset Positions */}
        <div className="space-y-2">
          <div className="text-sm text-gray-400">Preset Positions</div>
          <div className="grid grid-cols-3 gap-2">
            <button
              className={`px-3 py-2 rounded text-sm font-medium transition-colors ${getAngleColor(0)}`}
              onClick={() => setPresetAngle(0)}
              disabled={disabled}
            >
              <RotateCcw className="w-4 h-4 inline mr-1" />
              Left (0°)
            </button>
            
            <button
              className={`px-3 py-2 rounded text-sm font-medium transition-colors ${getAngleColor(90)}`}
              onClick={() => setPresetAngle(90)}
              disabled={disabled}
            >
              <Settings className="w-4 h-4 inline mr-1" />
              Center (90°)
            </button>
            
            <button
              className={`px-3 py-2 rounded text-sm font-medium transition-colors ${getAngleColor(180)}`}
              onClick={() => setPresetAngle(180)}
              disabled={disabled}
            >
              <RotateCw className="w-4 h-4 inline mr-1" />
              Right (180°)
            </button>
          </div>
        </div>

        {/* Fine Control */}
        <div className="space-y-2">
          <div className="text-sm text-gray-400">Fine Control</div>
          <div className="grid grid-cols-2 gap-2">
            <button
              className="btn-primary text-sm"
              onClick={() => setPresetAngle(Math.max(0, currentAngle - 10))}
              disabled={disabled}
            >
              -10°
            </button>
            
            <button
              className="btn-primary text-sm"
              onClick={() => setPresetAngle(Math.min(180, currentAngle + 10))}
              disabled={disabled}
            >
              +10°
            </button>
          </div>
        </div>

        {/* Gas Dispersal Mode */}
        <div className="space-y-2">
          <div className="text-sm text-gray-400">Special Functions</div>
          <button
            className="btn-danger w-full text-sm"
            onClick={() => {
              // Activate gas dispersal sequence
              setPresetAngle(180);
              setTimeout(() => setPresetAngle(0), 1000);
              setTimeout(() => setPresetAngle(180), 2000);
              setTimeout(() => setPresetAngle(90), 3000);
            }}
            disabled={disabled}
          >
            💨 Activate Gas Dispersal
          </button>
        </div>

        {/* Status */}
        <div className="text-center">
          <div className={`inline-flex items-center px-3 py-1 rounded-full text-xs font-medium ${
            disabled 
              ? 'bg-gray-700 text-gray-300' 
              : 'bg-green-700 text-green-300'
          }`}>
            {disabled ? '❌ Disabled' : '✅ Active'}
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

export default ServoControl;
