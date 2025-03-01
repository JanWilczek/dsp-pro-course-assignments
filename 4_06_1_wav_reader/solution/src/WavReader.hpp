#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include <functional>
#include <array>

namespace wav {
void analyzeWav(const std::string& pathToWav);

class WavReader {
public:
  WavReader(std::filesystem::path pathToWav);
  void read();
  void printInfo();
  void printDataAsHex();
  void printDataAsAscii();
  void saveAsBinaryFile(const std::filesystem::path& pathToBinary);

private:
  struct WavHeader {
    static_assert(sizeof(char) == sizeof(uint8_t));

    /** "RIFF" in ASCII */
    std::array<char, 4> chunkId;

    /** size of the entire file in bytes minus the size of
     * this and the previous field */
    uint32_t chunkSize;

    /** "WAVE" in ASCII */
    std::array<char, 4> format;

    /** "fmt " in ASCII (note the space) */
    std::array<char, 4> subchunk1Id;

    /** size of the format subchunk minus the size of
     * this and the previous field; 16 for PCM samples */
    uint32_t subchunk1Size;

    /** 1 for PCM samples. Other values indicate
     * some form of compression */
    uint16_t audioFormat;

    /** number of channels, 1 for mono, 2 for stereo, etc. */
    uint16_t channelCount;

    uint32_t sampleRate;

    /** sampleRate * channelCount * bitsPerSample/8 */
    uint32_t byteRate;

    /** channelCount * bitsPerSample/8 */
    uint16_t blockAlign;

    uint16_t bitsPerSample;
  };

  struct DataHeader {
    /** "data" in ASCII */
    std::array<char, 4> subchunk2Id;

    /** size of the samples in bytes */
    uint32_t subchunk2Size;
  };

  void printUsing(std::function<void(uint8_t)> printFunction);

  std::filesystem::path pathToWav_;
  std::uintmax_t fileSize_;
  WavHeader header_{};
  DataHeader dataHeader_{};
  std::size_t samplesCount_;
  std::vector<float> samples_;
};
}  // namespace wav
