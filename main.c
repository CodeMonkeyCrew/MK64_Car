#include <msp430.h>
#include "timer_pwm.h"
#include "color_sensor.h"
#include "spi.h"
#include "util.h"

#define DEFAULT_STEERING_VALUE 127 // center steering
#define DEFAULT_ENGINE_VALUE 127 // turn off engine
#define MIN_STEERING 30
#define MAX_STEERING 225

unsigned char steeringValue = DEFAULT_STEERING_VALUE;
unsigned char engineValue = DEFAULT_ENGINE_VALUE;

// measured average values for black and white
// row index = color, row[0] = min, row[1] = max
unsigned int color_ranges[NUMBER_OF_COLORS][2] = {
    { 925, 127 }, // red
    { 853, 127 }, // blue
    { 293, 53 }, // clear
    { 1060, 127 } // green
};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // set system clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ; // set range
    DCOCTL = CALDCO_16MHZ; // set DCO step and modulation

    // select P1.3 (GPIO) I/O function
    P1SEL &= ~BIT3;
    P1SEL2 &= ~BIT3;

    // setup P1.3 as input
    P1DIR &= ~BIT3;

    // select pullup resistor
    P1OUT |= BIT3;

    // enable pullup resistor
    P1REN |= BIT3;

    // interrupt on high-to-low transition
    P1IES |= BIT3;

    // clear pending interrupts
    P1IFG &= ~BIT3;

    // enable interrupt on P1.3
    P1IE |= BIT3;

    timer_pwm_init_steering(steeringValue);
    timer_pwm_init_engine(engineValue);
    timer_pwm_start();

    color_sensor_init();

    spi_init();

    __enable_interrupt();

    unsigned int r, g, b, c, steering, engine;
    while (1)
    {
        r = map(color_values[red], color_ranges[red][0], color_ranges[red][1], 0, 255);
        g = map(color_values[green], color_ranges[green][0], color_ranges[green][1], 0, 255);
        b = map(color_values[blue], color_ranges[blue][0], color_ranges[blue][1], 0, 255);
        c = map(color_values[clear], color_ranges[clear][0], color_ranges[clear][1], 0, 255);
        spi_send(r, g, b, c);
        steering = steeringValue;
        engine = engineValue;
        if (steering < MIN_STEERING)
        {
            timer_pwm_set_steering(MIN_STEERING);
        }
        else if (steering > MAX_STEERING)
        {
            timer_pwm_set_steering(MAX_STEERING);
        }
        else
        {
            timer_pwm_set_steering(steering);
        }
        timer_pwm_set_engine(engine);
    }
}
