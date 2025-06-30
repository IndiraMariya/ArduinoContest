#include <fix_fft.h>

char im[128], data[128];
int val;
const int motorPin = 9;  // Must be a PWM-capable pin

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 128; i++) {
    val = analogRead(A0); 
    data[i] = val / 4 - 128;
    im[i] = 0;
  }

  fix_fft(data, im, 7, 0);

  int bassMag = 0;

  for (int i = 1; i < 64; i++) {
    int mag = sqrt(data[i] * data[i] + im[i] * im[i]);
    mag = mag * 2;
    mag = constrain(mag, 0, 255);   // Avoid overflow
    
    if (i == 3 || i == 4) {
      bassMag += mag;
    }

    // Optional: show all frequency bin magnitudes
    Serial.print(mag);
    Serial.print(i < 63 ? ',' : '\n');
  }

  // bassMag = constrain(bassMag / 2, 0, 255);  // Average of bin 3 and 4
  if (bassMag < 20) bassMag = 0;  // Ignore weak signals
  else bassMag = constrain(bassMag * 2.0 , 0, 255);  // Boost

  // Print motor control value
  Serial.print("Motor PWM (BassMag): ");
  Serial.println(bassMag);

  analogWrite(motorPin, bassMag);

  delay(100);
}
