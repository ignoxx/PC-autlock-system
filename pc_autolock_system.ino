/*
 *  Libraries used by this project
 *  Ultrasonic-HC-SR04: https://github.com/JRodrigoTech/Ultrasonic-HC-SR04
 */

#include "Keyboard.h"
#include "Ultrasonic.h"

enum OS { WIN, MAC, LINUX };
Ultrasonic ultrasonic(12, 13);

const OS targetOS = OS::WIN;
const float maxDistanceDifferenceAllowed = 25;     // % (percent)
const float timeToLockScreenAfterDetection = 3000; // 3 seconds
unsigned long maxDistanceDetectionTime;            // in ms
int actualDistance;
bool lockTimerStarted = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {
  actualDistance = getActualDistance();
  if (!lockTimerStarted && shouldEnableLockTimer()) {
    lockTimerStarted = true;
    maxDistanceDetectionTime = millis();
    Serial.println("User is out of range, lock timer started.");
  }

  else if (!shouldEnableLockTimer()) {
    Serial.println("User is back in range, lock timer disabled.");
    lockTimerStarted = false;
  }

  if (shouldLockScreen()) {
    // lockScreen();
    Serial.println("Lock screen.");
  }

  delay(1000);
}

bool shouldLockScreen() {

  auto currentTime = millis() - maxDistanceDetectionTime;

  Serial.print("..currentTime: "); Serial.print(currentTime); Serial.print("..lockTimerStarted: "); Serial.print(lockTimerStarted);

  return lockTimerStarted && currentTime > timeToLockScreenAfterDetection;
}

bool shouldEnableLockTimer() {
  int allowedDistance = maxDistanceDifferenceAllowed / 100;
  
  Serial.print("actualDistance: "); Serial.print(actualDistance);
  Serial.print("..allowedDistance: "); Serial.print(allowedDistance); 
  return actualDistance > allowedDistance;
}

int getActualDistance() {
  long distanceSum = 0;

  for (byte i = 0; i < 10; i++) {
    distanceSum += ultrasonic.Ranging(CM);
  }

  return distanceSum / 10;
}

void lockScreen() {
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
    break;
  }
}
