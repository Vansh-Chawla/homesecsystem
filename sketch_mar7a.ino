#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD display (I2C address, columns, rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Initialize the servo motor
Servo servoMotor;

// Define the buzzer pin
const int buzzerPin = 13;

// Define the LED pin
const int ledPin = 12;

// Define the password (change it as needed)
String password = "2468";
String tempPassword = "";

// Define the keypad layout
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Define the keypad row and column pins
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

// Create the keypad object
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // Initialize the LCD display
  lcd.begin();
  lcd.backlight();
  lcd.print("Enter Password:");

  // Attach the servo motor to pin 10
  servoMotor.attach(10);
  servoMotor.write(0); // Initial position (locked)

  // Set the buzzer and LED pins as output
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Turn off the LED initially
}

void loop() {
  char key = customKeypad.getKey();

  if (key) {
    if (key == '#') {
      // Check if the entered password is correct
      if (tempPassword == password) {
        lcd.clear();
        lcd.print("Access Granted!");
        servoMotor.write(90); // Unlock the door
        digitalWrite(ledPin, HIGH); // Turn on the LED
        delay(2000); // Keep the door unlocked for 2 seconds
        servoMotor.write(0); // Lock the door again
        digitalWrite(ledPin, LOW); // Turn off the LED
        lcd.clear();
        lcd.print("Enter Password:");
        tempPassword = ""; // Reset the temporary password
      } else {
        lcd.clear();
        lcd.print("Access Denied!");
        digitalWrite(buzzerPin, HIGH); // Sound the buzzer
        delay(1000); // Buzzer sounds for 1 second
        digitalWrite(buzzerPin, LOW); // Turn off the buzzer
        lcd.clear();
        lcd.print("Enter Password:");
        tempPassword = ""; // Reset the temporary password
      }
    } else if (key == '*') {
      // Clear the entered password
      tempPassword = "";
      lcd.clear();
      lcd.print("Enter Password:");
    } else {
      // Append the key to the temporary password
      tempPassword += key;
      lcd.setCursor(0, 1);
      lcd.print(tempPassword);
    }
  }
}