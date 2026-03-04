# Q2 – API and Telemetry Dashboard

This folder contains a Node.js REST API and a minimal HTML/JS frontend dashboard for visualising vehicle sensor and telemetry data.

## Folder Structure

```
Q2/
├── api/
│   ├── package.json
│   ├── server.js        ← Express API with /health, /sensors, /telemetry
│   └── routes/
│       ├── sensors.js
│       └── telemetry.js
├── frontend/
│   ├── index.html       ← Single-page dashboard
│   └── app.js           ← Fetch calls + chart/table rendering
└── README.md
```

## Prerequisites

- [Node.js](https://nodejs.org/) v18 or later
- npm (bundled with Node.js)

---

## Running the API

```bash
cd Q2/api
npm install
npm start
```

The API will start on **http://localhost:3000**.

### Available Endpoints

| Method | Path | Description |
|--------|------|-------------|
| GET | `/health` | Health check – returns `{ status: "ok" }` |
| GET | `/sensors` | Placeholder list of sensor metadata |
| GET | `/telemetry` | Placeholder telemetry data stream |

---

## Running the Frontend

Open `Q2/frontend/index.html` directly in a browser, **or** serve it with a simple static server:

```bash
cd Q2/frontend
npx serve .
```

Then navigate to **http://localhost:5000** (or the port shown by `serve`).

> The dashboard fetches data from `http://localhost:3000` by default. Make sure the API is running first.

---

## Connecting to the Emulator Service

If you are using a vehicle emulator (e.g., the Redback Operations emulator), update the `API_BASE` constant in `frontend/app.js` to point to the emulator's address:

```js
// frontend/app.js
const API_BASE = 'http://<emulator-host>:<port>';
```

Then restart the frontend.

---

## Development Notes

- All route handlers in `api/routes/` return placeholder JSON responses — replace them with real data sources.
- The frontend chart placeholder uses a simple HTML `<table>`; replace it with Chart.js or similar as needed.
