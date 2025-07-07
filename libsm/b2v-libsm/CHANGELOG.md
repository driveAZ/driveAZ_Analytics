# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [8.0.0](https://github.com/tome-github/b2v-libsm/compare/v7.2.0...v8.0.0) (2022-11-28)


### ⚠ BREAKING CHANGES

* libsm_str_basic_vehicle_class and libsm_basic_vehicle_class_from_str now output strings matching J2735 by replacing underscore with dash. The parsing functions support both.
* Combine LIBSM_FAIL_NO_$x into LIBSM_FAIL_NULL_ARG
* in J2735 202007, there was some minor renaming and spelling fixes, and TimeMark had a breaking change.
  From the PDF:
  > DE_TimeMark was modified to better support leap seconds; otherwise,
  > there were no changes that affect backward compatibility.

  Based on the diff:
  * DSRC_TimeMark -> SPAT_TimeMark
  * DSRC_Elevation -> Common_Elevation
  * DSRC_MsgCount -> Common_MsgCount
  * DSRC_Angle -> Common_Angle
  * DE_LaneSharing.pedestrianTraffic -> reserved
  * DE_TravelerDataFrame.duraton spelling fix
  * Some regional extension stuff I don't understand
* **asn1c:** ASN1C renamed ATS_BASIC_JER to ATS_JER to comply with ASN1 standards
* LIBSM_VERSION_COMPATIBLE has been inverted to return 1(true) when compatible and 0(false) when incompatible.

### Features

