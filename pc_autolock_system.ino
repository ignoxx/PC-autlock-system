#include <HCSR04.h>
#include "Keyboard.h"

enum OS { WIN, MAC, LINUX };
UltraSonicDistanceSensor sensor(12, 13);

const OS targetOS = OS::LINUX;
const float maxDistanceDifferenceAllowed = 25;     // % (percent)
const double maxAllowedDistance = 45;
const float timeToLockScreenAfterDetection = 3000; // 3 seconds
unsigned long maxDistanceDetectionTime;            // in ms
int actualDistance;
bool lockTimerStarted = false;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {

  Serial.println(sensor.measureDistanceCm());
  delay(1000);
}
// void loop() {
//   actualDistance = getActualDistance();
//   Serial.println(actualDistance);
//   if (!lockTimerStarted && shouldEnableLockTimer()) {
//     lockTimerStarted = true;
//     maxDistanceDetectionTime = millis();
//     Serial.println("User is out of range, lock timer started.");
//   }

//   else if (!shouldEnableLockTimer()) {
//     Serial.println("User is back in range, lock timer disabled.");
//     lockTimerStarted = false;
//   }

//   if (shouldLockScreen()) {
//     // lockScreen();
//     Serial.println("Lock screen.");
//   }

//   delay(2000);
// }

bool shouldLockScreen() {

  auto currentTime = millis() - maxDistanceDetectionTime;

  //Serial.print("..currentTime: "); Serial.print(currentTime); Serial.print("..lockTimerStarted: "); Serial.print(lockTimerStarted);

  return lockTimerStarted && currentTime > timeToLockScreenAfterDetection;
}

bool shouldEnableLockTimer() {
  int allowedDistance = maxAllowedDistance * 1 + (maxDistanceDifferenceAllowed/100);
  
  //Serial.print("actualDistance: "); Serial.print(actualDistance);
  //Serial.print("..allowedDistance: "); Serial.print(allowedDistance); 
  return actualDistance > allowedDistance;
}

int getActualDistance() {
  double distanceSum = 0;

  for (byte i = 0; i < 10; i++) {
    distanceSum += sensor.measureDistanceCm();
    delay(10);
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
    Keyboard.press(KEY_LEFT_GUI);
    delay(10);
    Keyboard.write('l');
    delay(10);
    Keyboard.releaseAll();
    break;
  }
}

