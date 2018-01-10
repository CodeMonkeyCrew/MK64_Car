#include <msp430.h>
#include "timer_pwm.h"
#include "color_sensor.h"
#include "spi.h"
#include "util.h"

#define DEFAULT_STEERING_VALUE 127 // center steering
#define DEFAULT_ENGINE_VALUE 127 // turn off engine

unsigned char steeringValue = DEFAULT_STEERING_VALUE;
unsigned char engineValue = DEFAULT_ENGINE_VALUE;

// measured average values for black and white
// row index = color, row[0] = min, row[1] = max
static unsigned int color_ranges[NUMBER_OF_COLORS][2] = {
    { 2980, 200 }, // red
    { 2690, 200 }, // blue
    { 940, 85 }, // clear
    { 3300, 200 } // green
};

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // set system clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ; // set range
    DCOCTL = CALDCO_16MHZ; // set DCO step and modulation

    timer_pwm_init_steering(steeringValue);
    timer_pwm_init_engine(engineValue);
    timer_pwm_start();

    color_sensor_init();

    spi_init();

    __enable_interrupt();

    unsigned int r, g, b, c;
    while (1)
    {
        r = map(color_values[red], color_ranges[red][0], color_ranges[red][1], 0, 255);
        g = map(color_values[green], color_ranges[green][0], color_ranges[green][1], 0, 255);
        b = map(color_values[blue], color_ranges[blue][0], color_ranges[blue][1], 0, 255);
        c = map(color_values[clear], color_ranges[clear][0], color_ranges[clear][1], 0, 255);
        spi_send(r, g, b, c);
        timer_pwm_set_steering(steeringValue);
        timer_pwm_set_engine(engineValue);
    }
}
