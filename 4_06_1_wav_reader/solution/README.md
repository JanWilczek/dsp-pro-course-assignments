# 🎶 4.06. Assignment Solution: WAV Reader 🎼

## Prerequisites to install

- CMake
- C++ compiler

## Build instructions

```bash
cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON # optionally: -G "Ninja"
cmake --build build
```

## Usage

To analyze and convert to binary a file (e.g., 1 kHz sine), run the following command using the generated executable:

```bash
build/read_wav data/1_channel_1_second_1kHzsine_16bitPCM@44100Hz.wav
```

You can compare this program's output to the output of the program you coded yourself.

## Note on types used

`uint8_t` type defined in the _<cstdint>_ header is exactly 8 bits (1 byte) or the program using it does not compile. Period. In this way, we avoid using `unsigned char` which has **at least** 8 bits but may have more if the architecture or the C++ implementation does not support 8-bit variables.

