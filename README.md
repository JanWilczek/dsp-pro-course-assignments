<div align="center">

<img src="docs/WolfSoundLogo150px.webp"/>
<img src="docs/DspProCourseLogo.webp" width="256px" />

# DSP Pro Online Course Assignments with Solutions

</div>

Coding assignments (exercises) with solutions given to the members of the DSP Pro online course on learning digital audio signal processing for audio programming. For more information check https://www.wolfsoundacademy.com/dsp-pro

## Usage

To build and run the solutions locally, you need to first clone the repository.

```bash
# clone the repository if you haven't done so already (you only have to do it once)
git clone https://github.com/JanWilczek/dsp-pro-course-assignments.git
cd dsp-pro-course-assignments
```

To update the state of your local files with the latest version from GitHub, just run

```bash
git pull origin main:main
```

## 🏦 Structure

Each assignment has an example solution attached. The solutions are meant to be self-contained, i.e., you can treat them as separate projects unaware of the folder hierarchy.

To build an example project, just `cd` into the project directory in your terminal and proceed according to the _README.md_ file.

### 💡 Example

```bash
# cd into the soltion folder
cd 4_06_1_wav_reader/solution

# generate the C++ CMake project of the solution
cmake -Bbuild

# build the project
cmake --build build

# run the built executable
build/read_wav data/1_channel_1_second_1kHzsine_16bitPCM@44100Hz.wav
```

## 📝 License

This repository is [MIT-licensed](LICENSE.md) so that students outside of the course can participate and have fun solving the assignments!

