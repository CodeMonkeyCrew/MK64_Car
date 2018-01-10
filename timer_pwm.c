#include <msp430.h>
#include "timer_pwm.h"
#include "util.h"

// timer PWM on P2.1 -> STEERING
// timer PWM on P2.4 -> ENGINE

void timer_pwm_init_steering(uint8_t value)
{
    // setup P2.1 (GPIO) as output
    P2DIR |= BIT1;

    // select P2.1 (GPIO) peripheral module function
    P2SEL |= BIT1;
    P2SEL2 &= ~BIT1;

    // equals 20 ms (50 Hz PWM frequency)
    TA1CCR0 = 40000; // (16 MHz, divider 8)

    timer_pwm_set_steering(value);
    // reset/set mode
    TA1CCTL1 = OUTMOD_7;
}

void timer_pwm_init_engine(uint8_t value)
{
    // setup P2.4 (GPIO) as output
    P2DIR |= BIT4;

    // select P2.4 (GPIO) peripheral module function
    P2SEL |= BIT4;
    P2SEL2 &= ~BIT4;

    // equals 20 ms (50 Hz PWM frequency)
    TA1CCR0 = 40000; // (16 MHz, divider 8)

    timer_pwm_set_engine(value);
    // reset/set mode
    TA1CCTL2 = OUTMOD_7;
}

void timer_pwm_start()
{
    // SMCLK (16 MHz), divider 8, up mode, timer clear
    TA1CTL = TASSEL_2 + ID_3 + MC_1 + TACLR;
}

void timer_pwm_set_steering(uint8_t value)
{
    int ticksHigh = map(value, 0, 255, 2000, 4000); // (16 MHz, divider 8)
    TA1CCR1 = ticksHigh;
}

void timer_pwm_set_engine(uint8_t value)
{
    int ticksHigh = map(value, 0, 255, 2000, 4000); // (16 MHz, divider 8)
    TA1CCR2 = ticksHigh;
}
