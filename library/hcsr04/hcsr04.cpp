/*
  Ultrasonic.cpp - Library for HC-SR04 Ultrasonic Ranging Module.library
  Created by ITead studio. Apr 20, 2010.
  iteadstudio.com
  
  * modified 29 Oct 2013
  * by Hemal Chevli - black-electronics.com
*/

#include <Arduino.h>
#include "hcsr04.h"

hcsr04::hcsr04(int TP, int EP)
{
   pinMode(TP,OUTPUT);
   pinMode(EP,INPUT);
   Trig_pin=TP;
   Echo_pin=EP;
}

long hcsr04::timing()
{
  digitalWrite(Trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_pin, LOW);
  duration = pulseIn(Echo_pin,HIGH);
  return duration;
}

long hcsr04::ranging(int sys)
{
  timing();
  distance_cm = duration /29 / 2 ;
  distance_inc = duration / 74 / 2;
  if (sys)
  return distance_cm;
  else
  return distance_inc;
}
