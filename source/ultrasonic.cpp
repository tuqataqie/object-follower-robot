#include "ultrasonic.h"

static DigitalOut trig_p(D11, 0);  
static InterruptIn echo_p(D10);     
static Timer timer_obj;

volatile float calculated_distance = -1.0f;
volatile bool pulse_received = false;

void on_echo_rise() {
    timer_obj.reset();
    timer_obj.start();
}

void on_echo_fall() {
    timer_obj.stop();
    auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(timer_obj.elapsed_time()).count();
    calculated_distance = (float)time_us / 58.0f;
    pulse_received = true; 
}

void ultrasonic_init() {
    echo_p.mode(PullDown);
    echo_p.rise(&on_echo_rise);
    echo_p.fall(&on_echo_fall);
}

float get_range() {
    pulse_received = false; 
    
    trig_p = 1;
    wait_us(12); 
    trig_p = 0;
    
    Timer timeout_timer;
    timeout_timer.start();
    while (!pulse_received) {
        if (timeout_timer.elapsed_time() > 45ms) {
            return -1.0f; 
        }
    }
    
    return calculated_distance;
}