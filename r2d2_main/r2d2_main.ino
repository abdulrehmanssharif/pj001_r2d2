// Code written by:
// Jon Baker
// Alessandro Grande
// Abdul-Rehman Sharif
// Lucinda Hartfield

// Circuitry made by:
// Yucando Navarrete
// Vivekanand Gupta

// The following code will control a robotic car
// by detetecting an object and charging towards it
// it uses basic functions as:
//
// move_forward(speed)
//                      -   used to move_forward on an object detected
//                          the robot will move in a straight line
//                          until it detects the arena line where
//                          it will use reverse() to move back
//
// detect_object(range, speed)
//                      -   used to detect an object, the robot will
//                          move in a circle to find an object and
//                          return 1 if it finds something and return
//                          0 if it does not find anything - the search
//                          will be carried out for 15 seconds.
//
// detect_line ()
//                      -   used to detect a line, it returns the following
//                          an int value as follows:
//                          0  - if no line is detected
//                          1  - if line detected from the front
//                          -1 - if line detected from the back
//
// reverse(speed)
//                      -   reverses the robot with moveforwardspeed in same position
//
// stop()
//                      -   stops the robot
//
// move_random(speed)
//                      -   used to move the robot randomly, the robot will either
//                          move forward, move backward, or turn around. The movement 
//                          will be carried out for a random time
//

#include <Arduino.h>
#include <hcsr04.h>
#include <l298hn.h>                
#include <r2d2_function.h>

r2d2_function r2d2;

// Global parameters
// Speed at which it move_forwards an object
// optimum value: 0.4 to 0.8
float moveforwardspeed;
// Speed at which it rotates to find an object
// optimum value: 0.3 to 0.5
float searchspeed;
// Range of detection
// optimum value: 30 to 50
unsigned int range;

void setup() {
  Serial.begin(9600);
  moveforwardspeed = 0.6;
  searchspeed = 0.4;
  range = 30;
  
  // Give some settling time
  delay(5000);
}

void loop() {
  int detect_l = 0;
  int detect_o = 0;

  while(true) {
    // Sample code to detect and object and move_forward at it
    detect_o = r2d2.detect_object(range, searchspeed);

    if (detect_o == 1) {
      Serial.print("Detected an object nearby");
      r2d2.drive(moveforwardspeed);
      
      while (true) {
        detect_l = r2d2.detect_line();
        // If line is detected from front then reverse
        if(detect_l == 1 || detect_l == -1) {
          Serial.print("Detected line");
          r2d2.stop();
          r2d2.reverse(searchspeed);
          detect_l = 0;
          break;
        }
      }
    } else {
      r2d2.move_random(moveforwardspeed);
    }

    detect_o = 0;
    r2d2.stop();
  }
}
