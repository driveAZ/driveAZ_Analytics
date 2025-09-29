# BSM Data Package - DriveAZ Analytics

## Overview

This folder contains Basic Safety Message (BSM) data analysis tools and documentation to make this a distributable data package. BSMs are Vehicle-to-Everything (V2X) messages defined in SAE J2735 that broadcast safety-critical information from connected vehicles including position, speed, heading, and vehicle status.

**Data Collection**: BSM packets captured from connected vehicles and stored in Parquet format for efficient analysis. BSMs represent the most common V2X message type, broadcasting vehicle state information at 10Hz frequency.

**Repository Relationship**: This BSM_analysis folder within the larger driveAZ_Analytics repository serves as a self-contained data package. The parent repository contains additional analysis tools and related V2X datasets, while this folder can be distributed independently as a documented dataset.

## Data Contents and Structure

The BSM data is stored in Parquet format for efficient columnar processing:

- `parquet/` directory contains BSM data files in Apache Parquet format
- `BSM_Parquet.ipynb` - Jupyter notebook demonstrating data loading, decoding, and visualization
- Analysis workflow includes geohash-based filtering, BSM decoding using libsm tools, and movement visualization

### Quick Data Loading Examples

**Python (Pandas)**:
```python
import pandas as pd
import glob

# Load all Parquet files
parquet_files = glob.glob("parquet/*.parquet")
df = pd.concat([pd.read_parquet(f) for f in parquet_files], ignore_index=True)
print(df.info())
```

**Python (PyArrow)**:
```python
import pyarrow.parquet as pq

# Read single file
table = pq.read_table("parquet/bsm_data.parquet")
df = table.to_pandas()
```

### Expected BSM Data Structure

BSM Parquet files typically contain:
- **timestamp**: Message receipt timestamp (UTC)
- **geohash**: Spatial indexing for efficient geographic queries
- **vehicle_id**: Pseudonymized vehicle identifier
- **latitude/longitude**: Vehicle position (WGS84)
- **speed**: Vehicle speed (m/s)
- **heading**: Vehicle heading (degrees)
- **elevation**: Vehicle elevation (meters)
- **bsm_payload**: Raw BSM message payload (hex-encoded)
- **message_count**: BSM sequence counter
- **transmission_state**: Vehicle transmission state

## File Inventory

- **README.md**: This comprehensive documentation file
- **dcat_metadata.json**: DCAT-US v1.1 compliant metadata for data discovery and cataloging
- **dmp.md**: Data Management Plan outlining data handling, privacy, and preservation practices
- **dictionary/data_dictionary.csv**: Detailed field-by-field documentation of all data elements
- **BSM_Parquet.ipynb**: Analysis notebook with data processing workflows
- **parquet/**: Directory containing BSM data files in Parquet format

## Geospatial Information

**Coordinate Reference System**: WGS84 (EPSG:4326)

**Coordinate Format**: Decimal degrees for latitude and longitude fields

**Spatial Indexing**: Geohash encoding for efficient spatial queries and geographic grouping

**Coverage Area**: <FILL ME: Specify geographic coverage based on actual data>

**Map-Matching Considerations**: BSM positions represent real-time vehicle GPS locations with typical automotive GPS accuracy (3-5 meters). Consider temporal smoothing for trajectory analysis.

## Time Information

**Time Zone**: All timestamps are in UTC (ISO 8601 format)

**Collection Period**: <FILL ME: Specify actual collection timeframe>

**Frequency**: BSMs are typically broadcast at 10Hz (100ms intervals) when vehicles are in motion

**Clock Synchronization**: Timestamps may reflect either vehicle broadcast time or server receipt time depending on data collection methodology.

## Privacy and PII Statement

This dataset contains **no direct personally identifiable information (PII)**. Vehicle identifiers present in the data are pseudonymous values that do not directly identify specific vehicles or owners.

**Vehicle Identifiers**: BSM vehicle IDs are pseudonymized or hashed values that cannot be linked back to actual vehicle registrations or owners.

**Location Privacy**: While precise vehicle coordinates are included, they represent public road locations. Consider temporal and spatial aggregation for enhanced privacy when publishing or sharing.

**Trajectory Tracking**: Individual vehicle movements can be tracked within the dataset timeframe. Appropriate privacy measures should be applied for public data sharing.

## BSM Decoding Workflow

The analysis workflow leverages the `libsm` C library for BSM message decoding:

1. **Data Loading**: Parquet files are loaded and concatenated using pandas
2. **Spatial Filtering**: Geohash-based filtering for area-of-interest analysis
3. **BSM Decoding**: Raw BSM payloads decoded using `libsm/b2v-libsm/build/bin/decodeToJER`
4. **Data Extraction**: Structured data extracted from decoded JSON-ER format
5. **Analysis & Visualization**: Vehicle trajectories and patterns analyzed using Plotly

## How to Regenerate Data Dictionary

The CSV data dictionary (`dictionary/data_dictionary.csv`) should be generated by sampling the actual Parquet files to identify all fields, infer types, and determine value ranges. To regenerate:

1. Load sample Parquet files using pandas or PyArrow
2. Analyze schema and extract field metadata
3. Compute value ranges and examples from actual data
4. Update required/optional status based on field presence frequency
5. Include BSM-specific field documentation from SAE J2735 standard

## Why Validate This Data?

Data validation provides several key benefits for V2X research and development:

• **Schema Consistency**: Ensures all Parquet files follow consistent column structure and data types
• **Coordinate Validation**: Confirms latitude/longitude values are within reasonable geographic bounds
• **Temporal Integrity**: Validates timestamp ordering and frequency patterns consistent with 10Hz BSM broadcasting
• **Message Completeness**: Verifies BSM payload integrity and successful decoding rates
• **Reproducible Analysis**: Enables confident reuse of validated datasets across different research workflows

## Dependencies

**Python Packages**: pandas, matplotlib, seaborn, pyarrow, plotly, numpy
**External Tools**: libsm C library with decodeToJER binary for BSM message decoding
**Build Requirements**: See `libsm/b2v-libsm/README.md` for compilation instructions

## Citation

<FILL ME: Add appropriate citation format when publishing>

## License

<FILL ME: Specify license terms>

---

*For questions about this dataset or analysis tools, see contact information in `dcat_metadata.json`.*
