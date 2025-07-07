---

##  Build libsm

You can build **libsm** locally or using Docker.

---

### Build locally (recommended for development)

#### Install dependencies

On **Ubuntu**:

```bash
sudo apt-get update && sudo apt-get install -y \
    build-essential cmake git g++ protobuf-compiler \
    libprotobuf-dev libboost-all-dev


### Build the library

cd libsm/b2v-libsm
mkdir build
cd build
cmake ..
make -j$(nproc)

### Outputs:

    Binaries in build/bin

    Libraries in build/lib
