import React, { useState, useEffect } from 'react';
import { LineChart, Line, XAxis, YAxis, CartesianGrid, Tooltip, ResponsiveContainer, AreaChart, Area } from 'recharts';

const DataChart = ({ gasData, distanceData }) => {
  const [chartData, setChartData] = useState([]);
  const [timeRange, setTimeRange] = useState('1m'); // 1m, 5m, 15m

  useEffect(() => {
    // Add new data point
    const newDataPoint = {
      time: new Date().toLocaleTimeString(),
      gas: gasData,
      distance: distanceData,
      timestamp: Date.now()
    };

    setChartData(prevData => {
      const updatedData = [...prevData, newDataPoint];
      
      // Keep only data within the selected time range
      const timeRangeMs = {
        '1m': 60000,
        '5m': 300000,
        '15m': 900000
      }[timeRange];
      
      const cutoffTime = Date.now() - timeRangeMs;
      return updatedData.filter(point => point.timestamp > cutoffTime);
    });
  }, [gasData, distanceData, timeRange]);

  const getGasColor = (value) => {
    if (!value) return '#6b7280'; // Gray for no data
    if (value > 1000) return '#ef4444'; // red
    if (value > 500) return '#f59e0b'; // yellow
    return '#10b981'; // green
  };

  const getDistanceColor = (value) => {
    if (!value) return '#6b7280'; // Gray for no data
    if (value < 20) return '#ef4444'; // red
    if (value < 50) return '#f59e0b'; // yellow
    return '#10b981'; // green
  };

  const CustomTooltip = ({ active, payload, label }) => {
    if (active && payload && payload.length) {
      return (
        <div className="bg-gray-800 border border-gray-700 p-3 rounded-lg shadow-lg">
          <p className="text-white font-medium">{`Time: ${label}`}</p>
          {payload.map((entry, index) => (
            <p key={index} style={{ color: entry.color }}>
              {`${entry.name}: ${entry.value}`}
            </p>
          ))}
        </div>
      );
    }
    return null;
  };

  return (
    <div className="card">
      <div className="flex items-center justify-between mb-4">
        <h3 className="card-header">Real-time Data</h3>
        <div className="flex space-x-2">
          {['1m', '5m', '15m'].map(range => (
            <button
              key={range}
              onClick={() => setTimeRange(range)}
              className={`px-3 py-1 rounded text-xs font-medium transition-colors ${
                timeRange === range
                  ? 'bg-blue-600 text-white'
                  : 'bg-gray-700 text-gray-300 hover:bg-gray-600'
              }`}
            >
              {range}
            </button>
          ))}
        </div>
      </div>

      <div className="space-y-6">
        {/* Gas Concentration Chart */}
        <div>
          <h4 className="text-sm text-gray-400 mb-2">Gas Concentration (PPM)</h4>
          <ResponsiveContainer width="100%" height={150}>
            <AreaChart data={chartData}>
              <CartesianGrid strokeDasharray="3 3" stroke="#374151" />
              <XAxis 
                dataKey="time" 
                stroke="#9ca3af"
                fontSize={12}
                tick={{ fill: '#9ca3af' }}
              />
              <YAxis 
                stroke="#9ca3af"
                fontSize={12}
                tick={{ fill: '#9ca3af' }}
              />
              <Tooltip content={<CustomTooltip />} />
              <Area
                type="monotone"
                dataKey="gas"
                stroke="#3b82f6"
                fill="#3b82f6"
                fillOpacity={0.3}
                strokeWidth={2}
              />
            </AreaChart>
          </ResponsiveContainer>
        </div>

        {/* Distance Chart */}
        <div>
          <h4 className="text-sm text-gray-400 mb-2">Distance (cm)</h4>
          <ResponsiveContainer width="100%" height={150}>
            <LineChart data={chartData}>
              <CartesianGrid strokeDasharray="3 3" stroke="#374151" />
              <XAxis 
                dataKey="time" 
                stroke="#9ca3af"
                fontSize={12}
                tick={{ fill: '#9ca3af' }}
              />
              <YAxis 
                stroke="#9ca3af"
                fontSize={12}
                tick={{ fill: '#9ca3af' }}
              />
              <Tooltip content={<CustomTooltip />} />
              <Line
                type="monotone"
                dataKey="distance"
                stroke="#10b981"
                strokeWidth={2}
                dot={{ fill: '#10b981', strokeWidth: 2, r: 4 }}
                activeDot={{ r: 6, stroke: '#10b981', strokeWidth: 2, fill: '#10b981' }}
              />
            </LineChart>
          </ResponsiveContainer>
        </div>

        {/* Current Values Summary */}
        <div className="grid grid-cols-2 gap-4 pt-4 border-t border-gray-700">
          <div className="text-center">
            <div className="text-sm text-gray-400">Current Gas</div>
            <div 
              className="text-2xl font-bold"
              style={{ color: getGasColor(gasData) }}
            >
              {gasData ? gasData.toFixed(1) : '0.0'} PPM
            </div>
          </div>
          <div className="text-center">
            <div className="text-sm text-gray-400">Current Distance</div>
            <div 
              className="text-2xl font-bold"
              style={{ color: getDistanceColor(distanceData) }}
            >
              {distanceData || 0} cm
            </div>
          </div>
        </div>

        {/* Data Points Count */}
        <div className="text-center text-sm text-gray-400">
          {chartData.length} data points • {timeRange} range
        </div>
      </div>
    </div>
  );
};

export default DataChart;
