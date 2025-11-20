# PSM Data Package - DriveAZ Analytics

## Overview

This folder contains Personal Safety Message (PSM) data captured from two intersections and comprehensive documentation to make this a distributable data package. PSMs are Vehicle-to-Everything (V2X) messages defined in SAE J2735 that broadcast safety-critical information from vulnerable road users (pedestrians, cyclists, etc.) to nearby vehicles.

**Data Collection**: PSM packets were captured passively at a server receiving UDP transmissions from two intersection deployments. This represents real-world V2X safety messaging in an operational environment.

**Repository Relationship**: This PSM_analysis folder within the larger driveAZ_Analytics repository serves as a self-contained data package. The parent repository contains additional analysis tools and related V2X datasets, while this folder can be distributed independently as a documented dataset.


## Data Contents and Structure

The PSM data is stored in JSONL format (JSON Lines - one JSON object per line):

- `06_27_25_080201_to_090202.jsonl` - Morning time period data (08:02-09:02 UTC)
- `06_27_2025_090202_to_100202.jsonl` - Mid-morning time period data (09:02-10:02 UTC)
- `06_27_2025_120140_to_130140.jsonl` - Afternoon time period data (12:01-13:01 UTC)

### Quick Data Loading Examples

**Python**:
```python
import json
with open('06_27_2025_090202_to_100202.jsonl', 'r') as f:
    for line in f:
        psm_record = json.loads(line)
        print(psm_record['timestamp'], psm_record['source_ip'])
```

**Command line (jq)**:
```bash
# Extract timestamps and source IPs
head -10 06_27_2025_090202_to_100202.jsonl | jq -r '.timestamp + " " + .source_ip'

# Extract PSM position data (latitude)
jq -r '.packet_data.MessageFrame[1].value[0].PersonalSafetyMessage[] | select(.position) | .position[] | select(.lat) | .lat' 06_27_2025_090202_to_100202.jsonl
```

### Environment and Notebook

Use Python 3.12 and the repository `requirements.txt`.

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

Open and run `analysis.ipynb`. The notebook includes clustering with scikit-learn and folium visualizations. If a ModuleNotFoundError appears, install into the active kernel and re-run the imports cell.

### Example PSM Record

```json
{
  "timestamp": "2025-06-27T09:02:03.264666",
  "source_ip": "10.227.92.40",
  "packet_data": {
    "MessageFrame": [
      {"messageId": "32"},
      {
        "value": [{
          "PersonalSafetyMessage": [
            {"basicType": "1"},
            {"secMark": "1800"},
            {"msgCnt": "1"},
            {"id": "41 16 72 6A"},
            {"position": [
              {"lat": "333774223"},
              {"long": "-1121674652"},
              {"elevation": "-4096"}
            ]},
            {"speed": "8191"},
            {"heading": "28800"}
          ]
        }]
      }
    ]
  },
  "messageId": "32"
}
```

## File Inventory

- **README.md**: This comprehensive documentation file
- **dcat_metadata_PSM_Specific.json**: DCAT-US v1.1 compliant metadata for data discovery and cataloging
- **dictionary/data_dictionary_psm.csv**: Package-level field documentation derived from the observed JSONL schema
- ***.jsonl**: Timestamped PSM data files in JSON Lines format
- **analysis.ipynb**: Jupyter notebook with data exploration examples

## Geospatial Information

**Coordinate Reference System**: WGS84 (EPSG:4326)

**Coordinate Format**: Latitude and longitude are encoded in J2735 format as 1/10th microdegrees and stored as integer strings in the JSONL. Convert to decimal degrees by dividing by 10,000,000. Elevation is in decimeters, with -4096 indicating unavailable.

**Coverage Area**: Two intersections in Arizona:
- Intersection A (source IP 10.227.92.40): ~33.377°N, 112.168°W
- Intersection B (source IP 10.225.242.94): ~33.568°N, 112.100°W

**Map-Matching Considerations**: PSM positions represent user device locations with varying accuracy. Position accuracy fields are included but often report maximum uncertainty values (255m). Consider applying smoothing or validation when using for precise geospatial analysis.

Tip: In folium, use English-friendly tiles:

```python
folium.Map(location=[33.45, -112.07], zoom_start=14, tiles="CartoDB positron")
```

## Time Information

**Time Zone**: All timestamps are in UTC (ISO 8601 format: YYYY-MM-DDTHH:MM:SS.ffffff)

**Collection Period**: June 27, 2025, covering morning (08:02-10:02) and afternoon (12:01-13:01) periods

**Clock Skew**: Timestamps reflect server receipt time. Minor variations (sub-second) may exist between actual message generation and server logging due to network latency and processing delays.

## Privacy and PII Statement

This dataset contains **no direct personally identifiable information (PII)**. Device identifiers present in the data are pseudonymous hex-encoded values that do not directly identify individuals or specific devices.

**Identifiers**: The `id` field contains hex-encoded identifiers (e.g., "41 16 72 6A") that appear to be hashed or pseudonymized device identifiers. These should be treated as pseudonymous data.

**Location Privacy**: While precise coordinates are included, they represent public road locations and are aggregated across multiple users and time periods, reducing individual tracking risk.

## Data Dictionary

- Package-level: `dictionary/data_dictionary.csv` (this folder)

If the schema changes, regenerate the package dictionary from the notebook by sampling keys and value domains in the loaded DataFrame/records, then update the canonical version after review.


## Licenses and citation

- Data license: see `Docs/LICENSE-data.txt` (CC BY 4.0)
- Code/docs license: see repository `LICENSE` (Apache-2.0)
- Citation and contacts: see `dcat_metadata.json` (package) and `Docs/dcat_dataset.json` (dataset)

---

*For questions about this dataset or analysis tools, see contact information in `dcat_dataset.json`.*