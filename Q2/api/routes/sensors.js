const express = require('express');
const router = express.Router();

// GET /sensors – returns placeholder sensor metadata
router.get('/', (req, res) => {
  res.json({
    message: 'Placeholder – replace with real sensor data',
    sensors: [
      { id: 'spd-01', name: 'Speed Sensor',       unit: 'km/h',  status: 'active' },
      { id: 'tmp-01', name: 'Engine Temp Sensor',  unit: '°C',    status: 'active' },
      { id: 'bat-01', name: 'Battery Voltage',     unit: 'V',     status: 'active' },
      { id: 'acc-01', name: 'Accelerometer',       unit: 'm/s²',  status: 'inactive' },
    ],
  });
});

module.exports = router;
