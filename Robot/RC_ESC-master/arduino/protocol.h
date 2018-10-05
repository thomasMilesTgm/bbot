#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "ESC.h"

#define MAX_MESSAGE 100
#define DEBUG 1 // print debug messages?




class Listener
{
  public:
    Listener(byte left_motor_pin, byte right_motor_pin, int outputMin, int outputMax, int armVal);
    void serial_listen(void);
    void action(char *message, double value);
    ESC left_motor;
    ESC right_motor;
    
};
