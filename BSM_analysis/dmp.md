# Data Management Plan - BSM Dataset

## Data Summary & Purpose

**Dataset**: Basic Safety Messages (BSM) from DriveAz Mobile Application  
**Purpose**: Research and analysis of connected vehicle behavior, traffic patterns, and V2X system performance

This dataset contains BSM packets captured from connected vehicles and stored in Apache Parquet format for efficient columnar analysis. BSMs are SAE J2735-defined messages that broadcast safety-critical vehicle information including position, speed, heading, acceleration, and vehicle status at 10Hz frequency. The data supports traffic flow analysis, vehicle behavior studies, and connected vehicle system validation.

**Collection Method**: DriveAz iOS Application capture

- Data were collected via the DriveAz iOS application installed on participant devices; the app captures BSMs and associated telemetry (GPS position, speed, heading, timestamp) at the native 10 Hz reporting rate.
- Collection is opt-in and governed by informed consent; vehicle identifiers are pseudonymized on-device prior to transmission.
- Raw BSMs are buffered locally, validated, and uploaded securely (TLS) to the central repository; temporary local storage is encrypted and retained only as needed for reliable upload.
- On-device preprocessing includes basic validation (timestamp consistency, coordinate bounds) and hashing/pseudonymization of identifiers to reduce re-identification risk.
- Metadata about collection context (device model, GPS accuracy, collection start/end times) are recorded to support quality control and provenance tracking.
- Privacy and security controls described elsewhere in this DMP (pseudonymization, temporal limits, access controls) apply to DriveAz iOS-collected data.

## Standards & Formats

**Primary Data Format**: Apache Parquet - columnar storage format optimized for analytics workloads  
**Metadata Format**: CSV data dictionary with field-level documentation  
**Catalog Format**: DCAT-US v1.1 compliant JSON-LD for federal data catalog integration

**Geospatial Standard**: WGS84 coordinate reference system (EPSG:4326)  
**Coordinate Encoding**: Decimal degrees for latitude and longitude fields  
**Spatial Indexing**: Geohash encoding for efficient spatial queries  
**Temporal Standard**: UTC timestamps in ISO 8601 format

## Organization & Versioning

**Repository Structure**: This Git repository uses semantic branching with `main` as the stable branch for releases and `develop` for ongoing analysis work.

**Data File Naming**: Parquet files organized by collection period, geographic area, or data source as appropriate for the specific deployment.

**Version Control Strategy**:
- Tagged releases (v1.0, v1.1, etc.) for stable dataset versions
- Branch protection on `main` requiring peer review for documentation changes
- Commit messages follow conventional format for automated changelog generation
- Large Parquet files managed with Git LFS for efficient repository handling
- Schema evolution tracked through Parquet metadata and documentation updates

**Documentation Versioning**: Metadata files (README.md, data dictionary, DMP) are versioned alongside data to maintain consistency.

## Storage, Backup, and Access

**Primary Storage**: GitHub repository hosted at `driveAZ/driveAZ_Analytics`  
**Access Control**: Repository access controls based on data sensitivity and privacy requirements

**Backup Strategy**:
- Automated GitHub backup to organizational accounts
- Local institutional backup on research computing infrastructure
- Periodic archive snapshots to long-term storage systems
- Parquet file integrity verification through built-in checksums

**Data Integrity**: Parquet format includes built-in data validation and compression. Additional file-level checksums stored for verification.

## Privacy & Ethics

**PII Assessment**: No direct personally identifiable information is present in the published dataset.

**Vehicle Identifiers**: Vehicle IDs are pseudonymized or hashed values that cannot be linked back to actual vehicle registrations, VINs, or owners.

**Location Privacy Measures**:
- High-precision vehicle trajectories present privacy considerations for individual tracking
- Temporal aggregation and spatial generalization applied where appropriate
- Individual trip reconstruction possible within dataset timeframe

**Privacy Enhancement Techniques Applied**:
- Vehicle ID pseudonymization/hashing during data collection
- Consideration of k-anonymity for trajectory data
- Temporal boundaries to limit continuous tracking duration

**Residual Privacy Risk**: Moderate risk due to precise trajectory data; mitigated through:
- Pseudonymized identifiers with no reverse lookup capability
- Public road context (no private property locations)
- Limited temporal scope reducing long-term tracking potential
- Documentation of privacy measures for informed reuse

## Quality Control

**Data Validation Procedures**:
- Parquet schema validation ensuring consistent column structure
- Coordinate bounds checking (latitude: -90 to 90, longitude: -180 to 180)
- Speed and acceleration bounds validation (realistic vehicle performance limits)
- Timestamp chronological ordering and frequency validation
- BSM message completeness and decoding success rates

**Consistency Checks**:
- Vehicle ID sequence validation within collection periods
- Geohash consistency with coordinate fields
- BSM payload integrity verification through decoding validation
- Message counter sequence validation per vehicle

**Data Processing Quality**:
- BSM decoding error rates tracked and reported
- Spatial clustering validation for vehicle trajectory continuity
- Duplicate message detection and handling procedures

## Documentation & Metadata

**Documentation Components**:
- **README.md**: Comprehensive dataset overview and usage guide
- **dcat_metadata.json**: Federal data catalog standard metadata
- **dmp.md**: This data management plan
- **dictionary/data_dictionary.csv**: Field-level documentation with types and ranges
- **BSM_Parquet.ipynb**: Analysis workflow demonstration

**Metadata Maintenance**:
- Documentation updates accompany any schema or processing changes
- Version synchronization between data and metadata files
- Regular review cycle (quarterly) for accuracy and completeness
- Parquet schema evolution tracked in version history

## Preservation & Sharing

**Retention Plan**: Long-term retention as research dataset with historical value for connected vehicle development studies.

**Long-term Preservation**:
- Repository mirroring to institutional data repository
- Parquet format stability and cross-platform compatibility
- Migration plan for format evolution (Parquet versions, compression updates)
- Metadata preservation using Dublin Core and DataCite standards

**Sharing Strategy**:
- Controlled access through GitHub with privacy review for public sharing
- Integration with transportation research data catalogs
- Documentation for reuse in academic and industry research contexts
- Privacy-preserving aggregations for broader public data sharing

**Archive Target**: National Archives and Records Administration (NARA) or equivalent institutional repository for permanent preservation.

## Risk Assessment

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Data corruption/loss | Low | High | Version control, multiple backups, Parquet checksums |
| Privacy breach | Medium | High | Pseudonymization verification, access controls |
| Format obsolescence | Low | Low | Standard format, migration planning |
| Repository unavailability | Low | Medium | Multiple hosting locations, mirrors |
| BSM decoding failure | Medium | Medium | Multiple decoding tools, format validation |
| Trajectory re-identification | Medium | High | Privacy review, aggregation strategies |

## Roles & Responsibilities

**Data Steward**: <FILL ME: Name and contact>  
**Technical Lead**: <FILL ME: Name and contact>  
**Privacy Officer**: <FILL ME: Name and contact>

**Primary Contact**: <FILL ME: email@organization.edu>

**Organizational Affiliation**: <FILL ME: Institution/Organization>

---

*This DMP is maintained as a living document and updated with major dataset releases or methodology changes.*