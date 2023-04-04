#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

static unsigned long startTime = 0;
static unsigned long stopTime = 0;
static unsigned long backlightStart = 0;
static double elapsedTimeInMinutes = 0;
static double totalTimeInMinutes = 0;
static bool isSaved = false;
static unsigned long totalLight = 0;
static int maxLight = 0;
static byte displayTotalLight = 0;

const int BUTTON_PIN = 2;  // the number of the pushbutton pin
const int STOVE_PIN = 3;
const int DRIVER_1_PIN = 13;
const int PILOT_DRIVER_1_PIN = 12;
const int DRIVER_2_PIN = 11;
const int PILOT_DRIVER_2_PIN = 10;


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
  Serial.begin(9600); // initialize serial communication at 9600 bits per second:
  lcd.init(); //initialize the lcd
  pinMode(BUTTON_PIN, INPUT); // initialize the pushbutton pin as an input
  pinMode(DRIVER_1_PIN, INPUT);
  pinMode(PILOT_DRIVER_1_PIN, OUTPUT);
  pinMode(STOVE_PIN, OUTPUT);
  pinMode(DRIVER_2_PIN, INPUT);
  pinMode(PILOT_DRIVER_2_PIN, OUTPUT);
  digitalWrite(STOVE_PIN, LOW);
  digitalWrite(PILOT_DRIVER_1_PIN, LOW);
  digitalWrite(PILOT_DRIVER_2_PIN, LOW);
}

void loop() {
  bool driver1Run, driver2Run;
  driver1Run = readFromDriver(DRIVER_1_PIN, PILOT_DRIVER_1_PIN);
  driver2Run = readFromDriver(DRIVER_2_PIN, PILOT_DRIVER_2_PIN);
  runStove(driver1Run || driver2Run);

  lcd.clear();
  switchBackLight();
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

void switchBackLight() {
  int buttonState = digitalRead(BUTTON_PIN); // read the state of the pushbutton value 
  if (buttonState == HIGH) { // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    backlightStart = millis();
    lcd.backlight();
  } else {
    unsigned long elapsedBacklightTime = millis() - backlightStart;
    if (elapsedBacklightTime > 10000) {
      lcd.noBacklight();
    }
  }
}

bool readFromDriver(int driverNumberPin, int pilotNumberPin) {
  int isTurnOn = digitalRead(driverNumberPin);
  Serial.print(driverNumberPin);
  Serial.print(" = ");
  Serial.println(isTurnOn);
  if(isTurnOn == HIGH) {
    digitalWrite(pilotNumberPin, HIGH);
    return true;
  } else {
    digitalWrite(pilotNumberPin, LOW);
    return false;
  }
}

void runStove(bool ifRun) {
  if(ifRun)
    digitalWrite(STOVE_PIN, HIGH);
  else
    digitalWrite(STOVE_PIN, LOW);
}
