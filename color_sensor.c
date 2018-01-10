#include <msp430.h>
#include "color_sensor.h"

#define FALSE 0
#define TRUE !FALSE
#define NUMBER_OF_MEASUREMENTS 2

static Color_t current_color;

void color_sensor_init(void)
{
    // setup P1.3 (S0), P1.2 (S1)
    P1DIR |= BIT3 + BIT2;

    // setup P2.3 (S2) and P2.0 (S3) as output
    P2DIR |= BIT3 + BIT0;

    // setup P1.1 (OUT) as input
    P1DIR &= ~BIT1;

    // setup P1.0 (OE) as output
    P1DIR |= BIT0;

    // set frequency scaling to 100% (S0 to high, S1 to high)
    P1OUT |= BIT3 + BIT2;

    // set OE to low
    P1OUT &= ~BIT0;

    // select color
    current_color = red;

    if (current_color & BIT0)
    {
        // set S3 to high
        P2OUT |= BIT0;
    }
    else
    {
        // set S3 to low
        P2OUT &= ~BIT0;
    }
    if (current_color & BIT1)
    {
        // set S2 to high
        P2OUT |= BIT3;
    }
    else
    {
        // set S2 to low
        P2OUT &= ~BIT3;
    }

    // select peripheral module function (CCI0A) for P1.1 (OUT)
    P1SEL |= BIT1;
    P1SEL2 &= ~BIT1;

    // capture on rising edge, select CCIxA input, synchronize capture source, capture mode, capture/compare interrupt enable
    TA0CCTL0 = CM_1 + CCIS_0 + SCS + CAP + CCIE;

    // SMCLK, divider 0, continuous mode, timer_clear
    TA0CTL = TASSEL_2 + ID_0 + MC_2 + TACLR;
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void color_sensor_timer_isr(void)
{
    static int overflow = FALSE;
    static unsigned int previous_timer_value = 0;
    // check for timer overflow
    if (TA0CTL & TAIFG)
    {
        overflow = TRUE;
        // clear timer interrupt flag
        TA0CTL &= ~TAIFG;
    }
    else
    {
        static unsigned int counter = 0;
        // use second measurement only (allow sensor frequency to settle)
        if (++counter == NUMBER_OF_MEASUREMENTS)
        {
            unsigned int current_timer_value = TA0CCR0;
            unsigned int timer_ticks;
            if (overflow)
            {
                timer_ticks = current_timer_value + (0xFFFF - previous_timer_value);
                overflow = FALSE;
            }
            else
            {
                timer_ticks = current_timer_value - previous_timer_value;
            }

            color_values[current_color] = timer_ticks;
            counter = 0;

            // next color
            if (++current_color >= NUMBER_OF_COLORS)
            {
                current_color = red;
            }

            if (current_color & BIT0)
            {
                // set S3 to high
                P2OUT |= BIT0;
            }
            else
            {
                // set S3 to low
                P2OUT &= ~BIT0;
            }
            if (current_color & BIT1)
            {
                // set S2 to high
                P2OUT |= BIT3;
            }
            else
            {
                // set S2 to low
                P2OUT &= ~BIT3;
            }
        }

        // overwrite previous timer value
        previous_timer_value = TA0CCR0;
    }
}
