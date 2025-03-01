#include "WavReader.hpp"
#include <iostream>
#include <ranges>

int main(int argc, char* argv[]) {
  std::cout << "Number of arguments: " << argc << std::endl;
  std::cout << "Arguments:" << std::endl;
  for (auto i : std::views::iota(0, argc)) {
    std::cout << "#" << i << ": " << argv[i] << std::endl;
  }
  std::cout << std::endl;

  if (argc != 2) {
    std::cout << "read_wav program to read and analyze a WAV file."
              << std::endl;
    std::cout << "Usage:\n read_wav path/to/wav" << std::endl;
  }

  wav::analyzeWav(argv[1]);

  return 0;
}
