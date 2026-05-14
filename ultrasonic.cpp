#include "ultrasonic.h"

static DigitalOut trig_p(D11, 0);  
static DigitalIn echo_p(D10, PullDown);     
static Timer timer_obj;

float get_range() {
    trig_p = 1;
    wait_us(12); 
    trig_p = 0;
    timer_obj.reset();
    timer_obj.start();
    while (!echo_p) {
        if (timer_obj.elapsed_time() > 45ms) return -1.0f;
    }
    timer_obj.reset();
    while (echo_p) {
        if (timer_obj.elapsed_time() > 45ms) return -1.0f;
    }
    timer_obj.stop();
    auto time_us = std::chrono::duration_cast<std::chrono::microseconds>(timer_obj.elapsed_time()).count();
    return (float)time_us / 58.0f;
}