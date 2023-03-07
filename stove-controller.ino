#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

static unsigned long startTime = 0;
static unsigned long stopTime = 0;
static double elapsedTimeInMinutes = 0;
static double totalTimeInMinutes = 0;
static bool isSaved = false;
static unsigned long totalLight = 0;
static int maxLight = 0;
static byte displayTotalLight = 0;

const int BUTTON_PIN = 2;  // the number of the pushbutton pin


void displayTime(String time) {            // clear display
  lcd.setCursor(0, 0);         // move cursor to first char in first line
  lcd.print(time);        // print message at (0, 0)
  lcd.print(" min");
}

void displayLight(String lightLevel) {
  lcd.setCursor(0, 1); // move cursor to first char in second line
  if (displayTotalLight == 0) {
    lcd.print("Total: ");
    lcd.print(totalLight);
    displayTotalLight = 1;
  } else if (displayTotalLight == 1){
    lcd.print("Act. light: ");
    lcd.print(lightLevel);
    displayTotalLight = 2;  
  } else if (displayTotalLight == 2) {
    lcd.print("Max light: ");
    lcd.print(maxLight);
    displayTotalLight = 0;  
  }
}

void setup() {
  startTime = millis();
  // Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  lcd.init(); //initialize the lcd
  pinMode(BUTTON_PIN, INPUT); // initialize the pushbutton pin as an input
}

void loop() {
  lcd.clear();
  int buttonState = digitalRead(BUTTON_PIN); // read the state of the pushbutton value
  if (buttonState == HIGH) { // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    lcd.backlight();
  } else {
    lcd.noBacklight();
  }
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
    totalLight += analogValue;   
    setMaxValue(analogValue);
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

void setMaxValue(int value) {
  if(maxLight < value) {
    maxLight = value;
  }
}
