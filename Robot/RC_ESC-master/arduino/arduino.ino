/**
 * Arduino code for hardware interfacing
 */


#include "protocol.h"
Listener listener = Listener(4,5,OUTMIN,OUTMAX,PWM);
void setup() {
  
  Serial.begin(250000);
 
}

void loop() {
  listener.serial_listen();
  

}
