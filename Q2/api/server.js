const express = require('express');
const cors = require('cors');
const sensorsRouter = require('./routes/sensors');
const telemetryRouter = require('./routes/telemetry');

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());

// Health check
app.get('/health', (req, res) => {
  res.json({ status: 'ok', timestamp: new Date().toISOString() });
});

// Feature routes
app.use('/sensors', sensorsRouter);
app.use('/telemetry', telemetryRouter);

app.listen(PORT, () => {
  console.log(`VA Telemetry API running on http://localhost:${PORT}`);
});

module.exports = app;
