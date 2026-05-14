#include "mbed.h"

// --- Robot Pins Configuration ---
static DigitalOut trig_p(D11, 0);  
static DigitalIn  echo_p(D10, PullDown);     
static Timer      timer_obj;    

// Motor Driver (L298N)
static DigitalOut in1(PB_0, 0); 
static DigitalOut in2(PB_1, 0); 
static PwmOut     pwm_ctrl(D3);     

static DigitalOut car_led(LED1);
static BufferedSerial serial_line(USBTX, USBRX, 115200);

// --- Distance Thresholds (Matching your Video) ---
static constexpr float LIMIT_HIGH = 25.0f;  // cm
static constexpr float LIMIT_LOW  = 15.0f;  // cm
static constexpr float CAR_SPEED  = 0.35f;  // PWM speed

// Function to measure range
static float get_range()
{
    trig_p = 1;
    wait_us(12); // Pulse trigger
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

void car_forward(float dc) {
    in2 = 0;
    in1 = 1;
    pwm_ctrl.write(dc);
}

void car_stop() {
    in1 = 0;
    in2 = 0;
    pwm_ctrl.write(0.0f);
}

int main()
{
    pwm_ctrl.period_ms(1); // 1kHz Frequency

    bool active = false;

    while (true) {
        float distance = get_range();

        if (distance < 0.0f) {
            active = false;
        } else if (distance > LIMIT_HIGH) {
            active = true;
        } else if (distance < LIMIT_LOW) {
            active = false;
        }

        if (active) {
            car_forward(CAR_SPEED);
            car_led = 1;
        } else {
            car_stop();
            car_led = 0;
        }

        ThisThread::sleep_for(55ms); // Slightly modified loop timing
    }
}