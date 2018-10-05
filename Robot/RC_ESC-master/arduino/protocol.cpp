#include "protocol.h"


// Message format is {COMMAND},{VALUE}

//constructor
Listener::Listener(byte left_motor_pin, byte right_motor_pin, int outputMin, int outputMax, int armVal) {
  
  left_motor = ESC(left_motor_pin, outputMin, outputMax, armVal);
  right_motor = ESC(right_motor_pin, outputMin, outputMax, armVal);
}


void Listener::serial_listen(void) {
  char aChar;
  char message[MAX_MESSAGE];
  int i=0;
  char* cmd;
  double val=0;
  char *ptr;
  Serial.println("awaiting input");

    aChar = Serial.read();
    message[0] = aChar;
    
    while (aChar != '~') {

// Any standard code coes here, note that it gets interrupted by serial input
      
      if (Serial.available()) {
        aChar = Serial.read();
        message[i] = aChar;
        i++;
        message[i] = '\0'; // Strings need to have null terminator
      }
    }

    ptr = strtok(message, " ,");
    strcpy(cmd, ptr);
    val = atof(strtok(NULL, " ~"));

    if (DEBUG) {
        Serial.print("Recieved: ");
        Serial.print(ptr);
        Serial.print(" -> ");
        Serial.println(val);
    }
    action(ptr, val);
    //Serial.flush();
}

void Listener::action(char *message, double value) {
/**
 * Calls relevent hardware functions based on message from serial
 */

    if (strcmp(message, "motor_left") == 0) {
      
      if (DEBUG) {
        Serial.print("Set -- motor_left = ");
        Serial.println(value);
      }
      
      left_motor.set_speed(value);
      
    } else if (strcmp(message, "motor_right") == 0) {
      
      if (DEBUG) {
        Serial.print("Set -- motor_right = ");
        Serial.println(value);
      }
      
      right_motor.set_speed(value);        

    } else if (strcmp(message, "calibrate_left") == 0) {
      
      left_motor.calib();
      
    } else if (strcmp(message, "calibrate_right") == 0) {
      
      right_motor.calib();

    } else if (strcmp(message, "stop") == 0) {

      left_motor.hit_break();
      right_motor.hit_break();
    
    } else  {
      if (DEBUG) { Serial.println("Unknown Serial Command"); }
    }
  
}

