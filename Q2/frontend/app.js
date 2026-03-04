// Base URL of the VA Telemetry API.
// Update this to point to your emulator or deployed API if needed.
const API_BASE = 'http://localhost:3000';

async function checkHealth() {
  const badge = document.getElementById('status-badge');
  try {
    const res = await fetch(`${API_BASE}/health`);
    const data = await res.json();
    if (data.status === 'ok') {
      badge.textContent = '● API Online';
      badge.className = 'ok';
    } else {
      throw new Error('unexpected status');
    }
  } catch {
    badge.textContent = '● API Offline';
    badge.className = 'error';
  }
}

async function loadSensors() {
  const tbody = document.querySelector('#sensors-table tbody');
  try {
    const res = await fetch(`${API_BASE}/sensors`);
    const { sensors } = await res.json();
    tbody.innerHTML = sensors.map(s => `
      <tr>
        <td>${s.id}</td>
        <td>${s.name}</td>
        <td>${s.unit}</td>
        <td><span class="badge ${s.status}">${s.status}</span></td>
      </tr>`).join('');
  } catch {
    tbody.innerHTML = '<tr><td colspan="4">Failed to load sensor data.</td></tr>';
  }
}

async function loadTelemetry() {
  const tbody = document.querySelector('#telemetry-table tbody');
  try {
    const res = await fetch(`${API_BASE}/telemetry`);
    const { records } = await res.json();
    tbody.innerHTML = records.map(r => `
      <tr>
        <td>${new Date(r.timestamp).toLocaleTimeString()}</td>
        <td>${r.speed}</td>
        <td>${r.engineTemp}</td>
        <td>${r.batteryV}</td>
      </tr>`).join('');
    document.getElementById('last-updated').textContent =
      `Last updated: ${new Date().toLocaleTimeString()}`;
  } catch {
    tbody.innerHTML = '<tr><td colspan="4">Failed to load telemetry data.</td></tr>';
  }
}

function loadAll() {
  checkHealth();
  loadSensors();
  loadTelemetry();
}

// Auto-refresh every 10 seconds
loadAll();
setInterval(loadAll, 10000);
