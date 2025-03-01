#include "WavReader.hpp"
#include <cctype>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <format>
#include <string_view>

namespace wav {
namespace {
float convertSample(uint32_t sample, uint16_t bitsPerSample) {
  if (bitsPerSample == 8u) {
    // sample [1, 255]
    constexpr auto INT_8_MAX =
        static_cast<float>(std::numeric_limits<int8_t>::max());
    constexpr auto LPCM_VALUE_OF_0_SAMPLE = INT_8_MAX + 1.f;

    // signedSample [-127, 127]
    const auto signedSample =
        static_cast<float>(sample) - LPCM_VALUE_OF_0_SAMPLE;

    // [-1, 1]
    return signedSample / INT_8_MAX;
  } else if (bitsPerSample == 16u) {
    // we assume little-endian byte order on the host machine

    // [-32 767, 32 767]
    const auto* reinterpretedSample = reinterpret_cast<int16_t*>(&sample);

    // [-1, 1]
    return static_cast<float>(*reinterpretedSample) /
           std::numeric_limits<int16_t>::max();
  }
  return {};
}
}  // namespace

void analyzeWav(const std::string& pathToWav) {
  std::filesystem::path path{pathToWav};
  WavReader wavReader{path};
  wavReader.read();
  wavReader.printInfo();
  wavReader.printDataAsHex();
  wavReader.printDataAsAscii();
  const auto binaryOutputPath = path.parent_path().parent_path() / "output" /
                                (path.stem().string() + ".bin");
  wavReader.saveAsBinaryFile(binaryOutputPath);
}

WavReader::WavReader(std::filesystem::path pathToWav)
    : pathToWav_{std::move(pathToWav)} {}

void WavReader::read() {
  std::ifstream file{pathToWav_, std::ios::binary};

  if (!file) {
    throw std::runtime_error{"failed to open file"};
  }

  fileSize_ = std::filesystem::file_size(pathToWav_);

  file.read(header_.chunkId.data(), header_.chunkId.size());

  auto readField = [&](auto& field) {
    // This is the recommended way to read bytes from a file
    // in the C++ standard. Source:
    // https://en.cppreference.com/w/cpp/io/basic_istream/read
    file.read(reinterpret_cast<char*>(&field), sizeof(field));
  };
  readField(header_.chunkSize);

  file.read(header_.format.data(), header_.format.size());
  file.read(header_.subchunk1Id.data(), header_.subchunk1Id.size());

  readField(header_.subchunk1Size);
  readField(header_.audioFormat);
  readField(header_.channelCount);
  readField(header_.sampleRate);
  readField(header_.byteRate);
  readField(header_.blockAlign);
  readField(header_.bitsPerSample);

  file.read(dataHeader_.subchunk2Id.data(), dataHeader_.subchunk2Id.size());
  readField(dataHeader_.subchunk2Size);

  if (header_.blockAlign == 0u) {
    throw std::runtime_error{
        "block align is 0: cannot calculate frame count (invalid file)"};
  }

  samplesCount_ = dataHeader_.subchunk2Size / header_.blockAlign;
  samples_.reserve(samplesCount_);

  const auto sampleSize = header_.bitsPerSample / 8u;
  for (auto i = 0u; i < samplesCount_; ++i) {
    uint32_t sample = 0u;
    file.read(reinterpret_cast<char*>(&sample), sampleSize);

    // convert sample
    samples_.push_back(convertSample(sample, header_.bitsPerSample));

    // skip remaining channels
    for (auto channel = 1u; channel < header_.channelCount; ++channel) {
      file.read(reinterpret_cast<char*>(&sample), sampleSize);
    }
  }
}

void WavReader::printInfo() {
  if (header_.sampleRate == 0u) {
    throw std::runtime_error{"read() the file before printing info"};
  }

  auto printLine = [](const auto& label, const auto& value) {
    std::cout << std::format("{:>20}: {}", label, value) << std::endl;
  };

  printLine("File size", fileSize_);
  printLine("Chunk ID",
            std::string_view{header_.chunkId.begin(), header_.chunkId.end()});
  printLine("Chunk size", header_.chunkSize);
  printLine("Format",
            std::string_view{header_.format.begin(), header_.format.end()});
  printLine("Subchunk 1 ID", std::string_view{header_.subchunk1Id.begin(),
                                              header_.subchunk1Id.end()});
  printLine("Subchunk 1 size", header_.subchunk1Size);
  printLine("Audio format", header_.audioFormat);
  printLine("Channel count", header_.channelCount);
  printLine("Sample rate", header_.sampleRate);
  printLine("Byte rate", header_.byteRate);
  printLine("Block align", header_.blockAlign);
  printLine("Bits per sample", header_.bitsPerSample);

  printLine("Subchunk 2 ID", std::string_view{dataHeader_.subchunk2Id.begin(),
                                              dataHeader_.subchunk2Id.end()});
  printLine("Subchunk 2 size", dataHeader_.subchunk2Size);
  printLine("Samples count (subchunk 2 size / block align)", samplesCount_);
}

void WavReader::printDataAsHex() {
  printUsing([](uint8_t readByte) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << std::uppercase
              << static_cast<int>(readByte) << " ";
  });
}

void WavReader::printUsing(std::function<void(uint8_t)> printFunction) {
  std::ifstream file{pathToWav_, std::ios::binary};

  if (!file) {
    throw std::runtime_error{"failed to open file"};
  }

  uint8_t readByte;
  int byteCount = 0;

  constexpr auto BYTES_PER_LINE = 10;

  while (file.read(reinterpret_cast<char*>(&readByte), sizeof(readByte))) {
    printFunction(readByte);
    if (++byteCount % BYTES_PER_LINE == 0) {
      std::cout << std::endl;
    }

    // don't print more than MAX_LINES lines
    constexpr auto MAX_LINES = 10;
    if (byteCount / BYTES_PER_LINE > MAX_LINES) {
      break;
    }
  }

  if (byteCount % BYTES_PER_LINE != 0) {
    std::cout << std::endl;
  }
}

void WavReader::printDataAsAscii() {
  printUsing([](uint8_t readByte) {
    const auto character = static_cast<char>(readByte);
    // is the character printable?
    if (std::isprint(character)) {
      std::cout << character;
    } else {
      // print a placeholder character for non-printable characters
      std::cout << ".";
    }
  });
}

void WavReader::saveAsBinaryFile(const std::filesystem::path& pathToBinary) {
  // Create directories
  std::filesystem::create_directories(pathToBinary.parent_path());

  std::ofstream file{pathToBinary,
                     std::ios::binary | std::ios::out | std::ios::trunc};

  if (!file) {
    throw std::runtime_error{"failed to open output file for writing"};
  }

  file.write(reinterpret_cast<char*>(samples_.data()),
             sizeof(decltype(samples_)::value_type) * samples_.size());
}
}  // namespace wav
