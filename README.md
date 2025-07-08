# ArduinoContest
A repository for the IEEE SOLID STATE CIRCUITS SOCIETY™ Contest

## Materials
- Arduino Uno (or compatible)
- Breadboard
- Electret microphone module
- USB cable
- Jumper wires
- NMOS Transistor
- Vibration Coin Motor

## Wiring
<img src="wiring_diagram.jpg" alt="Wiring Diagram" width="400"/>

## Setup
Install required libraries:
- [fix_fft by Dimitrios P. Bouras](https://docs.arduino.cc/libraries/fix_fft/#Releases)

## Code 
...

## Usage 
1. Open ![main.ino](main.ino) in the Arduino IDE.
2. Connect your Arduino to your computer using a USB cable.
3. Verify the code, then click **Upload**.
4. Open the **Serial Monitor** (top right corner) to see audio input in real time.

## Testing
Speak or play sound near the microphone to check if values change in the `Serial Monitor`.
<img width="400" alt="SCR-20250628-spmj" src="https://github.com/user-attachments/assets/5b9714d4-04e3-480a-a8a4-749c56c4627b" />

## FFT:
1. Why is FFT Important For Our Project?
- The reason we are using FFT is because it allows us to identify the frequency components of a signal, which is important for applications like audio processing, especially because our project involves taking a music audio input and converting it into vibrations.
- It also allows us to perform real-time signal processing on our microcontroller by reducing the complexity of analyzing audio signals.
    - This resolves our problem about having limited RAM in our arduino, so no matter the length of the song, it will continously create vibrations based on the beats of the song.

2. Why Use Fixed-Point FFT For Our Project?
- Using floats is not always the most efficient way on an Arduino because they are too powerful.
- They represent big and small numbers accurately, but FFT doesn’t need that much precision or range, and the output values of FFT do not vary wildly.
- They change in predictable ways.
- Floats use a lot of memory, and Arduino has limited RAM.
- Arduino Analog-to-Digital Converter (ADC) gives integer values, and since FFT can use integers, there is no need to convert values to floats.
- The solution is to use a fixed-point implementation, which uses integers to represent decimal numbers, and it uses less memory since float math is avoided.
- This meets the needs for FFT and keeps the performance high on devices like Arduino.
  
- Efficiency:
    - Our code uses in-place FFT, which is more memory-efficient and ideal for our Arduino's limited RAM. We are also leveraging FFT symmetry by only analyzing the first 64 frequency bins.
    - For our project, we are specifically using Fixed FFT-8 because it offers fast performance and low memory usage, which is ideal for the Arduino’s limited processing power and RAM. While it sacrifices some precision, Fixed FFT-8 is sufficient for detecting bass frequencies and generating vibration feedback in sync with the music in real time.

3. Frequency Filtering Based on FFT Bins & How We Used this: 
- FFT splits the audio signal into “bins”, which represent ranges of frequencies. Each bin corresponds to a specific frequency band in the signal. For example, if a 256-point FFT is used at a sampling rate of 1024 Hz, then: 1024 / 256 = 4 Hz per bin This means:
  - Bin 0 = 0 Hz
  - Bin 1 = 4 Hz
  - Bin 2 = 8 Hz etc.

- Important note: Bin 0 is the DC component (0 Hz) and only bins up to bin 128 (half of 256) are used for analysis since the second half mirrors the first. Based on how strong the signal is in each bin, you can categorize it into bass, mid, or treble, or make more detailed frequency bands depending on your application.

- Our code only focuses on the first 64 bins or positive frequencies only and FFT symmtery allows us to ignore bins 64 to 127 (these are mirror duplicates). In our application we are extracting bass energy from Bin 3 which 225 Hz and Bin 4 which is 300 Hz. These are in the lower-mid/bass range and then we are turning that into PMW output to a motor. Essentially we are driving a motor based on bass intensity. We selected these bins because they correspond to frequency ranges commonly found in most songs. Due to the limited number of motors we could use in our project, we prioritized frequencies that would be reliably present in a wide range of music content.

4. Sources (MLA) for FFT Explanation:
- Bernoulli, Daniel. "Application of the FFT." Mathematics Stack Exchange, 12 Feb. 2017, https://math.stackexchange.com/q/2139840. Accessed 5 July 2025.
- Dewesoft. "Guide to FFT Analysis (Fast Fourier Transform)." Dewesoft, https://dewesoft.com/blog/guide-to-fft-analysis. Accessed 5 July 2025.
- Klafyvel. "FFT on Arduino." Klafyvel, 13 Nov. 2021, https://klafyvel.me/blog/articles/fft-arduino/. Accessed 5 July 2025.
- Marcus Müller. "What Is a Frequency Bin?" DSP Stack Exchange, 6 May 2016, https://dsp.stackexchange.com/questions/26927/what-is-a-frequency-bin. Accessed 5 July 2025.
- Singh, Rahul. "Breaking Down Confusions over Fast Fourier Transform (FFT)." Medium, Analytics Vidhya, 19 May 2021, https://medium.com/analytics-vidhya/breaking-down-confusions-over-fast-fourier-transform-fft-1561a029b1ab. Accessed 5 July 2025.
- Tektronix. "What Is FFT (Fast Fourier Transform) Math Function? Why Is It Useful on an Oscilloscope?" Tektronix Support FAQs, https://www.tek.com/en/support/faqs/what-fft-fast-fourier-transform-math-function-oscilloscope-useful. Accessed 5 July 2025.
