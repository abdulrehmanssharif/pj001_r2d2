// Header file for functions.cpp

#ifndef R2D2_FUNCTION
#define R2D2_FUNCTION

class r2d2_function {
  public:
    // Global parameters
    // Speed at which it move_forwards an object
    // optimum value: 0.4 to 0.8
    extern float moveforwardspeed;
    // Speed at which it rotates to find an object
    // optimum value: 0.3 to 0.5
    extern float searchspeed;
    // Range of detection
    // optimum value: 30 to 50
    extern unsigned int range;

    void flash_leds();
    void turn_leds_on();
    void turn_led_right();
    void turn_led_left();
    int read_line1();
    int read_line2();
    int detect_line();
    void move_random(float speed=moveforwardspeed);
    void reverse(float speed = moveforwardspeed);
    void turn(float speed = moveforwardspeed);
    void reverse_and_turn(float speed = moveforwardspeed);
    void move_forward(float speed = moveforwardspeed);
    void stop();
    int detect_object(int range_t = range, float speed = moveforwardspeed);
    void move_detect(float speed, int fwd_bck, int time, int wait=0);
};

#endif
