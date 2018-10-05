/* Electronic Speed Controller (ESC) - Library */

/*
	
*/

#include "ESC.h"

// < Constructor >
/* Sets the proper pin to output.
*/
ESC::ESC(byte ESC_pin, int outputMin, int outputMax, int armVal)
{
	oPin = ESC_pin;
	rMax = outputMin;
	fMax = outputMax;
	oArm = armVal;
}
ESC::ESC(void) {
  
}

// < Destructor >
ESC::~ESC()
{
	// Nothing to destruct
}

void ESC::set_speed(double value) {
  myESC.attach(oPin);
  int new_duty;

  // do not exceed 100/-100%
  if (value > 100) {
    value = 100;
  }
  if (value < -100) {
    value = -100;
  }

  
  
  if (value > 0) {
      new_duty = (int) (fMax - oESC)*value/100 + oESC;
      
  } else if (value <= 0) {
      new_duty = (int) (oESC + (oESC - rMax)*value/100);
      
  }
Serial.println(oESC);
  // if asked to change motor directon, stop first to avoid damage to motors
  if ((new_duty > oESC && duty < oESC) ||
      (new_duty < oESC && duty > oESC)) {
        hit_break();
        delay(2000);      // in fwd/pause/brk mode, 2 second delay is needed
      }
  
  duty = new_duty;

  if (ESC_DEBUG) {
    Serial.print("Duty cycle set to: ");
    Serial.println(duty);
  }
  
  myESC.writeMicroseconds(duty);  
}

void ESC::hit_break(void) {
  if (ESC_DEBUG) {
    Serial.println("Hit the breaks");
  }
  
  myESC.writeMicroseconds(oBrk);
  delay(100);
  myESC.writeMicroseconds(oESC);
}

/* Calibrate the maximum and minimum PWM signal the ESC is expecting
*/
void ESC::calib(void)
{
Serial.println("Duty cycles are: ");
Serial.print("oESC ");
Serial.println(oESC);
Serial.print("fMax ");
Serial.println(fMax);
Serial.print("rMax ");
Serial.println(rMax);
Serial.print("oBrk ");
Serial.println(oBrk);
  
	myESC.attach(oPin);  			      // attaches the ESC on pin oPin to the ESC object
	myESC.writeMicroseconds(oESC);  // begin calibration, neutural 
 
  Serial.print("Press and hold 'set' on the ESC. Blue LED will become solid\n");
  
  delay(ESC_CAL_DELAY);

  myESC.writeMicroseconds(fMax);  // set to full throttle

  Serial.print("Red LED should flash and moter will beep\n");

delay(ESC_CAL_DELAY);


//  for (int i=fMax; i >= oBrk; i--) {
//    myESC.writeMicroseconds(i);   // move to full break
//    delay(5);
//  }
    myESC.writeMicroseconds(oESC);
    delay(200);
    myESC.writeMicroseconds(oBrk);
  

    Serial.print("Moter will beep twice\n");


delay(ESC_CAL_DELAY);
  myESC.writeMicroseconds(oESC); 
  
  Serial.print("Red and Blue LEDs will blink, moter will beep three times\n");

delay(ESC_CAL_DELAY);
  Serial.print("Restart ESC to save calibration\n");
}


void ESC::arm(void)
{
	myESC.attach(oPin);  			// attaches the ESC on pin oPin to the ESC object
	myESC.writeMicroseconds (oArm);
}
void ESC::stop(void)
{
	myESC.writeMicroseconds (ESC_STOP_PULSE);
}
void ESC::speed(int outputESC)
{
	oESC = constrain(outputESC, rMax, fMax);
	myESC.writeMicroseconds(oESC);
}
