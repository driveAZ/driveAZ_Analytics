# DriveAZ Project Documentation Seed

## Overview & Lineage
SMART Grant DriveAZ connected vehicle data flows: iOS mobile collection → DRIVEAZ-MSGROUTER logging/relay → CyVerse storage → analytics & packaging. 

## Data Locations (CyVerse)
- BSM Parquet objects: cyverse://bsm/*.parquet
- PSM JSONL objects: cyverse://psm/*.jsonl

## Data Dictionaries
- BSM: `data-dictionaries/data_dictionary_bsm.csv`
- PSM: `data-dictionaries/data_dictionary_psm.csv`

See `data-dictionaries/README.md` for column semantics (NTL RDM / ROSA P compliant machine-readable format).

## DCAT-US Metadata
This dataset declares a CC BY 4.0 license at the dataset level and for each distribution. See the `license` field in the dataset and distributions.

## Ethics / PII Statement
No direct personally identifiable information (PII). Vehicle/device identifiers are pseudonymous. Trajectory inference is possible; apply spatial/temporal aggregation or k-anonymity strategies before broad public release.

## Licenses & Attribution (TASL)
- Tabular dictionary artifacts (CSV) & sample tabular derivatives: CC BY 4.0 (see `LICENSE-data.txt`).

TASL Example: “SMART Grant DriveArizona Connected Vehicle Data Dictionaries — Arizona Commerce Authority / City of Phoenix — Source: https://github.com/driveAZ/driveAZ_Analytics — License: CC BY 4.0”.

TASL guidance: https://creativecommons.org/use-remix/attribution/

### Dataset-level TASL
- BSM: Title — SMART Grant DriveArizona BSM Dataset (Phoenix, 2025-06 to 2025-08); Author — Arizona Commerce Authority; Source — https://de.cyverse.org/data/ds/iplant/home/shared/DriveArizona/logs; License — CC BY 4.0 https://creativecommons.org/licenses/by/4.0/
- PSM: Title — SMART Grant DriveArizona PSM Dataset (Phoenix, 2025-06 to 2025-08); Author — City of Phoenix; Source — https://de.cyverse.org/data/ds/iplant/home/shared/DriveArizona/psm; License — CC BY 4.0 https://creativecommons.org/licenses/by/4.0/

## Contact
Marisa Walker (Arizona Commerce Authority) – marisaw@azcommerce.com  
Simon Ramos, P.E. (City of Phoenix Transportation Engineering) – simon.ramos@phoenix.gov
