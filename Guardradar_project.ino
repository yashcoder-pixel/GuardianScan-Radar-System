#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo radarServo;

#define redPin     13
#define greenPin   12
#define BUZZER     2
#define servoPin   9
#define echoPin    8
#define trigPin    7

const int detectionZone = 50;
const int dangerZone = 20;

enum State {
  BOOT,
  SCANNING,
  OBJECT_DETECTED,
  TARGET_LOCKED,
  ALARM_ACTIVE,
  THREAT_CLEARED
};

State currentState = BOOT;
State previousState = BOOT;

int angle = 0;
bool forward = true;

int targetAngle = 0;
float distance = 0;

unsigned long lastScan = 0;
unsigned long lastLCD = 0;
const unsigned long LCD_REFRESH_INTERVAL = 150; // Throttles LCD refresh to 150ms to eliminate flicker

// Measure Distance
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  float d = duration * 0.034 / 2.0;

  if (d <= 0 || d > 400)
    d = 400;

  return d;
}

// Radar Data for Processing
void sendRadarData(int a, float d) {
  Serial.print(a);
  Serial.print(",");
  Serial.println(d);
}

// Display routines formatted to exactly 16 characters to wipe out old text cleanly
void showScan() {
  char line0[17];
  char line1[17];

  snprintf(line0, sizeof(line0), "SCAN: %3d\xDF       ", angle);
  snprintf(line1, sizeof(line1), "DIST: %3d cm    ", (int)distance);

  lcd.setCursor(0, 0);
  lcd.print(line0);
  lcd.setCursor(0, 1);
  lcd.print(line1);
}

void showDetected() {
  char line1[17];
  snprintf(line1, sizeof(line1), "DIST: %3d cm    ", (int)distance);

  lcd.setCursor(0, 0);
  lcd.print("OBJECT DETECTED ");
  lcd.setCursor(0, 1);
  lcd.print(line1);
}

void showLocked() {
  char line1[17];
  snprintf(line1, sizeof(line1), "ANGLE: %3d\xDF     ", targetAngle);

  lcd.setCursor(0, 0);
  lcd.print("TARGET LOCKED!  ");
  lcd.setCursor(0, 1);
  lcd.print(line1);
}

void showAlarm() {
  char line1[17];
  snprintf(line1, sizeof(line1), "DIST: %3d cm    ", (int)distance);

  lcd.setCursor(0, 0);
  lcd.print("!! ALARM ACTIVE ");
  lcd.setCursor(0, 1);
  lcd.print(line1);
}

void showCleared() {
  lcd.setCursor(0, 0);
  lcd.print("THREAT CLEARED  ");
  lcd.setCursor(0, 1);
  lcd.print("RESUMING SCAN...");
}

// Setup
void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  lcd.init();
  lcd.backlight();

  radarServo.attach(servoPin);

  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("GuardianScan    ");
  lcd.setCursor(0, 1);
  lcd.print("System Ready    ");

  digitalWrite(greenPin, HIGH);

  delay(2000);
  lcd.clear();

  currentState = SCANNING;
}

// Main Loop
void loop() {

  // Clean the screen ONCE on state transition to prevent visual artifacts
  if (currentState != previousState) {
    lcd.clear();
    previousState = currentState;
  }

  switch (currentState) {

    // SCANNING
    case SCANNING:
      digitalWrite(greenPin, HIGH);
      digitalWrite(redPin, LOW);
      noTone(BUZZER);

      if (millis() - lastScan >= 20) {
        lastScan = millis();

        radarServo.write(angle);
        distance = getDistance();
        sendRadarData(angle, distance);

        // Refresh LCD every 150ms instead of every 20ms
        if (millis() - lastLCD >= LCD_REFRESH_INTERVAL) {
          lastLCD = millis();
          showScan();
        }

        if (distance <= dangerZone) {
          targetAngle = angle;
          currentState = TARGET_LOCKED;
        } else if (distance <= detectionZone) {
          currentState = OBJECT_DETECTED;
        }

        if (forward) {
          angle++;
          if (angle >= 180) forward = false;
        } else {
          angle--;
          if (angle <= 0) forward = true;
        }
      }
      break;

    // OBJECT DETECTED
    case OBJECT_DETECTED:
      digitalWrite(greenPin, LOW);
      digitalWrite(redPin, HIGH);

      showDetected();
      delay(400);

      distance = getDistance();

      if (distance <= dangerZone) {
        targetAngle = angle;
        currentState = TARGET_LOCKED;
      } else {
        currentState = SCANNING;
      }
      break;

    // TARGET LOCKED
    case TARGET_LOCKED:
      radarServo.write(targetAngle);
      showLocked();

      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);

      delay(500); // Short pause to display "Target Locked"
      currentState = ALARM_ACTIVE;
      break;

    // ALARM ACTIVE
    case ALARM_ACTIVE:
      radarServo.write(targetAngle);
      distance = getDistance();

      tone(BUZZER, 1000);
      sendRadarData(targetAngle, distance);

      // Continuously update distance on LCD during alarm state
      if (millis() - lastLCD >= LCD_REFRESH_INTERVAL) {
        lastLCD = millis();
        showAlarm();
      }

      if (distance > dangerZone) {
        noTone(BUZZER);
        currentState = THREAT_CLEARED;
      }
      break;

    // THREAT CLEARED
    case THREAT_CLEARED:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      noTone(BUZZER);

      showCleared();
      delay(1500);

      currentState = SCANNING;
      break;

    default:
      currentState = SCANNING;
      break;
  }
}