# SysTick-Timer-and-interrupts
Development of a counter/timer utilizing SysTick and Interrupts

This code is a program for a timer that runs on an MSP432 microcontroller. The program initializes some global variables and the ports used for the LEDs and buttons. The program uses interrupts to handle button presses and update the timer. The timer is set up using the SysTick timer, which generates periodic interrupts that update the timer display and decrement the time remaining.

The program has functions to start, stop, reset, and set the timer. The timer can also blink the LEDs when it runs out. The main loop of the program waits for button presses and updates the timer when setting is enabled.
The program has the following components:

•	Global variables: The program initializes some global variables including Seconds, Minutes, Set_On, On, Blink, and BlinkCount. These variables store the current values of the timer, the state of the timer (whether it's on or off), and the state of the LEDs (whether they're blinking or not).

•	Initialization of ports: The program initializes the ports used for the LEDs and buttons. Port 1 is used for the buttons, while ports 2 and 4 are used for the LEDs.

•	SysTick timer: The program configures the SysTick timer to generate interrupts every 3,000,000 clock cycles (which is approximately 1 second). The timer interrupt handler updates the timer display and decrements the time remaining.

•	Functions for starting, stopping, resetting, and setting the timer: The program defines functions for starting, stopping, resetting, and setting the timer. These functions are called when the corresponding buttons are pressed.

•	Blinking LEDs: When the timer runs out, the program sets the Blink variable to 1, which causes the LEDs to blink. The BlinkLeds function is called every time SysTick_Handler is executed to control the blinking of the LEDs.

•	Main loop: The main loop of the program waits for button presses and updates the timer when setting is enabled. If the start button is pressed, the Start function is called to start the timer. If the stop button is pressed, the Stop function is called to stop the timer. If the set button is pressed, the Set function is called to allow the user to set the timer. If the reset button is pressed, the Reset function is called to reset the timer to zero.
