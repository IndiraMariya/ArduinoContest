#include <fix_fft.h>

char im[128], data[128];
int val;
const int motorPin = 9;
unsigned long lastMotorUpdate = 0;
const unsigned long motorInterval = 2000;  // 2 seconds

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  if (millis() - lastMotorUpdate >= motorInterval) {
    // Sample 128 audio points
    for (int i = 0; i < 128; i++) {
      val = analogRead(A0);
      data[i] = val / 4 - 128;
      im[i] = 0;
    }

    // Run FFT
    fix_fft(data, im, 7, 0);

    int peakMidBass = 0;

    for (int i = 1; i < 64; i++) {
      int mag = sqrt(data[i] * data[i] + im[i] * im[i]);

      // Boost and clean signal
      mag = (mag - 1) * 20;
      mag = max(0, mag);
      mag = constrain(mag, 0, 255);

      // Focus on mid-bass range: bins 10 to 20
      if (i >= 10 && i <= 20 && mag > peakMidBass) {
        peakMidBass = mag;
      }

      // Print all magnitudes
      Serial.print(mag);
      Serial.print(i < 63 ? ',' : '\n');
    }

    // Optional: ensure visible motor kick
    if (peakMidBass > 20) {
      peakMidBass = max(peakMidBass, 100);
    }

    Serial.print("Mid-Bass (bins 10-20): ");
    Serial.println(peakMidBass);

    // Pulse motor
    analogWrite(motorPin, peakMidBass);
    delay(100);
    analogWrite(motorPin, 0);

    lastMotorUpdate = millis();
  }
}
