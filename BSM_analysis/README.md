# BSM Data Package - DriveAZ Analytics

## Overview

This folder contains Basic Safety Message (BSM) analysis assets and documentation. BSMs are SAE J2735 V2X messages carrying vehicle state such as position and heading. Data files are provided in Parquet for efficient analytics, with a companion notebook to load, filter, optionally decode, and visualize trajectories.
.

## Contents

- `parquet/` – Parquet files with BSM records
- `BSM_Parquet.ipynb` – Notebook: load, filter, optionally decode, and visualize
- `dictionary/data_dictionary_bsm.csv` – Field-by-field documentation for this package
- `dcat_metadata.json` – DCAT-US v1.1 metadata



## Actual Parquet schema (current files)

The included Parquet files have six columns:

- `mf_bytes` (bytes) – Raw message frame bytes
- `TimeStamp` (float) – Unix epoch seconds (UTC)
- `MessageType` (string) – Message name (e.g., BasicSafetyMessage)
- `Geohash` (string) – Spatial index for quick filtering/grouping
- `Latitude` (float) – Decimal degrees (WGS84)
- `Longitude` (float) – Decimal degrees (WGS84)

Your downstream decoding can expand these into BSM coreData fields if needed.

## Quickstart

1) Create/activate a Python 3.12 virtual environment and install deps:

```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

2) Open and run `BSM_Parquet.ipynb`:

- It will safely load all files under `parquet/`, show schema and samples
- You can filter by geohash (e.g., Phoenix AOI) and generate maps/animations

## Visualization tips (English tiles and Phoenix AOI)

Folium maps with English labels:

```python
import folium
m = folium.Map(location=[lat, lon], zoom_start=15, tiles="CartoDB positron")
```

Plotly mapbox (no token) using Carto tiles:

```python
fig.update_layout(mapbox_style="carto-positron")
```

Filter to Phoenix bounding box (example):

```python
lat_min, lat_max = 33.2, 33.8
lon_min, lon_max = -112.3, -111.8
df_phx = df[(df.Latitude.between(lat_min, lat_max)) & (df.Longitude.between(lon_min, lon_max))]
```

## Optional decoding with libsm

If you need decoded BSM fields from `mf_bytes`, build the decoder once:

```bash
cd libsm/b2v-libsm
mkdir -p build && cd build
cmake .. && make -j$(nproc)
```

The notebook expects the binary at `libsm/b2v-libsm/build/bin/decodeToJER`. Decoding is called per-row with a short timeout; ensure your focus set is small before bulk decode.


## Validation checklist

- Latitude in [-90, 90], Longitude in [-180, 180]
- TimeStamp parses to reasonable UTC times; order consistent within a vehicle/geohash
- Geohash matches expected base32 and length
- `mf_bytes` present and non-empty for decodable rows

## Time and coverage

- Time zone: UTC
- Example capture times: see notebook output and Parquet filenames
- Coverage: Records include Beijing sample geohash and Phoenix-focused analysis; filter to your AOI as needed

## Privacy

No direct PII is included. Points represent public-road GPS fixes. Use spatial/temporal aggregation when sharing trajectories.

## Licenses and citation

- Data: see `Docs/LICENSE-data.txt` (CC BY 4.0)
- Code/docs: see repository `LICENSE` (Apache-2.0)
---

For questions, see contacts in `dcat_metadata.json`.
