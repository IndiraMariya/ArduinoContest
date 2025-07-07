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

## Detailed Breakdown on FFT & Why it's Important to Our Project
1. General Idea & Differences:
  The Fourier Transform is a mathematical method used to convert signals from the time domain into the frequency domain, showing how much of each frequency exists in a signal. It breaks down complex waveforms into a combination of simpler sine and cosine waves. However, the Fourier Transform is computationally intensive and not suitable for direct use on microcontrollers without optimization. This is where the Fast Fourier Transform (FFT), which is an algorithm that efficiently computes the Fourier Transform, making it practical for real-time processing on devices like computers and microcontrollers. On an oscilloscope, FFT is used as a math function to convert time-domain signals into a frequency-domain representation. Basically, in a time-domain frame, the signal characteristics appear as one waveform, which is the sum of all the frequency components. In contrast, the frequency domain shows each frequency component independently, making it easier to analyze individual frequency bands in a signal.

  This library is generally used for music projects on Arduino to detect specific frequencies and analyze audio signals. There are different kinds of FFT, like fixed-point, discrete, and regular FFT. FFT is essentially a super-fast algorithm that computes the Discrete Fourier Transform (DFT) or Fourier Transform (FT) (FFT is just a fast way of computing DFT). Fixed-point FFT does not use floating-point numbers and is more efficient on Arduino for this reason. FFT can be used to measure sound pressure in noisy environments and vibrations.

2. FFT Variants Being Compared:
- ExactFFT is the highly accurate version of FFT, but it is extremely slow and it uses floating point, double-precision etc. Additionally the memory usage is very high so it is efficient for microcontroller applications. 
- Fixed8FFT is the Fixed-point FFT with 8-bit precision (it is really fast but it is not as accurate) and so for arduino use it if you need max speed and low memory usage. 
- Fixed16FFT is the Fixed-point FFT with 16-bit precision and (there is a balance between speed and accuracy) and so for arduino use it for better accuracy but not as good speed.
- Float FFT uses single-precision floating point and is not as accurate as exact FFT but still accurate but it is faster than Exact FFT and memory usage is kind of high. It also uses more RAM than fixed-point precision. It is good for mid-range microcontrollers that can handle/support floats.
- Approx FFT is not that precise and it is faster than Float FFT and memory usage is less than Float FFT. It also needs a microcontroller that can handle floats.

From this it is safe to use Fixed8FFT or Fixed16FFT for arduino due to limited RAM based on the user’s needs.

3. Efficiency:
  There are two ways to implement the FFT library: out-of-place and in-place. Out-of-place stores the original data and then puts the new, changed data in a separate place. In-place does not store the original data — it overwrites it with the new, processed data. In-place is more efficient and uses less memory. Running in-place allows the user to handle over 256 points on an Arduino Uno. Also, the FFT output has symmetry that allows only the first half to be stored, because the second half is the complex conjugate mirror of the first half. The first half represents the positive frequency components, and the second half (which would be negative frequencies) can be derived from the first, so it saves space.

4. Frequency Filtering Based on FFT Bins:
FFT splits the audio signal into “bins”, which represent ranges of frequencies. Each bin corresponds to a specific frequency band in the signal. For example, if a 256-point FFT is used at a sampling rate of 1024 Hz, then: 1024 / 256 = 4 Hz per bin This means:
Bin 0 = 0 Hz
Bin 1 = 4 Hz
Bin 2 = 8 Hz etc.

Important note: Bin 0 is the DC component (0 Hz) and only bins up to bin 128 (half of 256) are used for analysis since the second half mirrors the first. Based on how strong the signal is in each bin, you can categorize it into bass, mid, or treble, or make more detailed frequency bands depending on your application.

5. Why is FFT Important For Our Project?
The reason we are using FFT is because it allows us to identify the frequency components of a signal, which is important for applications like audio processing, especially because our project involves taking a music audio input and converting it into vibrations. It also allows us to perform real-time signal processing on our microcontroller by reducing the complexity of analyzing audio signals. This resolves our problem about having limited RAM in our arduino, so no matter the length of the song, it will continously create vibrations based on the beats of the song.

For our project, we are specifically using Fixed FFT-8 because it offers fast performance and low memory usage, which is ideal for the Arduino’s limited processing power and RAM. While it sacrifices some precision, Fixed FFT-8 is sufficient for detecting bass frequencies and generating vibration feedback in sync with the music in real time.

6. Why Use Fixed-Point FFT For Our Project?
  Using floats is not always the most efficient way on an Arduino because they are too powerful. They represent big and small numbers accurately, but FFT doesn’t need that much precision or range, and the output values of FFT do not vary wildly — they change in predictable ways.

  Additionally, floats use a lot of memory, and Arduino has limited RAM. Also, the Arduino Analog-to-Digital Converter (ADC) gives integer values, and since FFT can use integers, there is no need to convert values to floats. The solution is to use a fixed-point implementation, which uses integers to represent decimal numbers, and it uses less memory since float math is avoided. This meets the needs for FFT and keeps the performance high on devices like Arduino.
