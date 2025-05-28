// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String songTitle = "                Landslide - Fleetwood Mac   ";
int scrollIndex = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Now Playing:");
}

void loop() {
  String toDisplay = songTitle.substring(scrollIndex, scrollIndex + 16);

  lcd.setCursor(0, 1);
  lcd.print(toDisplay);

  scrollIndex++;
  if (scrollIndex > songTitle.length() - 16) {
    scrollIndex = 0;  // Restart scroll
  }

  delay(200); // Adjust for scroll speed
}
