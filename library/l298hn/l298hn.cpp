/* mbed simple H-bridge motor controller
 * Copyright (c) 2007-2010, sford, http://mbed.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Arduino.h>
#include "l298hn.h"

l298hn::l298hn(int pwm, int fwd, int rev):
        _pwm(pwm), _fwd(fwd), _rev(rev) {

  // Set initial condition of PWM
  //_pwm.period(0.001); // No need to setup by default set to 1khz
  pinMode(_pwm, OUTPUT);
  analogWrite(_pwm, 0);

  // Initial condition of output enables
  pinMode(_fwd, OUTPUT);
  analogWrite(_fwd, 0);
  pinMode(_rev, OUTPUT);
  analogWrite(_rev, 0);
}

void l298hn::speed(float speed) {
  int pwm_mod;
  pwm_mod = speed * 255;

  // Set reverse and forward speed based on value
  digitalWrite(_fwd, (speed > 0.0));
  digitalWrite(_rev, (speed < 0.0));
  // Set PWM based on speed , value goes from 0 to 255
  analogWrite(_pwm, abs(pwm_mod));
}



