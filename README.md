# Redback-Bintang – Vehicle Analytics (VA) Assessment

This repository contains starter files for the **Vehicle Analytics (VA)** assessment, split into three tasks.

---

## Repository Structure

```
Redback-Bintang/
├── Q1/                        ← Weather Data Integration (Theoretical)
│   ├── cloud_diagram.drawio   ← Blank draw.io architecture diagram
│   ├── template.pdf           ← Placeholder PDF report
│   └── README.md
│
├── Q2/                        ← API and Telemetry Dashboard
│   ├── api/
│   │   ├── package.json
│   │   ├── server.js          ← Express API (/health, /sensors, /telemetry)
│   │   └── routes/
│   │       ├── sensors.js
│   │       └── telemetry.js
│   ├── frontend/
│   │   ├── index.html         ← Single-page dashboard
│   │   └── app.js             ← Fetch + render logic
│   └── README.md
│
├── Q3/                        ← C++ Debugging
│   ├── buggy_compile.cpp      ← File with intentional compile-time errors
│   ├── buggy_runtime.cpp      ← File with intentional runtime errors
│   ├── fixes/                 ← Place corrected files here
│   └── README.md
│
└── README.md                  ← This file
```

---

## Q1 – Weather Data Integration (Theoretical)

This task requires no code. You must produce:

1. **Cloud Architecture Diagram** – edit `Q1/cloud_diagram.drawio` to show how weather station data flows through ingestion, processing, storage, and integration with vehicle telemetry systems.
2. **Written Report** – replace `Q1/template.pdf` with a PDF explaining the design.

See [`Q1/README.md`](Q1/README.md) for full requirements.

---

## Q2 – API and Telemetry Dashboard

A Node.js/Express REST API with a minimal HTML/JS frontend.

### Quick Start

```bash
# 1. Install dependencies and start the API
cd Q2/api
npm install
npm start          # runs on http://localhost:3000

# 2. Open the dashboard in a browser
open Q2/frontend/index.html
# or serve it:
cd Q2/frontend && npx serve .
```

### API Endpoints

| Method | Path | Description |
|--------|------|-------------|
| GET | `/health` | `{ status: "ok" }` |
| GET | `/sensors` | Placeholder sensor metadata |
| GET | `/telemetry` | Placeholder telemetry records |

See [`Q2/README.md`](Q2/README.md) for full instructions including emulator connection.

---

## Q3 – C++ Debugging

Two C++ files with intentional errors for debugging practice.

### Compile & Run

```bash
# Compile-time errors (will NOT compile)
g++ -Wall -Wextra -o buggy_compile Q3/buggy_compile.cpp

# Runtime errors (compiles, crashes at runtime)
g++ -Wall -Wextra -g -o buggy_runtime Q3/buggy_runtime.cpp
./buggy_runtime

# With AddressSanitizer
g++ -fsanitize=address -g -o buggy_runtime_asan Q3/buggy_runtime.cpp
./buggy_runtime_asan
```

Place corrected files in `Q3/fixes/`. See [`Q3/README.md`](Q3/README.md) for debugging tips.
