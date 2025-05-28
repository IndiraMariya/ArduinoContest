#include <fix_fft.h>

char im[128], data[128];
int val;

void setup() {
  Serial.begin(115200);
  analogReference(DEFAULT);
}

void loop() {
  for (int i = 0; i < 128; i++) {
    val = analogRead(A0); 
    data[i] = val / 4 - 128;
    im[i] = 0;
  }

  fix_fft(data, im, 7, 0);

  for (int i = 1; i < 64; i++) {
    int mag = sqrt(data[i] * data[i] + im[i] * im[i]);
    mag = mag * 2;                  // BOOST magnitude (can try 3, 4, etc.)
    mag = constrain(mag, 0, 255);   // Avoid overflow
    Serial.print(mag);
    Serial.print(i < 63 ? ',' : '\n');
  }

  delay(30);
}
