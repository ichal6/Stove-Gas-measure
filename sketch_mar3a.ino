#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

static unsigned long startTime = 0;
static unsigned long stopTime = 0;;


void displayTime(String time) {
    lcd.clear();                 // clear display
    lcd.setCursor(0, 0);         // move cursor to   (0, 0)
    lcd.print(time);        // print message at (0, 0)
    lcd.print(" min");
}

void displayLight(String lightLevel) {
    lcd.setCursor(0, 1);         // move cursor to   (0, 1)
    lcd.print(lightLevel);        // print message at (0, 1)
    lcd.print(" light");
}

void setup() {
  startTime = millis();
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
  stopTime = millis();  
  unsigned long elapsedTime = stopTime - startTime;

  double elapsedTimeInMinutes = (double)elapsedTime/60000;

  Serial.println(elapsedTimeInMinutes, 4);
  displayTime(String(elapsedTimeInMinutes, 4));
  displayLight(String(analogValue));

  delay(2000);
}
