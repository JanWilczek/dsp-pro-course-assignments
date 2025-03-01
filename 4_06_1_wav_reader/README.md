# 🎶 WAV Reader 🎼

```bash
cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON # optionally: -G "Ninja"
cmake --build build
```

## Note on types used

`uint8_t` type defined in the _<cstdint>_ header is exactly 8 bits (1 byte) or the program using it does not compile. Period. In this way, we avoid using `unsigned char` which has **at least** 8 bits but may have more if the architecture or the C++ implementation does not support 8-bit variables.

