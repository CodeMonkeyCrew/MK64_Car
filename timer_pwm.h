#ifndef TIMER_PWM__H
#define TIMER_PWM__H

#include <inttypes.h>

void timer_pwm_init_steering(uint8_t value);
void timer_pwm_init_engine(uint8_t value);
void timer_pwm_start(void);
void timer_pwm_set_steering(uint8_t value);
void timer_pwm_set_engine(uint8_t value);

#endif
