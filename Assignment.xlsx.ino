#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LcdBarGraphRobojax.h>
#include <NewPing.h>
#include <Servo.h>

byte lcdNumCols = 16; // number of columns in the LCD
byte lcdLine = 2; // number of lines in the LCD

LiquidCrystal_I2C lcd(0x27, lcdNumCols, lcdLine); // 0x27 is the I2C address for the LCD
LcdBarGraphRobojax rjx(&lcd, lcdNumCols, 0, 0); // Creating a 16-character long bargraph starting at char 0 of line 0

#define TRIGGER_PIN  4 // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 15 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Servo servoMain;//
Servo servoMain2;
int trigpin = 9;
int echopin = 8;
int distance;
float duration;
float cm;

int VCC2 = 2; // Additional VCC(5V) for the device

void setup()
{
  pinMode(VCC2, OUTPUT); // Set the pin mode for the extra VCC
  digitalWrite(VCC2, HIGH); // Set extra VCC to HIGH (5V)

  lcd.init(); // Initialize the LCD
  lcd.clear();
  lcd.print("Smart");
  lcd.setCursor(0, 1);
  lcd.print("Dustbin");

  delay(2000); // Delay for visualization
  lcd.clear();

  servoMain.attach(7);//
  servoMain2.attach(6);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);


   
}

void loop()
{
  int distanceCM = sonar.ping_cm(); // Get distance in cm
  int distanceIN = sonar.ping_in(); // Get distance in inches

  rjx.clearLine(1); // Clear line 1 to display fresh voltage value

  // Draw bar graph based on the distance
  rjx.drawValue(distanceCM, MAX_DISTANCE);

  lcd.setCursor(0, 1);
  lcd.print("Bin:");

  if (distanceCM >= 0 && distanceCM <= 3)
  {
    lcd.setCursor(5, 1);
    lcd.print("Full");
  }
  else if (distanceCM > 3 && distanceCM <= 13)
  {
    lcd.setCursor(5, 1);
    lcd.print("Half Full");
  }
  else if (distanceCM > 14 && distanceCM <= 15)
  {
    lcd.setCursor(5, 1);
    lcd.print("Empty_Bin");
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print("Empty");
  }

  // Adjust the backlight intensity
  int backlightIntensity = map(distanceCM, 0, MAX_DISTANCE, 255, 0);
  lcd.setBacklight(backlightIntensity);

  digitalWrite(trigpin, LOW);
  delay(2);

  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);

  cm = (duration / 58.82);
  distance = cm;


  if (distance < 30) {
    servoMain.write(180);
    servoMain2.write(180);
    delay(3000);
  } else {
    servoMain.write(0);
    servoMain2.write(0);
    delay(50);
  }
}
