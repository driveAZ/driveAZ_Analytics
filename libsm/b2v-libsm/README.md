# b2v-libsm: SAE J2735 Safety Messaging Library

This library provides functions for working with PSMs and BSMs.

src/j2735 and src/j2540 contain the slightly modified output of https://github.com/mouse07410/asn1c


## Features
* functions for allocating J2735 structures and filling required fields
* helpers for encode/decode UPER to struct

### Naming Conventions
We have some classes of function that have certain properties.

* `libsm_alloc_init_$name`
  * Return a pointer to a new object, which must pass constraints (i.e. minimum viable). May have params for required fields.
* `libsm_alloc_init_partial_$name`
  * Return a pointer to a new object that does not pass constraints (i.e. minimum viable). The user must finish setting it up.
* `libsm_init_$name`
  * Take an allocated pointer, and make it have valid constraints (i.e. minimum viable). May have params for required fields.
  * note that this may still allocate internal fields
* `libsm_set_$name`
  * Take two pointers, put second pointer into first pointer.
* `libsm_get_$name`
  * Return a pointer from inside a pointer.


## Usage
### build
```
git submodule update --init
cmake -B build        # generate project buildsystem
cmake --build build/  # build the project
```
examples and tools and test binary are in build/bin

### Unit testing
We're using http://cpputest.github.io/

compile, then
```
./build/bin/test_libsm
```
to get junit output written to working dir
```
./build/bin/test_libsm -o junit
```


### Examples/tools
`examples` provides a few usage examples
* `bsmPartII.c` creates a BSM with part II data
* `createPSMBSM` creates a PSM and a BSM with some example data and encodes it
* `create-SPAT.c` creates a SPAT message
* `create-TIM.c` creates a TIM with sample data and encodes it
* `validator.c` Validates a UPER-encoded J2735 message



## Instructions for compiling the ASN to c code
This should work on Linux and macOS.

* run `./tooling/gen-j2735.sh ../path/to/J2735.ASN`

This will
* Make a temp directory, fetch the correct ASN1c and compile it
* Run asn1c on the asn file passed in
* Modify the output of ASN1C
  * `s/include <INTEGER.h>/include "INTEGER.h"/`
    * fixes a name conflict between INTEGER.h in j2735, and integer.h in fatfs
      because HFS is case insensitive and this needs to work on a mac.
  * Change the word "free" to "asn_free" in places asn1c uses it to not mean
    the function free, so our unit test framework can redefine free.
