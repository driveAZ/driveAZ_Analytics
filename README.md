
# DriveAZ Analytics Repository

This repository contains Drive Arizona data analysis tools and datasets collected under the **Strengthening Mobility and Revolutionizing Transportation (SMART) Program** for Vehicle-to-Everything (V2X) transportation safety research.

## Repository Structure

- **PSM_analysis/**: Complete PSM (Personal Safety Message) data package with documentation
  - Contains JSONL data files from two intersection deployments
  - Includes comprehensive documentation (README, DCAT metadata, data management plan, data dictionary)
  - Self-contained and distributable as an independent dataset
- **BSM_analysis/**: Complete BSM (Basic Safety Message) data package with documentation
  - Contains Parquet data files from connected vehicles
  - Includes analysis notebook and comprehensive documentation
  - Self-contained and distributable as an independent dataset
- **notebooks/**: Jupyter notebooks for data exploration and analysis  
- **libsm/**: SAE J2735 message encoding/decoding tools

## Data Packages

### PSM Data Package

The `PSM_analysis/` folder contains a complete, documented dataset of Personal Safety Messages captured from two intersections. See `PSM_analysis/README.md` for full documentation including:

- Data structure and loading examples
- Geospatial and temporal information
- Privacy and ethics statement  
- Comprehensive data dictionary
- DCAT-US compliant metadata

### BSM Data Package

The `BSM_analysis/` folder contains Basic Safety Message data from connected vehicles in Parquet format. See `BSM_analysis/README.md` for full documentation including:

- Parquet data loading and analysis workflows
- BSM decoding using libsm tools
- Vehicle trajectory analysis examples
- Comprehensive documentation and metadata  
- Comprehensive data dictionary
- DCAT-US compliant metadata

## Development Environment Setup

Recommended: Python 3.12

### 1. Create and activate a virtual environment

```bash
python -m venv .venv
source .venv/bin/activate
```

### 2. Install Python dependencies

```bash
pip install -r requirements.txt
```

If you encounter install issues, upgrade packaging tools:

```bash
python -m pip install --upgrade pip setuptools wheel
```

### 3. Deactivate when done

```bash
deactivate
```

### Notebook tips

- If a notebook errors with "ModuleNotFoundError", install the package into the active kernel and re-run the imports cell.
- For Folium maps with English labels, use tiles="CartoDB positron".
- For Plotly mapbox without a token, use `mapbox_style="carto-positron"`.

## SAE J2735 Message Tools (libsm)

This repository includes the `libsm` binary tools for encoding and decoding SAE J2735 messages (e.g., TIM, BSM, PSM) using C++.

### Requirements

- Ubuntu 20.04 or later
- CMake ≥ 3.21
- GCC (C/C++) ≥ 9.4

### Dependencies

```bash
sudo apt update
sudo apt install -y build-essential cmake g++ libboost-all-dev
```

### Build Instructions

1. **Clone or download the repository** and move to the `b2v-libsm` directory:

```bash
cd libsm/b2v-libsm
```

2. **Create and enter the build directory**:

```bash
mkdir build
cd build
```

3. **Configure the build (disable test suite)**:

```bash
cmake .. -DLIBSM_TESTS=OFF
```

4. **Build the binaries**:

```bash
make -j$(nproc)
```

### Output

After successful compilation, the binaries will be located at:

```
build/bin/
```

Typical output binaries include:

* `createTIM`
* `decodeToJER`
* `bsmPartII`

### Verify

To test a binary:

```bash
./bin/decodeToJER -i <hex_input_file>
```

Replace `<hex_input_file>` with your input file or use echo-piped hex input.

### Notes

* If you want to build the test suite (`cpputest`), ensure CMake version ≥ 3.5 and set:

```bash
cmake .. -DLIBSM_TESTS=ON
```

* You can copy the binaries into your system path or Docker container after building.

## License

- Code and notebooks in this repository are licensed under Apache-2.0 (see `LICENSE`).
- Data packages (in `BSM_analysis/` and `PSM_analysis/`) include their own documentation and may specify data-license terms within their folders.


