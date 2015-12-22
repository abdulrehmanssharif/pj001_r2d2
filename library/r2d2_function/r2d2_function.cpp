// The following file implements all the functions necessary 
// for the robot.
// Please don't modify any of these functions!

#include <functions.h>
#include <Ultrasonic.h>
#include <Ardumoto.h>
#include <defines.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include <pins_arduino.h>
#endif

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);

// Two sensors are used to detect a line, line_sens1 and line_sens2
// if there is a difference between the readings of these sensors 
// the robot has detected a line.
// Setting pins for line sensor
DigitalInOut line1(p20);
DigitalInOut line2(p19);

// Setting pins for motor, as follows:
// Example: Motor____(PWM, Forward, Reverse)
Motor MotorLeft(p23, p27, p28);
Motor MotorRight(p22, p30, p29);

// Setting pins for ultrasonic sensor, as follows:
// Example: usensor(Trigger, Echo)
HCSR04 usensor(p25,p26);

void turn_leds_on() {
        myled1 = 1;
        myled2 = 1;
        myled3 = 1;
        myled4 = 1;
        wait(0.1);
        myled1 = 0;
        myled2 = 0;
        myled3 = 0;
        myled4 = 0;
}

void turn_led_left() {
        myled1 = 1;
        myled2 = 1;
        wait(0.1);
        myled1 = 0;
        myled2 = 0;
}

void turn_led_right() {
        myled3 = 1;
        myled4 = 1;
        wait(0.1);
        myled3 = 0;
        myled4 = 0;
}

void flash_leds() {
    Timer t;
    t.start();
    while(t.read_ms() < 1000) {
        myled1 = 0;
        myled2 = 0;
        myled3 = 0;
        myled4 = 0;
        wait(0.15);
        myled1 = 1;
        wait(0.15);
        myled2 = 1;
        wait(0.15);
        myled3 = 1;
        wait(0.15);
        myled4 = 1;
        wait(0.15);
        myled1 = 0;
        myled2 = 0;
        myled3 = 0;
        myled4 = 0;
    }
    t.stop();
}

// Returns value from the QRE1113
// lower numbers mean more refleacive
// more than 3000 means nothing was reflected.
int read_line1() {
    // Time to record how long input is active
    Timer temp_t;
    
    // Activate the line sensor and then read
    line1.output();
    line1 = 1;
    wait_us(15);
    line1.input();
    
    // Start timer
    temp_t.start();

    // Time how long the input is HIGH, but quit
    // after 1ms as nothing happens after that
    while (line1 == 1 && temp_t.read_us() < 1000);
    return temp_t.read_us();
}

int read_line2() {
    // Time to record how long input is active
    Timer temp_t;
    
    // Activate the line sensor and then read
    line2.output();
    line2 = 1;
    wait_us(15);
    line2.input();
    
    // Start timer
    temp_t.start();

    // Time how long the input is HIGH, but quit
    // after 1ms as nothing happens after that
    while (line2 == 1 && temp_t.read_us() < 1000);
    return temp_t.read_us();
}

int detect_line() {
    int line1val = read_line1();
    int line2val = read_line2();
    
    printf("LineSensor readings: %d \n", line1val-line2val);
    
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

void reverse(float speed) {
    //printf("Reverse\n");
    MotorLeft.speed(-(speed));
    MotorRight.speed(-(speed));
}

void turn(float speed) {
    //printf("Turning\n");
    MotorLeft.speed(speed);
    MotorRight.speed(-(speed));
}

void reverseandturn(float speed) {
    //printf("Reverse and turn\n");
    MotorLeft.speed((speed-0.3));
    MotorRight.speed(-(speed-0.1));
}

void move_forward(float speed) {
    MotorLeft.speed(speed);
    MotorRight.speed(speed);
}

void move_random(float speed)
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
                move_detect(speed, fwd_bck, random_time);
            // If fwd == 0 move bacward
            } else {
                //printf("Moving bacwards\n");
                move_detect(speed, fwd_bck, random_time);
            }   
        // Turn
        } else {
            //printf("Turning\n");
            move_detect(speed, fwd_bck, random_time);
        }
    }
}

void stop() {
    MotorLeft.speed(0.0);
    MotorRight.speed(0.0);
}

int detect_object(int range_t, float speed) {
    // Start a timer - finds an object for 5 seconds
    // if it doesn't find anything returns 0
    Timer usensor_t, inner_t;
    usensor_t.start();
    
    // Variable to store sensed value
    unsigned int sense, dist, reverse;
    sense = 0;
    dist = 0;
    reverse = 0;
    
    while (usensor_t.read_ms() < 15000) {
        // Start the ultrasonic sensor
        usensor.start();
        inner_t.start();
        dist = usensor.get_dist_cm();
        
        printf("UltraSonic readings: %d \n", dist);
        
        // If an object is detected based on out set range return 1
        if (dist <= range_t && dist >= 1) {
            sense = 1;
            stop();
            flash_leds();
            break;
        } else {
            sense = 0;
            turn(speed);
        }
        
        if (inner_t.read_ms() >=100) {
            if (reverse == 2) {
                speed = 0.7;
                reverse = 0;
            } else {
                speed = 0.0;
            }
            reverse++;
            inner_t.reset();
        }
    }
    
    usensor_t.stop();
    usensor_t.reset();
    return sense;
}

void move_detect(float speed, int fwd_bck, int time, int wait) {
    Timer t;
    t.start();
    int detect = 0;
    
    if(fwd_bck == 1) {
        move_forward(speed);
        wait_ms(wait);
    } else {
        turn(speed + 0.2);
    }
    
    while (t.read_ms() < time) {
        detect = detect_line();
        // If line is detected from front then reverse
        if(detect == 1) {
            stop();
            turn_leds_on();
            move_detect(-moveforwardspeed,1,1000,300);
            stop();
            break;
        // If line is detected from back just keep on moving forward
        } else if (detect == -1) {
            stop();
            turn_leds_on();
            move_detect(moveforwardspeed,1,1000,300);
            stop();
            break;
        }
    }
    stop();
    t.stop();
}
