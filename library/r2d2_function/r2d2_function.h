// Header file for functions.cpp
// Copyright 2014/2015
// Authors: Alessandro Grande, Abdul-Rehman Sharif, Lucinda Hartfield

// This also contains Ardumoto.h library functions to control DC motors\
// Copyright 2012 Jeroen Doggen (jeroendoggen@gmail.com)

#ifndef R2D2_FUNCTION
#define R2D2_FUNCTION

#include <Arduino.h>
#include <l298hn.h>
#include <hcsr04.h>

// Class specifies most of the functions needed for proj001_r2d2
class r2d2_function {
  public:
    // Constructor
    r2d2_function();

    // Read line sensor left
    int read_lline();
    // Read line sensor right
    int read_rline();
    // Returns 1 (front), -1 (back) and 0 (no) if line is detected
    int detect_line();
    // Results in random movement of bot
    void move_random(float speed);
    // Reverses the bot
    void reverse(float speed);
    // Turns the bot - 360 cw motion
    void turn(float speed);
    // Reverse and turns the bot
    void reverse_and_turn(float speed);
    // Drives the bot forward
    void drive(float speed);
    // Stops the bot
    void stop();
    // Detects an object using ultra sonic sensors
    int detect_object(int range, float speed);
    // Detects and object while moving in a 360 rotation
    void move_detect(float speed, int fwd_bck, int time, int wait);

  private:

};

#endif