* path prediction ([7e805ad](https://github.com/tome-github/b2v-libsm/commit/7e805ad842fc2750f7b537a138b5be94e41a518a))
* update J2735 to 2020 07 ([df7fb76](https://github.com/tome-github/b2v-libsm/commit/df7fb76d19eb7e1be52b6e3ba9a129ea8655f3d2))


### Bug Fixes

* Combine NULL argument errors ([bedcddf](https://github.com/tome-github/b2v-libsm/commit/bedcddf007de54e6a5d2e6ee5c4337bca40c77c9))
* invert version check ([98aec8b](https://github.com/tome-github/b2v-libsm/commit/98aec8bafe4504e1048f77bf804af81d732a7160))
* use J2735 basic type strings ([4f51032](https://github.com/tome-github/b2v-libsm/commit/4f5103213e8bbd8db4f08d8a0af5bfd515df7895))


### build

* **asn1c:** rename ATS_BASIC_JER to ATS_JER ([c79ba81](https://github.com/tome-github/b2v-libsm/commit/c79ba81bd1a13d629d929f461805ff10fe147ccc))
* Some cmake support was added, but it is not yet complete

## [7.2.0](https://github.com/tome-github/b2v-libsm/compare/v7.1.0...v7.2.0) (2022-08-18)


### Features

* add MessageFrame helper functions ([e07cbe7](https://github.com/tome-github/b2v-libsm/commit/e07cbe7d626ceff30f6daf6eaab3b266c58f0d22))
* **bitstring:** init functions ([13d5780](https://github.com/tome-github/b2v-libsm/commit/13d57801c8efda2acab89ba9211321e3c97d11c3))

## [7.1.0](https://github.com/tome-github/b2v-libsm/compare/v7.0.0...v7.1.0) (2022-07-28)


### Features

* update asn1c for JER support ([3009197](https://github.com/tome-github/b2v-libsm/commit/300919727306f2e8ecc66ce096d3eba2c67ba97e))

## [7.0.0](https://github.com/tome-github/b2v-libsm/compare/v6.5.0...v7.0.0) (2022-05-05)


### ⚠ BREAKING CHANGES

* If you built this repo with segger you now need to
maintain your own segger_bodge named config.h and define HAVE_CONFIG_H


### build

* remove segger_bodge.h ([d4164e1](https://github.com/tome-github/b2v-libsm/commit/d4164e17729e9acf2847c467b7b115615f260b53))

## [6.5.0](https://github.com/tome-github/b2v-libsm/compare/v6.4.1...v6.5.0) (2022-04-07)


### Features

* init TemporaryID function ([443af18](https://github.com/tome-github/b2v-libsm/commit/443af182600b9997d7acec6b8074e2d778fc3222))

### [6.4.1](https://github.com/tome-github/b2v-libsm/compare/v6.4.0...v6.4.1) (2022-03-04)


### Bug Fixes

* **segger_bodge:** segger doesn't define struct timespec ([e8189f8](https://github.com/tome-github/b2v-libsm/commit/e8189f88e5750ef9fb5e989b14937b567302f5b2))

## [6.4.0](https://github.com/tome-github/b2v-libsm/compare/v6.3.1...v6.4.0) (2022-03-02)


### Features

* add packet error rate functions ([ce4a265](https://github.com/tome-github/b2v-libsm/commit/ce4a26562244f7e8c4e5149307ffc8c99a673ff7))

### [6.3.1](https://github.com/tome-github/b2v-libsm/compare/v6.3.0...v6.3.1) (2022-01-24)


### Bug Fixes

* LIBSM_VERSION_COMPATIBLE now works, but is still backwards ([42d5b31](https://github.com/tome-github/b2v-libsm/commit/42d5b3166c2c27e2a5d96b03fab1bb17ab87b0aa))

## [6.3.0](https://github.com/tome-github/b2v-libsm/compare/v6.2.1...v6.3.0) (2022-01-20)


### Features

* add libsm_encode_messageframe ([8816ab5](https://github.com/tome-github/b2v-libsm/commit/8816ab5b4748531bf6349b8f48d5aa0775d1367b))

### [6.2.1](https://github.com/tome-github/b2v-libsm/compare/v6.2.0...v6.2.1) (2021-09-09)


### Bug Fixes

* lint.sh macOS workaround ([3769618](https://github.com/tome-github/b2v-libsm/commit/376961843087faf92f269a6d621e640dca8a6ef4))

### Build

* rename files to be more different than case ([3b06299](https://github.com/tome-github/b2v-libsm/commit/3b0629915d7aee2a4b47e356ee6d1c9521880540))

## [6.2.0](https://github.com/tome-github/b2v-libsm/compare/v6.1.0...v6.2.0) (2021-07-14)


### Features

* add path history timestamp functions ([d78ce2a](https://github.com/tome-github/b2v-libsm/commit/d78ce2a03ec1b651afc3f34cbd1af3f7056eb8b8))
* **j2735:** add defines for units(min/max/etc) ([e8749d7](https://github.com/tome-github/b2v-libsm/commit/e8749d7213f7e833d42297b026c860e51cd467af))
* **j2735-defines:** add RANGE_CHECK macro and fix RANGE_COERCE ([7f3a0cd](https://github.com/tome-github/b2v-libsm/commit/7f3a0cd7293f398e5db6d812173fbd07fda7ceee))
* **libsm:** add string to basic/device type functions ([fa9e9ca](https://github.com/tome-github/b2v-libsm/commit/fa9e9ca417e7fd5ac77af9ce301a8c9e9cb37eb9))
* **libsm:** allocate path prediction ([1e3e780](https://github.com/tome-github/b2v-libsm/commit/1e3e780f0b8a58d770eb626aac82296c103a2d98))


### Bug Fixes

* use proper versions for clang warnings ([fe30eea](https://github.com/tome-github/b2v-libsm/commit/fe30eea1d9930471766b14b79af32153dae96cb4))
* warning flags are now correct for different compilers ([9261eb6](https://github.com/tome-github/b2v-libsm/commit/9261eb66469c92d3ac317225d9880107060f9ac2))

## [6.1.0](https://github.com/tome-github/b2v-libsm/compare/v6.0.0...v6.1.0) (2021-06-15)


### Features

* **j2735-defines:** add conversion defines ([ea15385](https://github.com/tome-github/b2v-libsm/commit/ea15385f1af645f63d25c07426342a6f0b4aa1fc))


### Bug Fixes

* **Makefile.include:** wildcard properly ([1b64d56](https://github.com/tome-github/b2v-libsm/commit/1b64d56904c07f1eded1becb6ce6a2ce85052c89))
* **mf2CSV:** acceleration conversion ([fe2bb43](https://github.com/tome-github/b2v-libsm/commit/fe2bb43579d6ccac7d63af5fe164f053d650b32b))
* **mf2CSV:** display debugstring ([f63a4e3](https://github.com/tome-github/b2v-libsm/commit/f63a4e370a81ee20442e310926981599b6e6879e))
* **mf2CSV:** format end of debugstring ([093aa01](https://github.com/tome-github/b2v-libsm/commit/093aa0111ecf0b6c173315b32ac1b8962f60839a))
* **mf2CSV:** reformat utc seconds and offset signage ([c682c6e](https://github.com/tome-github/b2v-libsm/commit/c682c6e213f1c197bbbc787bad47150a8c2f4e00))

## [6.0.0](https://github.com/tome-github/b2v-libsm/compare/v5.2.0...v6.0.0) (2021-04-29)


### ⚠ BREAKING CHANGES

* API for libsm_get_basic_vehicle_class is now pass by reference to avoid giant stack usage.
* **cmdline_utils:** Now depends on b2v-common

### Features

* **mf2CSV:** add msgCount ([99f47c2](https://github.com/tome-github/b2v-libsm/commit/99f47c26dbc1904fd1edb56b7491d091dbcd631f))
* **mf2CSV:** add RSSI and Channel ([e97a168](https://github.com/tome-github/b2v-libsm/commit/e97a16859ed80300e4704beb0ea07ca84662a1ef))


### Bug Fixes

* don't pass bsm by value to libsm_get_basic_vehicle_class ([4f12ac6](https://github.com/tome-github/b2v-libsm/commit/4f12ac697d3ec9c67003777a1b02ea8e006d28d5))
* **cmdline_utils:** increase mf buffer size ([1d813e6](https://github.com/tome-github/b2v-libsm/commit/1d813e6715bf3a887108273e30fbde41ab2ee994))
* **mf2csv:** don't use floats for printing ([86b5722](https://github.com/tome-github/b2v-libsm/commit/86b5722b3b20c1906002021a446d7dae07ada329))

## [5.2.0](https://github.com/tome-github/b2v-libsm/compare/v5.1.1...v5.2.0) (2021-04-15)


### Features

* add functions to access path history timestamp ([4981b79](https://github.com/tome-github/b2v-libsm/commit/4981b79ce437254667bb75f7798fd204c26dc095))
* **cmdline_utils:** add help option  ([#87](https://github.com/tome-github/b2v-libsm/issues/87)) ([948236b](https://github.com/tome-github/b2v-libsm/commit/948236b2caa02258e8e3534364a7a560e46135d0))


### Bug Fixes

* **get path prediction:** check for null input ([a55224d](https://github.com/tome-github/b2v-libsm/commit/a55224d60a770a8870c96758805f660154a9b8b9))

### [5.1.1](https://github.com/tome-github/b2v-libsm/compare/v5.1.0...v5.1.1) (2021-03-24)


### Bug Fixes

* update missing and incorrect defines for J2735 and J2945 ([c53966f](https://github.com/tome-github/b2v-libsm/commit/c53966f27972891ed3f6687573469c90e295fb88))

## [5.1.0](https://github.com/tome-github/b2v-libsm/compare/v5.0.0...v5.1.0) (2021-03-18)


### Features

* Add J2945/9 defines and add to includes in libsm.h ([dc28d62](https://github.com/tome-github/b2v-libsm/commit/dc28d6294758c7e9da1d93f58a95a8edaf081e9e))
* **mf2CSV:** update to include path prediction ([#75](https://github.com/tome-github/b2v-libsm/issues/75)) ([27d6bb7](https://github.com/tome-github/b2v-libsm/commit/27d6bb7dba23eb97277a783f53212c26b71d6fdd)), and update with 4 way acceleration ([#82](https://github.com/tome-github/b2v-libsm/issues/82)) ([0c5d4cf](https://github.com/tome-github/b2v-libsm/commit/0c5d4cf3dfcb104d3665a3f9b05fb99971bc9a18))


### Bug Fixes

* **decoding:** asn_decode consuming 0 bytes is a failure ([#72](https://github.com/tome-github/b2v-libsm/issues/72)) ([a99709e](https://github.com/tome-github/b2v-libsm/commit/a99709ef25a704daa163268a849cccf4280a8c8d))
* **mf2CSV:** handle PSM elevation if non existent ([#78](https://github.com/tome-github/b2v-libsm/issues/78)) ([9cb0687](https://github.com/tome-github/b2v-libsm/commit/9cb0687d1b7855698e502e8a67b435b0daf0ef30))

## [5.0.0](https://github.com/tome-github/b2v-libsm/compare/v4.4.0...v5.0.0) (2021-02-18)


### ⚠ BREAKING CHANGES

* Makefile.gcc.include now defines LIBSM_SRCS,
LIBSM_CFLAGS, and LIBSM_INC_DIRS instead of SRC_FILES and CFLAGS
* asn1c changed and some j2735 files like
der_encoder.c are no longer present.
If you were including all the files directly this is a breaking change.
If you only used the compiled libraries, nothing should change.

ticket: BV-429

### Features

* script to generate the j2735 from ASN & update ASN1C ([#48](https://github.com/tome-github/b2v-libsm/issues/48)) ([e7e3ed1](https://github.com/tome-github/b2v-libsm/commit/e7e3ed19cbfdc82c137578ac859700fcb296eaf8))
* **mf2csv:** handle unavailable values properly ([6ca5490](https://github.com/tome-github/b2v-libsm/commit/6ca5490935116319274302a9d0ee1135914aa609))


### Bug Fixes

* **asn1c:** fix asn1c long issue (again) ([#69](https://github.com/tome-github/b2v-libsm/issues/69)) ([6091165](https://github.com/tome-github/b2v-libsm/commit/6091165c198f4f4d2dd82274975c3236aa7acd00))


### build

* Makefile.gcc.include is now source of truth for building ([#64](https://github.com/tome-github/b2v-libsm/issues/64)) ([ede224e](https://github.com/tome-github/b2v-libsm/commit/ede224ecee5039b85455af368cfc279f4d475686))

## [4.4.0](https://github.com/tome-github/b2v-libsm/compare/v4.3.0...v4.4.0) (2021-01-08)


### Features

* range coercion defines for j2735 DEs ([#58](https://github.com/tome-github/b2v-libsm/issues/58)) ([c17c0d9](https://github.com/tome-github/b2v-libsm/commit/c17c0d9d32f7d170c8480b84764315d0d61ae43d))

## [4.3.0](https://github.com/tome-github/b2v-libsm/compare/v4.2.2...v4.3.0) (2020-12-17)


### Features

* add defines for min/max/unknown ([#51](https://github.com/tome-github/b2v-libsm/issues/51)) ([80df8e8](https://github.com/tome-github/b2v-libsm/commit/80df8e84526a8e7f7f6213e620b2ee02f3e0c0a8))
* add libsm_alloc_init_AccelerationSet4Way ([#56](https://github.com/tome-github/b2v-libsm/issues/56)) ([5dbfa85](https://github.com/tome-github/b2v-libsm/commit/5dbfa85d0a1eb69b9e83e112ca425b61f7aa0237))
* functionality for adding pathHistory to PSM and BSM ([cf4ecd5](https://github.com/tome-github/b2v-libsm/commit/cf4ecd5ba36d0d132dfa9fbac1ed8518e37eab30))


### Bug Fixes

* **cmdline utils:** help displays all possible entries ([b24e8f8](https://github.com/tome-github/b2v-libsm/commit/b24e8f8310ab445cec9855d2fbd4eb00d56f07e0))
* **encode:** return correct error when len=0 ([#47](https://github.com/tome-github/b2v-libsm/issues/47)) ([37c960e](https://github.com/tome-github/b2v-libsm/commit/37c960e513581c0b982dfc201a1e46732a480429))
* **j2735 defines:** remove semicolons from defines ([#54](https://github.com/tome-github/b2v-libsm/issues/54)) ([a81066c](https://github.com/tome-github/b2v-libsm/commit/a81066cfe6de3a49dfabe0ab9f7f7ebf0d1a0c5a))

### [4.2.2](https://github.com/tome-github/b2v-libsm/compare/v4.2.1...v4.2.2) (2020-09-03)


### Bug Fixes

* add argument checks to mf2CSV ([3de2aff](https://github.com/tome-github/b2v-libsm/commit/3de2affa01d138a6bcedbdd9d38d4b075708cb81))
* **build warnings:** properly enable build warnings as errors ([#40](https://github.com/tome-github/b2v-libsm/issues/40)) ([ee1021c](https://github.com/tome-github/b2v-libsm/commit/ee1021cad2bb72be9a4f30ecd08c1aa38161d2a7))
* **libsm:** remove prints to stdout/stderr ([#36](https://github.com/tome-github/b2v-libsm/issues/36)) ([9096d17](https://github.com/tome-github/b2v-libsm/commit/9096d1701e4c111c24ec2cdaa81e66402ca89b9c))

### [4.2.1](https://github.com/tome-github/b2v-libsm/compare/v4.2.0...v4.2.1) (2020-06-01)


### Bug Fixes

* **libsm:** fix range check in PSM type to string func ([#33](https://github.com/tome-github/b2v-libsm/issues/33)) ([89f5e39](https://github.com/tome-github/b2v-libsm/commit/89f5e39d14ff5c9c8f58ded9721473e4c1beb0c4))

## [4.2.0](https://github.com/tome-github/b2v-libsm/compare/v4.1.0...v4.2.0) (2020-05-28)


### Features

* **libsm:** add path prediction ([#26](https://github.com/tome-github/b2v-libsm/issues/26)) ([250fadc](https://github.com/tome-github/b2v-libsm/commit/250fadced73ae1d7f4445f6b5d492ec6c566e705))


### Bug Fixes

* **asn1c output:** change cast from long to intmax_t  ([#28](https://github.com/tome-github/b2v-libsm/issues/28)) ([1d29f80](https://github.com/tome-github/b2v-libsm/commit/1d29f80d051ff4952144af8e34acf14fe8142066))

## [4.1.0](https://github.com/tome-github/b2v-libsm/compare/v4.0.0...v4.1.0) (2020-05-21)


### Features

* **versioning:** define VERSION_LIBSM string version ([#29](https://github.com/tome-github/b2v-libsm/issues/29)) ([273a8aa](https://github.com/tome-github/b2v-libsm/commit/273a8aa9d77a8eda2423392a8b07e202e33ad6fd))


### Bug Fixes

* update libsm library ([56374a1](https://github.com/tome-github/b2v-libsm/commit/56374a14a7a4678f46dc8852a51e133560de1bfb))
* **cmdline utils:** link against correct libsm archive ([14a6c52](https://github.com/tome-github/b2v-libsm/commit/14a6c52a37b242c24029da18f7679ffae1c224e8))

## [4.0.0](https://github.com/tome-github/b2v-libsm/compare/v3.3.0...v4.0.0) (2020-05-07)


### ⚠ BREAKING CHANGES

* Move output archives to build/libsm-$platform.a
Currently there is libsm-nordic.a and libsm-native.a

These changes were made in cfb63dd610953fe9ebe2c81f7d3ffaf22e294c5d
and later 3e5a4dd0da50aa634912ce1cc649ebe68c7760ea, but I forgot to call
out the breaking changes when writing the commit message.

### Features

* **cmdutils:** Added utility to convert BSM log to text in CSV file ([caf8beb](https://github.com/tome-github/b2v-libsm/commit/caf8beb2dcc03b7b8a04903b3a3439ef5baab31b))
* added ability to convert BSM log file input to text output in CSV file ([75887b2](https://github.com/tome-github/b2v-libsm/commit/75887b2dceb1db1695a9d808581e5a08ac3d21ad))
* added function to libsm to set the basic vehicle class in the BSM ([85a07de](https://github.com/tome-github/b2v-libsm/commit/85a07de63dc8c02e5ec05ed425164201f37fbd83))
* added function to return the vehicle type from the BSM ([3a3bbb2](https://github.com/tome-github/b2v-libsm/commit/3a3bbb20ad4c8fb945c0c296ba9ae6a18a5cd021))


### Bug Fixes

* remove SAE J2735 PDF that shouldn't be in this repo ([#14](https://github.com/tome-github/b2v-libsm/issues/14)) ([4c3011e](https://github.com/tome-github/b2v-libsm/commit/4c3011e01342a76201eec16db909c699885ea013))


* call out breaking change in cfb63dd ([38dfd1a](https://github.com/tome-github/b2v-libsm/commit/38dfd1a0c1600a76f2f0b1b9a4ccf6b4404500f8))

## [3.3.0](https://github.com/tome-github/b2v-libsm/compare/v3.2.0...v3.3.0) (2020-03-27)


### Features

* added new file for code owners ([33c4793](https://github.com/tome-github/b2v-libsm/commit/33c479371e460335b5c59296ca2ad64043162b43))

## [3.2.0](https://github.com/tome-github/b2v-libsm/compare/v3.1.1...v3.2.0) (2020-03-03)


### Features

* **unit tests:** Add cpputest and jenkinsfile and tests ([#6](https://github.com/tome-github/b2v-libsm/issues/6)) ([d9e0f8c](https://github.com/tome-github/b2v-libsm/commit/d9e0f8c2b7c4875598186a8d493cd26956bcadd1))


### Bug Fixes

* **bsm init:** set brake applied status to unavailable ([#3](https://github.com/tome-github/b2v-libsm/issues/3)) ([5ee461d](https://github.com/tome-github/b2v-libsm/commit/5ee461d596e745f51ff8965e337762d6451fec78))
* **bsm init:** set brake applied status to unavailable correctly ([#5](https://github.com/tome-github/b2v-libsm/issues/5)) ([6eab07e](https://github.com/tome-github/b2v-libsm/commit/6eab07ee275286f27695cc33af9ed8b614aff3eb))
* **versioning:** update lib/version.h on release ([#4](https://github.com/tome-github/b2v-libsm/issues/4)) ([46625d1](https://github.com/tome-github/b2v-libsm/commit/46625d17f2bf84239e6427a92f8d7b614e4b35de))

### [3.1.1](https://github.com/tome-github/b2v-libsm/compare/v3.1.0...v3.1.1) (2020-02-13)


### Bug Fixes

* **createPSMBSM:** demo callocing some optionals ([2d5524c](https://github.com/tome-github/b2v-libsm/commit/2d5524c920f61c388c4f09884f31bf85c820e79a))
* Replace D2V with B2V ([da7a954](https://github.com/tome-github/b2v-libsm/commit/da7a954dcd50c507dff191d5e8b4f54b7aa4b97c))

## [3.1.0](https://github.com/tome-github/b2v-libsm/compare/v3.0.0...v3.1.0) (2020-01-17)


### Features

* add BSM Part II example ([fb6dbb2](https://github.com/tome-github/b2v-libsm/commit/fb6dbb209dcf98deb5821b44d136006c7b44586c))
* **bsmPartII:** send vehicle type ([7d39818](https://github.com/tome-github/b2v-libsm/commit/7d39818daca9dcbeb98c4f8bcb49227ca41c15ad))
