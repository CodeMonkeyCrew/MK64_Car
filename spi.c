#include <msp430.h>
#include "spi.h"
#include "timer_pwm.h"

#define SEND_R 0
#define SEND_G 1
#define SEND_B 2
#define SEND_C 3
#define RECEIVE_STEERING 0
#define RECEIVE_ENGINE 1
#define DEFAULT_SEND_VALUE 0
#define FRAME_SIZE 4

extern unsigned char steeringValue;
extern unsigned char engineValue;

static int sendCursor = -1;
static int receiveCursor = -1;

static unsigned char rValue = DEFAULT_SEND_VALUE;
static unsigned char gValue = DEFAULT_SEND_VALUE;
static unsigned char bValue = DEFAULT_SEND_VALUE;
static unsigned char cValue = DEFAULT_SEND_VALUE;

void spi_init(void)
{
    // select peripheral module function for P1.5 (CLK), P1.6 (SOMI) and P1.7 (SIMO)
    P1SEL |= BIT5 + BIT6 + BIT7;
    P1SEL2 |= BIT5 + BIT6 + BIT7;

    // select I/O function for P1.4 (STE)
    P1SEL &= ~BIT4;
    P1SEL2 &= ~BIT4;

    // setup P1.4 (STE) as input
    P1DIR &= ~BIT4;

    // interrupt on low-to-high transition
    P1IES &= ~BIT4;

    // enable interrupts on P1.4 (STE)
    P1IE |= BIT4;
}

void spi_send(unsigned char newRValue, unsigned char newGValue, unsigned char newBValue, unsigned char newCValue)
{
    rValue = newRValue;
    gValue = newGValue;
    bValue = newBValue;
    cValue = newCValue;
}

#pragma vector=PORT1_VECTOR
__interrupt void port1_isr(void)
{
    if (P1IFG & BIT4)
    {
        // clear P1.4 (STE) interrupt flag
        P1IFG &= ~BIT4;

        // slave ACTIVE -> INACTIVE transition

        // reset USCI_B0
        UCB0CTL1 = UCSWRST;

        // synchronous mode, 3-pin SPI, slave mode, MSB first
        UCB0CTL0 = UCSYNC + UCMODE_0 + UCMSB;

        // release USCI_B0 for operation
        UCB0CTL1 &= ~UCSWRST;

        // enable rx / tx interrupt
        IE2 |= UCB0RXIE + UCB0TXIE;

        // prepare tx buffer for first transmission
        UCB0TXBUF = rValue; // start with transmitting r value

        // update send / receive cursor
        sendCursor = SEND_G;
        receiveCursor = RECEIVE_STEERING;
    }
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void spi_tx_isr(void)
{
    if (IFG2 & UCB0TXIFG)
    {
        // send data
        switch (sendCursor)
        {
        case SEND_R:
            UCB0TXBUF = rValue;
            break;
        case SEND_G:
            UCB0TXBUF = gValue;
            break;
        case SEND_B:
            UCB0TXBUF = bValue;
            break;
        case SEND_C:
            UCB0TXBUF = cValue;
            break;
        default:
            UCB0TXBUF = DEFAULT_SEND_VALUE;
            break;
        }

        // update send cursor
        if (sendCursor >= 0)
        {
            if (++sendCursor >= FRAME_SIZE)
            {
                sendCursor = 0;
            }
        }
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void spi_rx_isr(void)
{
    if (IFG2 & UCB0RXIFG)
    {
        // receive data
        unsigned char data = UCB0RXBUF;
        switch (receiveCursor)
        {
        case RECEIVE_STEERING:
            steeringValue = data;
            break;
        case RECEIVE_ENGINE:
            engineValue = data;
            break;
        default:
            // ignore other bytes
            break;
        }

        // update receive cursor
        if (receiveCursor >= 0)
        {
            if (++receiveCursor >= FRAME_SIZE)
            {
                receiveCursor = 0;
            }
        }
    }
}
