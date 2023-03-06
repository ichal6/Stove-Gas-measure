#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

static unsigned long startTime = 0;
static unsigned long stopTime = 0;
static double elapsedTimeInMinutes = 0;
static double totalTimeInMinutes = 0;
static bool isSaved = false;


void displayTime(String time) {            // clear display
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
  // Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
}

void loop() {
  lcd.clear();     
  // reads the input on analog pin A0 (value between 0 and 1023)
  int analogValue = analogRead(A0);

  // Serial.print("Analog reading: ");
  // Serial.println(analogValue);   // the raw analog reading
  displayLight(String(analogValue));

  if (analogValue <= 2) {
    if (isSaved == false) {
      isSaved = true;
      totalTimeInMinutes += elapsedTimeInMinutes;
      elapsedTimeInMinutes = 0;
    }
    startTime = millis();
  } else {
    isSaved = false;
    stopTime = millis();  
    unsigned long elapsedTime = stopTime - startTime;
    elapsedTimeInMinutes = (double)elapsedTime/60000;
  }

  // Serial.print("Time in min: ");
  // Serial.println(elapsedTimeInMinutes, 4);
  displayTime(String(elapsedTimeInMinutes + totalTimeInMinutes, 2));  

  delay(2000); // refresh in two seconds
}
