## Data Dictionaries

This folder contains machine-readable data dictionaries aligned with National Transportation Library (NTL) Research Data Management Data Package guidance and ROSA P submission expectations.

### Files
- `data_dictionary_bsm.csv` – Fields derived from profiling Basic Safety Message (BSM) Parquet data (Table = `bsm_parquet`).
- `data_dictionary_psm.csv` – Fields flattened from Personal Safety Message (PSM) JSONL data (Table = `psm_jsonl`).

### Column Definitions
| Column | Purpose |
|--------|---------|
| Table | Logical dataset/table identifier for joins or multi-dataset packaging |
| VariableName | Canonical field name (dot-notation for nested JSON) |
| Label/Description | Human-readable description of the variable |
| Type | Logical data type (int, float, string, binary, etc.) |
| Unit/Format | Measurement unit or representation format (e.g., decimal degrees, ISO-8601) |
| AllowedValues/Range | Enumerated set or numeric range constraint |
| MissingValue(s) | Sentinel values representing missing/unavailable data |
| Derivation/Formula | How the value is produced or derived |
| RelationTo | Related variables (e.g., Latitude ↔ Longitude) |
| Example | Representative example value from the dataset |
| Source/Provenance | High-level ingestion lineage |
| QA/Validation | Rule(s) for validation or quality screening |
| Notes | Ancillary context or decoding information |

### Provenance
- BSM: DriveAZ-IOS App to DRIVEAZ-MSGROUTER to Cyverse
- PSM: NoTraffic → City of Phoenix server

Note: The "Source/Provenance" column in each CSV reflects this lineage as "NoTraffic to City of Phoenix server" for field-level provenance.

### Licensing
These dictionaries (as tabular documentation) are released under **CC BY 4.0** (see `../LICENSE-data.txt`). Provide TASL attribution when reusing: Title, Author, Source, License.

TASL example: “SMART Grant DriveArizona Connected Vehicle Data Dictionaries — Arizona Commerce Authority / City of Phoenix — Source: https://github.com/driveAZ/driveAZ_Analytics — License: CC BY 4.0 (https://creativecommons.org/licenses/by/4.0/)”.