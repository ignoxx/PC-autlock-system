#include "HCSR04.h"
////////////////////////////////////consttruct/destruct
void HCSR04::init(int out, int echo[], int n) {
  this->out = out;
  this->echo = echo;
  this->n = n;
  pinMode(this->out, OUTPUT);
  for (int i = 0; i < n; i++) {
    pinMode(this->echo[i], INPUT);
  }
}
HCSR04::HCSR04(int out, int echo) { this->init(out, new int[1]{echo}, 1); }
HCSR04::HCSR04(int out, int echo[], int n) { this->init(out, echo, n); }
HCSR04::~HCSR04() {
  ~this->out;
  delete[] this->echo;
  ~this->n;
}
///////////////////////////////////////////////////dist
float HCSR04::dist(int n) const {
  // Make sure to set trigPin is clear/LOW State for 5 µs
  digitalWrite(this->out, LOW);
  delayMicroseconds(5);

  // For generating Ultra sound wave, set 'trigPin' to HIGH State for 10 µs
  // The sensor then creates an 8 cycle burst of ultrasound at 40 kHz
  digitalWrite(this->out, HIGH);
  delayMicroseconds(10);

  // Set trigPin to LOW, to stop sending the sound wave and start/stop the
  // duration to travel back
  digitalWrite(this->out, LOW);

  noInterrupts();
  /*
   When sound waves reach receiver, it turns the echoPin HIGH for the same
   duration. pulseIn(), function waits for echoPin to go into HIGH and then
   stops timing when it switches to LOW state. Read in the echoPin input, as the
   roundtrip duration for ultra sound wave to reach back from detected object.
 */
  long d = pulseIn(this->echo[n], HIGH);
  interrupts();

  /*
    Let 'd' be the distance, and speed of sound as 343 m/s Or 0.034cm/µs, there
    distance will equal duration multiplied by speed of sound. As the sound wave
    has to travel to the object and back to the sensor echoPin, we need to only
    consider half the duration to obect in front. distance to object =
    (traveltime/2) x speed of sound
  */
  return (d * 0.0343) / 2;
}
float HCSR04::dist() const { return this->dist(0); }
