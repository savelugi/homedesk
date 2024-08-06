#include <Arduino.h>

// Define pin numbers for the Wemos D1 Mini
const int buttonPin1 = D1;  // GPIO5
const int buttonPin2 = D2;  // GPIO13

const int r_en = D3;   // GPIO0
const int l_en = D4;   // GPIO2
const int r_pwm = D5;  // GPIO14
const int l_pwm = D6;  // GPIO12

const int ledPin = LED_BUILTIN;  // Built-in LED for status indication

// Variables to store button states and timing
int buttonState1 = 0;
int buttonState2 = 0;
unsigned long buttonPressTime1 = 0;
unsigned long buttonPressTime2 = 0;
unsigned long runStartTime1 = 0;
unsigned long runStartTime2 = 0;

const unsigned long maxPressTime = 1000;   // 1 second to reach max speed
const unsigned long maxRunTime = 20000;    // 20 seconds maximum run time

void setup() {
  Serial.begin(9600);

  // Initialize button pins as input
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // Initialize motor control pins as output
  pinMode(r_en, OUTPUT);
  pinMode(l_en, OUTPUT);
  pinMode(r_pwm, OUTPUT);
  pinMode(l_pwm, OUTPUT);

  // Initialize the built-in LED pin as output for status indication
  pinMode(ledPin, OUTPUT);
  
  // Ensure motor is stopped initially
  digitalWrite(r_en, LOW);
  digitalWrite(l_en, LOW);
  digitalWrite(ledPin, LOW);
}

void loop() {
  // Read button states
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);

  unsigned long currentTime = millis();

  if (buttonState1 == HIGH) {
    if (buttonPressTime1 == 0) {
      buttonPressTime1 = currentTime;  // Record the time the button was first pressed
      runStartTime1 = currentTime;     // Record the run start time
    }

    unsigned long pressDuration = currentTime - buttonPressTime1;
    unsigned long runDuration = currentTime - runStartTime1;

    if (runDuration < maxRunTime) {
      int speed = map(min(pressDuration, maxPressTime), 0, maxPressTime, 0, 255);  // Map the press duration to PWM value

      // Enable motor control
      digitalWrite(r_en, HIGH);
      digitalWrite(l_en, HIGH);

      // Set motor speed and direction to forward
      analogWrite(r_pwm, speed);
      analogWrite(l_pwm, 0);

      digitalWrite(ledPin, HIGH);  // Turn on status LED

      Serial.print("Motor running forward at speed: ");
      Serial.println(speed);
    } else {
      // Stop the motor if run duration exceeds max run time
      digitalWrite(r_en, LOW);
      digitalWrite(l_en, LOW);

      Serial.println("Error: Motor stopped due to exceeding max run time");
    }
  } else if (buttonState2 == HIGH) {
    if (buttonPressTime2 == 0) {
      buttonPressTime2 = currentTime;  // Record the time the button was first pressed
      runStartTime2 = currentTime;     // Record the run start time
    }

    unsigned long pressDuration = currentTime - buttonPressTime2;
    unsigned long runDuration = currentTime - runStartTime2;

    if (runDuration < maxRunTime) {
      int speed = map(min(pressDuration, maxPressTime), 0, maxPressTime, 0, 255);  // Map the press duration to PWM value

      // Enable motor control
      digitalWrite(r_en, HIGH);
      digitalWrite(l_en, HIGH);

      // Set motor speed and direction to backward
      analogWrite(r_pwm, 0);
      analogWrite(l_pwm, speed);

      digitalWrite(ledPin, HIGH);  // Turn on status LED

      Serial.print("Motor running backward at speed: ");
      Serial.println(speed);
    } else {
      // Stop the motor if run duration exceeds max run time
      digitalWrite(r_en, LOW);
      digitalWrite(l_en, LOW);

      Serial.println("Error: Motor stopped due to exceeding max run time");
    }
  } else {
    // Reset the button press times
    buttonPressTime1 = 0;
    buttonPressTime2 = 0;
    runStartTime1 = 0;
    runStartTime2 = 0;

    // Stop the motor
    digitalWrite(r_en, LOW);
    digitalWrite(l_en, LOW);
    digitalWrite(ledPin, LOW);  // Turn off status LED

    Serial.println("Motor stopped");
  }

  // Add a small delay to avoid button bouncing issues
  delay(50);
}
