/**
 * @file led_blinking.c
 *
 * @brief File containing code for testing microcontroller.
 *
 * This file contains testing code for microcontroller of
 * the system. It is a simple code that just sets all pins
 * of the MCU as digital, and initializes one pin as an
 * output pin, and sets it to zero. On that pin the LED will
 * be connected. After that in a while loop LED is turned on
 * and off periodically. 
 * The purpose of the code is to test that the MCU is capable
 * of handling basic code, and that it can be used for following
 * work.
*/


/**
 * @ Main function, entry point
 * 
 * In the main function all the job is done, the pins of the MCU are set
 * and configured, and LED is turned on and off in while loop.
 * 
*/
void main() {


     AD1PCFG = 0xFFFF; // Set All pins as digital
     TRISB.F15 = 0;   // Initialize pin RB15 as output
     LATB.F15 = 0;   // Set pin RB15 to zero

     while(1) {
        LATB.F15 = ~LATB.F15; // Invert RB15 value
        Delay_ms(1000);
     }

}
