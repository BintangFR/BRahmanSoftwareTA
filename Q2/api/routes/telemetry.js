const express = require('express');
const router = express.Router();

// GET /telemetry – returns placeholder telemetry records
router.get('/', (req, res) => {
  const now = Date.now();
  res.json({
    message: 'Placeholder – replace with real telemetry stream',
    records: Array.from({ length: 5 }, (_, i) => ({
      timestamp: new Date(now - i * 1000).toISOString(),
      speed:       Math.round(60 + Math.random() * 40),
      engineTemp:  Math.round(80 + Math.random() * 20),
      batteryV:    +(48 + Math.random() * 4).toFixed(2),
    })),
  });
});

module.exports = router;
