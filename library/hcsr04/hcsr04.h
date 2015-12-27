/*
  Ultrasonic.h - Library for HR-SC04 Ultrasonic Ranging Module.
  Created by ITead studio. Alex, Apr 20, 2010.
  iteadstudio.com
  
  * modified 29 Oct 2013
  * by Hemal Chevli - black-electronics.com
*/


#ifndef HCSR04_h
#define HCSR04_h

#include <Arduino.h>
#define CM 1
#define INC 0

class hcsr04
{
  public:
    hcsr04(int TP, int EP);
    long timing();
    long ranging(int sys);

    private:
    int Trig_pin;
    int Echo_pin;
    long duration,distance_cm,distance_inc;
    
};

#endif
