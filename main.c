#include "msp.h"

// Global variables for all functions of this code
int Seconds = 0;
int Minutes = 0;
int Set_On = 1;
int On = 0;
int Blink = 0;      // variable to control blinking of LEDs
int BlinkCount = 0; // variable to count the number of times LEDs have blinked

void Start()
{
    // Checks to see if there is any value within Seconds and Minutes to begin process
    if (Seconds > 0 || Minutes > 0) {
        Set_On = 0;                 // Turns off process that allows users to add more or less time
        On = 1;                     // Begins the counter/timer process
        P2->OUT |= 0x02;            // Turns on green LED to give visual signal that it should now start counting down
        P2->OUT &= ~0x05;           // Turns off blue and red LED
    }
}


void Stop()
{
    On = 0;
    P2->OUT |= 0x01;
    P2->OUT &= ~0x06;
}

void Set()                          //when this button is pressed, you are allowed to stop the counter at any point and add or subtract value to the counter
{
    Stop();                         //calls the stop function
    Set_On = 1;                     //turns on the area that allows value to be added to the timer
    P2->OUT |= 0x02;
    P2->OUT &= ~0x05;
}

void Reset() {
    Stop();
    Set_On = 1;
    Seconds = 0;
    Minutes = 0;
}

void Update()
{
    // Update the LEDs to show the current time
    P4->OUT = Seconds + (Minutes << 6);
}

void BlinkLeds()
{
    if(Blink == 1) // if Blink is set to 1, then blink the LEDs
    {
        if(BlinkCount % 2 == 0) // turn LEDs on every second time BlinkCount is even
        {
            P4->OUT = 0xFF; // turn on all LEDs
        }
        else // turn LEDs off every odd time BlinkCount is odd
        {
            P4->OUT = 0x00; // turn off all LEDs
        }

        BlinkCount++; // increment BlinkCount
    }
}

void SysTick_Handler(void)
{
    if(Set_On == 1)
    {
        P2->OUT |= 0x02;
    }

    if(On == 1)
    {
        P2->OUT |= 0x04;
        P2->OUT &= ~0x03;
        Seconds--;

        if(Seconds < 0)
        {
            if(Minutes > 0)
            {
                Seconds = 59;
                Minutes--;
            }
            else
            {
                Blink = 1;      // set Blink to 1 to start blinking LEDs
                BlinkCount = 0; // reset BlinkCount
                On = 0;
            }
        }

        Update();
    }
    BlinkLeds(); // call the BlinkLeds function every time SysTick_Handler is executed
}
    // Blink all LEDs when timer ends
void Blink_LEDS() {
    P4->OUT = 0xFF; // turn all LEDs on
    __delay_cycles(1000000); // wait for a brief period before turning off LED's
    P4->OUT = 0x00; // turn all LEDs off
    __delay_cycles(1000000); // wait for a brief period before turning on LED's
}


int main(void)
{
    // Stop the watchdog timer
    WDT_A->CTL = WDT_A_CTL_PW |
                 WDT_A_CTL_HOLD;

    // Configure port 1
    P1->DIR |= BIT0;
    P1->DIR &= ~(BIT1 | BIT4);
    P1->REN |= BIT1 | BIT4;
    P1->OUT |= BIT0 | BIT1 | BIT4;
    P1->OUT &= ~BIT0;

    // Configure port 2
    P2->DIR |= BIT0 | BIT1 | BIT2;
    P2->DIR &= ~(BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
    P2->REN |= BIT4 | BIT5 | BIT6 | BIT7;
    P2->OUT &= ~(BIT4 | BIT5 | BIT6 | BIT7);

    // Configure port 4
    P4->DIR |= 0xFF;
    P4->OUT &= ~0xFF;

    // Configure the SysTick timer
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk |
                    SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = 3000000 - 1;
    SysTick->VAL = 0x01;

    // Enable interrupts
    __enable_irq();


    while (1) {
        // Check buttons and perform actions accordingly
        if (P2->IN & BIT4) {
            Start();
        } else if (P2->IN & BIT5) {
            Stop();
        } else if (P2->IN & BIT6) {
            Set();
        } else if (P2->IN & BIT7) {
            Reset();
        }

        // Update the timer only when setting is enabled
        if (Set_On) {
            // Add or subtract seconds based on button presses
            if (!(P1->IN & BIT1)) {
                Seconds++;
            }
            if (!(P1->IN & BIT4)) {
                Seconds--;
            }

            // Handle overflow and underflow of seconds and minutes
            if (Seconds >= 60) {
                Seconds = 0;
                Minutes++;
            } else if (Seconds < 0) {
                Seconds = 59;
                Minutes--;
            }
            if (Minutes < 0) {
                Minutes = 3;

            Minutes = Minutes % 4;
        }

        Update();
        __delay_cycles(5); // used to determine the rate at which the instructions will execute to transition between LED's
    }
}
}
