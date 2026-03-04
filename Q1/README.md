# Q1 – Weather Data Integration (Theoretical)

This task is **theoretical** — no code implementation is required. Instead, you must produce:

## Deliverables

### 1. Cloud Architecture Diagram
Update or create the file `cloud_diagram.drawio` (a blank placeholder is included) to illustrate:
- Ingestion of data from weather stations (e.g., via MQTT, REST APIs, or streaming services)
- Processing, storage, and access layers in the cloud (e.g., AWS IoT Core / Azure IoT Hub, data lakes, time-series databases)
- Integration points with existing vehicle telemetry and log systems

### 2. Written Report (PDF)
Produce a PDF (replace `template.pdf`) that explains:
- **Data Ingestion**: How weather data is collected from stations and pushed to the cloud
- **Processing & Storage**: How data is cleaned, transformed, and stored (batch vs. stream)
- **Access Layer**: APIs or query interfaces used by other services
- **Integration**: How weather data is merged with existing vehicle telemetry/log pipelines

## Files
| File | Purpose |
|------|---------|
| `cloud_diagram.drawio` | Blank draw.io file — fill in your architecture diagram |
| `template.pdf` | Placeholder — replace with your completed PDF report |
| `README.md` | This file |

## Notes
- Edit `cloud_diagram.drawio` using [draw.io](https://app.diagrams.net/) (free, browser-based)
- Export your final diagram as an image or embed it in the PDF
- The PDF should be self-contained and readable without the diagram file
