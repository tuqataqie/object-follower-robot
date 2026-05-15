#include "mbed.h"
#include "motor.h"
#include "ultrasonic.h"

// تعريف الدالة اللي في startup.cpp عشان الـ Compiler يشوفها
void system_init();

static DigitalOut car_led(LED1);
static constexpr float LIMIT_HIGH = 25.0f;
static constexpr float LIMIT_LOW  = 15.0f;
static constexpr float CAR_SPEED  = 0.35f;

int main() {
    system_init(); // تهيئة الموتور والـ Interrupts معاً
    bool active = false;

    while (true) {
        float distance = get_range();
        if (distance < 0.0f) active = false;
        else if (distance > LIMIT_HIGH) active = true;
        else if (distance < LIMIT_LOW) active = false;

        if (active) {
            car_forward(CAR_SPEED);
            car_led = 1;
        } else {
            car_stop();
            car_led = 0;
        }
        ThisThread::sleep_for(55ms);
    }
}