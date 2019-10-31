#include "Keyboard.h"
#include "src/HCSR04.h";

#define DEV_MODE

/*
  HCSR04(trigger, echo)
  trigger     - trigger pin
  echo        - echo pin
*/
HCSR04 sensor(12, 13);

enum OS { WIN, MAC, LINUX };

/*
  Each OS has its own Key shortcuts for locking the screen
  Set the target OS here (Make the selection user changeable)
*/
const OS targetOS = OS::LINUX;

/*
  The base distance between the user and the device
  Needs to be adjusted for each workplace (Automate this)
*/
const float baseAllowedDistance = 80; // cm

/*
  Max distance allowed before it triggers to lock screen
  Value depends procentually on baseAllowedDistance
*/
const float maxDistanceDifferenceAllowed = 25; // % (percent)

const long timeToLockScreenAfterDetection = 4000; // ms

/*
  The speed(in ms) for measuring the distance
  if the screen is locked, speed is slowed down.
*/
long scanningSpeed = 1000; // ms

unsigned long maxDistanceDetectionTime; // ms
int actualDistance;
bool lockTimerStarted = false;
bool isScreenLocked = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  actualDistance = sensor.dist();
  Serial.print("Measured distance:\t");
  Serial.print(actualDistance);
  Serial.print("\n");

  if (!lockTimerStarted && shouldEnableLockTimer()) {
    lockTimerStarted = true;
    maxDistanceDetectionTime = millis();
    Serial.println("User is out of range, lock timer started.");
  }

  else if (lockTimerStarted && !shouldEnableLockTimer()) {
    lockTimerStarted = false;
    scanningSpeed = 1000; // 1s
    isScreenLocked = false;

    Serial.println("User is back in range, lock timer disabled.");
  }

  if (shouldLockScreen()) {
    Serial.println("Lock screen..");

#ifndef DEV_MODE
    if (!isScreenLocked)
      lockScreen();
#endif
    scanningSpeed = 10000; // 10s
  }

  delay(scanningSpeed);
}

bool shouldLockScreen() {

  auto currentTime = millis() - maxDistanceDetectionTime;
  return lockTimerStarted && currentTime > timeToLockScreenAfterDetection;
}

bool shouldEnableLockTimer() {
  int allowedDistance =
      baseAllowedDistance * (1 + (maxDistanceDifferenceAllowed / 100));
  return actualDistance > allowedDistance;
}

void lockScreen() {
  isScreenLocked = true;
  Serial.println("Pressing..");

  switch (targetOS) {
  /*
    Windows and Linux use the same shortcut
  */
  case OS::WIN:
  case OS::LINUX:
    Serial.print("on Windows..");

    Keyboard.press(KEY_LEFT_GUI);
    delay(10);
    Keyboard.write('l');
    delay(10);
    Keyboard.releaseAll();
    break;

  case OS::MAC:
    Serial.print("on Mac..");
    Keyboard.press(KEY_LEFT_GUI);
    delay(10);
    Keyboard.press(KEY_LEFT_CTRL);
    delay(10);
    Keyboard.write('q');
    delay(10);
    Keyboard.releaseAll();
    break;
  }
}
