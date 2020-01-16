# PC autolock system using arduino
Autlock your PC when it detects that the user left his workplace

# Used hardware
- Arduino Leonardo (should support Keyboard simulations)
- Ultrasonic Sensor (for measuring distance)

# Installation
Connect the echo & trigger pin of the sensor to pin 12 & 13 of the Arduino, upload the code, connect the usb to your pc and the arduino and you're done.

Depending on your OS you've to modify the screen-lock-shortcut

The measurements are not always reliable, consider more factors to check if the user is really away from his workplace
