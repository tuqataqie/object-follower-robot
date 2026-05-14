#include "motor.h"

static DigitalOut in1(PB_0, 0); 
static DigitalOut in2(PB_1, 0); 
static PwmOut pwm_ctrl(D3);

void motor_init() {
    pwm_ctrl.period_ms(1); 
}

void car_forward(float dc) {
    in2 = 0; in1 = 1;
    pwm_ctrl.write(dc);
}

void car_stop() {
    in1 = 0; in2 = 0;
    pwm_ctrl.write(0.0f);
}