/* Electronic Speed Controller (ESC) - Library */

/*

*/

#ifndef ESC_Library
#define ESC_Library

#if (ARDUINO >= 100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Servo.h>				// Including the Servo library
#define ESC_CAL_DELAY	10000	// Calibration delay (milisecond)
#define ESC_STOP_PULSE	500	//
#define PIN   4      // pin number
#define OUTMIN 750    // min duty cycle
#define OUTMAX 1750   // max duty cycle
#define BREAK  750
#define PWM    50    // pwm frequency
#define ESC_DEBUG 1

class ESC
{
	public:
    void maunal_pwm(void);
		ESC(byte ESC_pin, int outputMin, int outputMax, int armVal);
    ESC(void);
		~ESC();
		void calib(void);
		void arm(void);
		void stop(void);
		void speed(int ESC_val);
    void hit_break(void);
    void set_speed(double value);
    
    byte oPin;      // ESC output Pin
    int rMax = OUTMIN; 
    int fMax = OUTMAX;
    int oESC = (fMax + rMax) / 2;
    int oArm = PWM;
    int oBrk = BREAK;

    int duty = 0; // duty cycle
    
	Servo myESC;    // create servo object to control an ESC
	private:
	// < Local attributes >
		// Hardware
};

#endif

/* Electronic Speed Controller (ESC) - Library */
