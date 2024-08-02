/**
 * @file main_program.c
 * @brief Main file of the program.
 
 * This file contains main function of the program that serves as an
 * entry point. It controls all the behavious of the complete program.
 * It initialies all the necesseary components of the system, and then
 * using the functions from other modueles, does all the work.
*/


#include "car_drive.h"
#include "servo_sensor.h"

/**
 * @brief Variable to store sensor value.
 *
 * Varible that saves the value of the sensor when the vehicle is going straight.
 */
unsigned sensor_reading = 0;

/**
 * @brief Array in which fron sensor values are store.
 *
 * Array that saves the values of the sensor when the vehicle 
 * stopped, scanned the space for further moving and is scanning
 * in front of itself for possible obstacle that is to close
 * 
 */
unsigned extern front_reading[];

/**
 * @brief Array in which sensor values are store.
 *
 * Array that saves the values of the sensor when the vehicle is stopped and is
 * scanning  space for further moving
 * 
 */
unsigned extern sensor_values_processed[];


/**
 * @brief Function that determines direction of the car after scaning the space.
 *
 * This function is called after vehicle encounters an obstacle, stops its
 * moving, and scans the space. Based on the values in sensor_values[] array
 * the vehicle determines which of the five directions possible it is going
 * to take.
 * @param None
 * @return void
*/
void where_to_go(){
     int i = 0;
     int lowest_reading = sensor_values_processed[0];
     int index = 0;

	 /* Check the front reading for potential reverse driving */
    if(front_reading[0] >= 400 || front_reading[1] >= 400 || front_reading[2] >= 400)
    {
        drive_reverse();
        Delay_ms(350);
        stop();
     }

	 /* Find the lowest values in sensor readings for each direction */
     for(i = 0; i < DIRECTIONS; i++)
     {
         if(sensor_values_processed[i] < lowest_reading)
         {
             lowest_reading = sensor_values_processed[i];
             index = i;
         }
     }

     /* Determine the directions to go */
     switch(index)
     {
         case 0:
           drive_full_left();
           break;
         case 1:
           drive_small_left();
           break;
         case 2:
           drive_forward();
           break;
         case 3:
           drive_small_right();
           break;
         case 4:
           drive_full_right();
           break;
         default:
           stop();
           break;
     }

}


/**
 * @brief Main function of the program, entry point.
 *
 * This function is an entry point of the program. It does all neccesary configuration of all
 * the modules of the program (MCU, servo_sensor, car_drive etc.)
 * In the infinite while loop program is continously driving the vehicle, stops at obstacles
 * scans the space when needed etc.
*/
void main()
{
    
        unsigned i = 0;

        AD1PCFG = 0xFDFF;        // set all pins to digital except pin AN9(RB15)
        LATB = 0;
        TRISB = 0;
        TRISB.F15 = 1;          // set pin AN9(RB15) as input

		/* Initialization of needed components */
        initialize_car_drive();
        Delay_ms(100);
        initialize_servo_sensor();
        Delay_ms(100);
        set_servo_sensor_to_starting_pos();
        Delay_ms(300);

		/* At the start vehicle just drives forward */
        drive_forward();
        while(1)
        {
        		/* Drive forward until obstacle, get sensor reading and decide where to go */
                sensor_reading = ADC1_Get_Sample(9);
                if(sensor_reading >= 300)
                {
                        stop();
                        scan_space_with_sensor();
                        where_to_go();
                 }
        }
}
