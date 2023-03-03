#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void display() {
    Serial.print("Start: ");
    lcd.clear();                 // clear display
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print("Arduino");        // print message at (0, 0)
    lcd.setCursor(2, 1);         // move cursor to   (2, 1)
    lcd.print("GetStarted.com"); // print message at (2, 1)
    delay(2000);                 // display the above for two seconds

    lcd.clear();                  // clear display
    lcd.setCursor(3, 0);          // move cursor to   (3, 0)
    lcd.print("DIYables");        // print message at (3, 0)
    lcd.setCursor(0, 1);          // move cursor to   (0, 1)
    lcd.print("www.diyables.io"); // print message at (0, 1)
    delay(2000);                  // display the above for two seconds
}

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
}

void loop() {
  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(A0);

  Serial.print("Analog reading: ");
  Serial.print(analogValue);   // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (analogValue < 10) {
    Serial.println(" - Dark");
  } else if (analogValue < 200) {
    Serial.println(" - Dim");
  } else if (analogValue < 500) {
    Serial.println(" - Light");
  } else if (analogValue < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }

  delay(500);
  display();
}
