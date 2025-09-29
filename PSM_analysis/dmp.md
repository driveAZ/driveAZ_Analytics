# Data Management Plan - PSM Dataset

## Data Summary & Purpose

**Dataset**: Personal Safety Messages (PSM) from Two Intersections  
**Purpose**: Research and quality assurance of Vehicle-to-Everything (V2X) safety communication systems

This dataset contains PSM packets captured passively at a server from UDP transmissions originating from two intersection deployments. PSMs are SAE J2735-defined messages that broadcast safety-critical information from vulnerable road users (pedestrians, cyclists, emergency responders) to nearby connected vehicles. The data supports analysis of V2X deployment effectiveness, message frequency patterns, and geospatial coverage characteristics.

**Collection Method**: Passive UDP packet capture at server endpoints receiving transmissions from intersection-based roadside units (RSUs).

## Standards & Formats

**Primary Data Format**: JSONL (JSON Lines) - one JSON object per line for efficient streaming processing  
**Metadata Format**: CSV data dictionary with field-level documentation  
**Catalog Format**: DCAT-US v1.1 compliant JSON-LD for federal data catalog integration

**Geospatial Standard**: WGS84 coordinate reference system (EPSG:4326)  
**Coordinate Encoding**: SAE J2735 format (1/10th microdegrees as integers)  
**Temporal Standard**: UTC timestamps in ISO 8601 format (YYYY-MM-DDTHH:MM:SS.ffffff)

## Organization & Versioning

**Repository Structure**: This Git repository uses semantic branching with `main` as the stable branch for releases and `develop` for ongoing analysis work.

**Data File Naming**: Time-based naming convention `MM_DD_YYYY_HHMMSS_to_HHMMSS.jsonl` indicating data collection period in UTC.

**Version Control Strategy**:
- Tagged releases (v1.0, v1.1, etc.) for stable dataset versions
- Branch protection on `main` requiring peer review for documentation changes
- Commit messages follow conventional format for automated changelog generation
- Data files are tracked with Git LFS for efficient repository management

**Documentation Versioning**: Metadata files (README.md, data dictionary, DMP) are versioned alongside data to maintain consistency.

## Storage, Backup, and Access

**Primary Storage**: GitHub repository hosted at `driveAZ/driveAZ_Analytics`  
**Access Control**: Public repository with open access to promote research collaboration

**Backup Strategy**:
- Automated GitHub backup to organizational accounts
- Local institutional backup on research computing infrastructure
- Periodic archive snapshots to long-term storage systems

**Data Integrity**: SHA-256 checksums computed for all data files and stored in `checksums.txt` for verification.

## Privacy & Ethics

**PII Assessment**: No direct personally identifiable information is present in the published dataset.

**Pseudonymous Identifiers**: Device IDs are hex-encoded values (e.g., "41 16 72 6A") that appear to be hashed or pseudonymized. Original device identifiers are not recoverable from published values.

**Location Privacy Measures**:
- Data represents public road intersections with inherent traffic visibility
- Temporal aggregation across multiple time periods reduces individual tracking potential
- No direct linking to vehicle registration or user account information

**Residual Linkage Risk**: Low risk of re-identification due to:
- Pseudonymized device identifiers
- Public location context (intersection environments)
- Temporal aggregation reducing continuous tracking potential

**Mitigation Strategies Applied**:
- Review of sample data confirmed absence of direct identifiers
- Device ID pseudonymization maintained throughout processing pipeline
- Documentation of privacy-preserving measures for transparency

## Quality Control

**Data Validation Procedures**:
- JSONL parsing validation ensures structural integrity of all records
- Coordinate range validation (latitude: 33.3-33.6°N, longitude: 112.0-112.2°W)
- Timestamp format validation and chronological ordering checks
- Speed and heading value bounds checking (speed < 200 mph, heading 0-360°)

**Consistency Checks**:
- Message counter sequence validation within device ID groups
- Source IP consistency with expected intersection endpoints
- Duplicate record detection based on timestamp + device ID combinations

**Documentation Quality**:
- Automated data dictionary generation from sample analysis
- Field coverage reporting (percentage of records containing each field)
- Type inference validation against J2735 specification

## Documentation & Metadata

**Documentation Components**:
- **README.md**: Comprehensive dataset overview and usage guide
- **dcat_metadata.json**: Federal data catalog standard metadata
- **dmp.md**: This data management plan
- **dictionary/data_dictionary.csv**: Field-level documentation with types and ranges

**Metadata Maintenance**:
- Documentation updates accompany any schema or processing changes
- Version synchronization between data and metadata files
- Regular review cycle (quarterly) for accuracy and completeness

## Preservation & Sharing

**Retention Plan**: Indefinite retention as research dataset with historical value for V2X development studies.

**Long-term Preservation**:
- Repository mirroring to institutional data repository
- Format migration plan for JSONL/CSV if standards evolve
- Metadata preservation using Dublin Core and DataCite standards

**Sharing Strategy**:
- Open access through GitHub with DOI assignment for citation
- Integration with transportation data catalogs (e.g., USDOT data hub)
- Documentation for reuse in academic and industry research contexts

**Archive Target**: National Archives and Records Administration (NARA) or equivalent institutional repository for permanent preservation.

## Risk Assessment

| Risk | Likelihood | Impact | Mitigation |
|------|-----------|--------|------------|
| Data corruption/loss | Low | High | Version control, multiple backups, checksums |
| Privacy breach | Very Low | Medium | PII review, pseudonymization verification |
| Format obsolescence | Medium | Low | Standard formats, migration planning |
| Repository unavailability | Low | Medium | Multiple hosting locations, mirrors |
| Incomplete documentation | Medium | High | Automated validation, review cycles |

## Roles & Responsibilities

**Data Steward**: <FILL ME: Name and contact>  
**Technical Lead**: <FILL ME: Name and contact>  
**Privacy Officer**: <FILL ME: Name and contact>

**Primary Contact**: <FILL ME: email@organization.edu>

**Organizational Affiliation**: <FILL ME: Institution/Organization>

---

*This DMP is maintained as a living document and updated with major dataset releases or methodology changes.*