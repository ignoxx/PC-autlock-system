#include "Keyboard.h"
#include "src/HCSR04.h";

enum OS { WIN, MAC, LINUX };
HCSR04 sensor(12, 13); // initialisation class HCSR04 (trig pin , echo pin)


// 
const OS targetOS = OS::LINUX;
const float maxDistanceDifferenceAllowed = 25;    // % (percent)
const float baseAllowedDistance = 80;             // cm
const long timeToLockScreenAfterDetection = 4000; // ms
long scanningSpeed = 1000;                        // ms
unsigned long maxDistanceDetectionTime;           // ms
int actualDistance;
bool lockTimerStarted = false;
bool isScreenLocked = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  actualDistance = sensor.dist();
  Serial.println(actualDistance);

  if (!lockTimerStarted && shouldEnableLockTimer()) {
    lockTimerStarted = true;
    maxDistanceDetectionTime = millis();
    Serial.println("User is out of range, lock timer started.");
  }

  else if (lockTimerStarted && !shouldEnableLockTimer()) {
    Serial.println("User is back in range, lock timer disabled.");
    lockTimerStarted = false;
    scanningSpeed = 1000; // 1s
    isScreenLocked = false;
  }

  if (shouldLockScreen()) {
    Serial.println("Lock screen.");

    // if (!isScreenLocked)
    //   lockScreen();

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
  case OS::WIN:
    Serial.print("on Windows..");

    Keyboard.press(KEY_LEFT_GUI);
    delay(10);
    Keyboard.write('l');
    delay(10);
    Keyboard.releaseAll();
    break;

  case OS::MAC:
    Serial.print("on Mac..");
    break;

  case OS::LINUX:
    Serial.print("on Linux..");
    Keyboard.press(KEY_LEFT_GUI);
    delay(10);
    Keyboard.write('l');
    delay(10);
    Keyboard.releaseAll();
    break;
  }
}
