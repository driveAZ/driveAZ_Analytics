
### 1. Create and activate a virtual environment

```bash
python3 -m venv venv
source venv/bin/activate
```

### 2. Install Python dependencies

```bash
pip install -r requirements.txt
```

---

### 3. Deactivate when done

```bash
deactivate
```


# b2v-libsm

This repository builds the `libsm` binary tools for encoding and decoding SAE J2735 messages (e.g., TIM, BSM, PSM) using C++.

##  Requirements

- Ubuntu 20.04 or later
- CMake ≥ 3.21
- GCC (C/C++) ≥ 9.4

###  Dependencies

```bash
sudo apt update
sudo apt install -y build-essential cmake g++ libboost-all-dev
````

---

##  Build Instructions

1. **Clone or download the repository** and move to the `b2v-libsm` directory:

```bash
cd ~/driveAZ_Analytics/libsm/b2v-libsm
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

---

##  Output

After successful compilation, the binaries will be located at:

```
build/bin/
```

Typical output binaries include:

* `createTIM`
* `decodeToJER`
* `bsmPartII`

---

##  Verify

To test a binary:

```bash
./bin/decodeToJER -i <hex_input_file>
```

Replace `<hex_input_file>` with your input file or use echo-piped hex input.

---

##  Notes

* If you want to build the test suite (`cpputest`), ensure CMake version ≥ 3.5 and set:

```bash
cmake .. -DLIBSM_TESTS=ON
```

* You can copy the binaries into your system path or Docker container after building.

---

##  License

