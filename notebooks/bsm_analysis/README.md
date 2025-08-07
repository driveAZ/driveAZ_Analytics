# BSM Parquet Analysis - Notebook README

## Overview
This notebook demonstrates how to process, decode, and visualize Basic Safety Message (BSM) data stored in Parquet files. It is designed for use with the driveAZ Analytics project and leverages the custom `libsm` C library for decoding BSM payloads.

## Directory Structure
- `notebooks/bsm_analysis/` - Contains the analysis notebook and related files.
  - `parquet/` - Directory containing BSM data in Parquet format.
- `libsm/b2v-libsm/` - Contains the C library and decoder binary (`decodeToJER`) used for BSM decoding.

## Parquet Files
Place your BSM Parquet files in the `notebooks/bsm_analysis/parquet/` directory. The notebook will automatically detect and load all `.parquet` files in this folder.

## Dependencies
- Python packages: `pandas`, `matplotlib`, `seaborn`, `pyarrow`, `plotly`, `numpy` (install via `pip install -r requirements.txt`)
- C library: `libsm` (must be built; the decoder binary should be at `libsm/b2v-libsm/build/bin/decodeToJER`)

## Usage Notes
- Ensure the `decodeToJER` binary is built and available at the expected path before running the decoding cells in the notebook.
- The notebook is organized so that each code cell is preceded by a markdown cell explaining its purpose.

---
