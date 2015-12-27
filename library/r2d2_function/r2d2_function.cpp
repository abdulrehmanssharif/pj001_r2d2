// The following file implements all the functions necessary 
// for the robot.
// Please don't modify any of these functions!

#include <Arduino.h>
#include <l298hn.h>
#include <hcsr04.h>
#include "r2d2_function.h"

// Two sensors are used to detect a line, line_sens1 and line_sens2
// if there is a difference between the readings of these sensors 
// the robot has detected a line.
// Setting pins for line sensor
#define US_ECHO 13
#define US_TRIG 12

// Setting pins for motor, as follows:
// Example: Motor____(PWM, Forward, Reverse)
#define MLEFT_EN 8
#define MLEFT_IN1 9
#define MLEFT_IN2 10

#define MRIGHT_EN 2
#define MRIGHT_IN1 3
#define MRIGHT_IN2 11

// Setting pins for line sensor
#define LLEFT 5
#define LRIGHT 4

l298hn motorLeft(MLEFT_EN, MLEFT_IN1, MLEFT_IN2);
l298hn motorRight(MRIGHT_EN, MRIGHT_IN1, MRIGHT_IN2);

// Setting pins for ultrasonic sensor, as follows:
// Example: usensor(Trigger, Echo)
hcsr04 usensor(US_TRIG ,US_ECHO);

r2d2_function::r2d2_function()
{

}

// Returns value from the QRE1113
// lower numbers mean more refleacive
// more than 3000 means nothing was reflected.
int r2d2_function::read_lline() {
  //Returns value from the QRE1113 
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( LLEFT, OUTPUT );
  digitalWrite( LLEFT, HIGH );  
  delayMicroseconds(10);
  pinMode( LLEFT, INPUT );

  long time = micros();

  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(LLEFT) == HIGH && micros() - time < 3000); 
  int diff = micros() - time;

  return diff;
}

int r2d2_function::read_rline() {
  //Returns value from the QRE1113 
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( LRIGHT, OUTPUT );
  digitalWrite( LRIGHT, HIGH );  
  delayMicroseconds(10);
  pinMode( LRIGHT, INPUT );

  long time = micros();

  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(LRIGHT) == HIGH && micros() - time < 3000); 
  int diff = micros() - time;

  return diff;
}

int r2d2_function::detect_line() {
  int line1val = read_lline();
  int line2val = read_rline();

  Serial.print("LineSensor readings:");
  Serial.print(line1val-line2val);
  Serial.print("\n");

  if ((line1val-line2val) > 70) {
    //printf("Line detected from front");
    return 1;
  } else if ((line1val-line2val) < -70) {
    //printf("Line detected from back");
    return -1;
  } else {
    //printf("Line not detected");
    return 0;
  }
}

void r2d2_function::reverse(float speed) {
  //printf("Reverse\n");
  motorLeft.speed(-(speed));
  motorRight.speed(-(speed));
}

void r2d2_function::turn(float speed) {
  //printf("Turning\n");
  motorLeft.speed(speed);
  motorRight.speed(-(speed));
}

void r2d2_function::reverse_and_turn(float speed) {
  //printf("Reverse and turn\n");
  motorLeft.speed((speed-0.3));
  motorRight.speed(-(speed-0.1));
}

void r2d2_function::drive(float speed) {
  motorLeft.speed(speed);
  motorRight.speed(speed);
}

void r2d2_function::move_random(float speed)
{
  int counter;
  int fwd_bck;
  int fwd;
  float random_time;

  //printf("Moving randomly\n");
  counter = rand() % 5;

  for (int i = 0; i < counter; i++)
  {
    random_time = rand() % 1000 + 1000;

    fwd_bck = rand()%2;
    
    // If fwd_back == 1 move forward or backwards
    if (fwd_bck == 1) {
      fwd = rand()%2;
      // If fwd == 1 move forward
      if  (fwd == 1) { 
        //printf("Moving forward\n");
        move_detect(speed, fwd_bck, random_time, 5);
      // If fwd == 0 move bacward
      } else {
        //printf("Moving bacwards\n");
        move_detect(speed, fwd_bck, random_time, 5);
      }   
    // Turn
    } else {
      //printf("Turning\n");
      move_detect(speed, fwd_bck, random_time, 5);
    }
  }
}

void r2d2_function::stop() {
  motorLeft.speed(0.0);
  motorRight.speed(0.0);
}

int r2d2_function::detect_object(int range_t, float speed) {
  // Start a timer - finds an object for 5 seconds
  // if it doesn't find anything returns 0
  long usensor_t, inner_t;
  usensor_t = micros();
  
  // Variable to store sensed value
  unsigned int sense, dist, reverse;
  sense = 0;
  dist = 0;
  reverse = 0;
  
  while ((micros() - usensor_t) < 15000) {
    // Start the ultrasonic sensor
    inner_t = micros();
    dist = usensor.ranging(1);
    
    Serial.print("UltraSonic readings: ");
    Serial.print(dist);
    Serial.print("\n");
    
    // If an object is detected based on out set range return 1
    if (dist <= range_t && dist >= 1) {
      sense = 1;
      stop();
      break;
    } else {
      sense = 0;
      turn(speed);
    }
    
    if ((micros() - inner_t) >=100) {
      if (reverse == 2) {
        speed = 0.7;
        reverse = 0;
      } else {
        speed = 0.0;
      }
      reverse++;
      inner_t = 0;
    }
  }
  
  usensor_t = 0;
  return sense;
}

void r2d2_function::move_detect(float speed, int fwd_bck, int time, int wait) {
  long t;
  t = micros();
  int detect = 0;
  
  if(fwd_bck == 1) {
    drive(speed);
    delay(wait);
  } else {
    turn(speed + 0.2);
  }
  
  while ((micros() - t) < time) {
    detect = detect_line();
    // If line is detected from front then reverse
    if(detect == 1) {
      stop();
      move_detect(-speed,1,1000,300);
      stop();
      break;
    // If line is detected from back just keep on moving forward
    } else if (detect == -1) {
      stop();
      move_detect(speed,1,1000,300);
      stop();
      break;
    }
  }
  stop();
  t = 0;
}
