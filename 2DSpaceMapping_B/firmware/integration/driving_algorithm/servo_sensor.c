/**
 * @file servo_sensor.c
 * @brief File containing function definitions for servo_sensor module.
 * This file contains function definitions of functions that are used
 * for controlling the servo_sensor component of the vehicle, and also some
 * variables needed in that process. There are functions for for initializing
 * servo_sensor, that is setting PWM pins, and values on that pins, and alse
 * setting appropriate pin to analog for taking the input from sensor and 
 * AD convertor. Also there is function for scanning the space with sensor.
 * This implies setting servo in several positions and taking the value of
 * the IR Distance sensor attached to servo in that position, and saving it 
 * in some variables.
*/

#include "servo_sensor.h"

/** Number of servo-sensor positions, that is how many different PWMs are */
#define SENSOR_POSITIONS 10
/** Number of servo-sensor front positions*/
#define FRONT_POSITIONS 3

/**
 * @name PWM variables
 * Variables for setting PWM to different values, and thus controll servo-sensor
 * @{
*/
unsigned pwm_period = 0;
float    duty_cycle = 0;
float    duty_cycles[] = {2.0, 2.9, 4.9, 5.8, 7.9, 8.8, 10.5, 11.4, 12.1, 13.5};
float    front_duty_cycles[] = {6.0, 7.5, 9.0};
/** @} */


/** Array that store sensor values in each position of the sensor*/
unsigned sensor_values[SENSOR_POSITIONS] = {0};

/** Array that store sensor values after processing, each values in this array 
    is associated with one driving direction */
unsigned sensor_values_processed[DIRECTIONS] = {0};

/** Values to store 3 measurements in each sensor positions */
unsigned sensor_value_1, sensor_value_2, sensor_value_3 = 0;

/** Array that store sensor values in front positions of the servo_sensor */
unsigned front_reading[FRONT_POSITIONS] = {0};


/*
 * Function for initializing servo-sensor. First it initialies ADC 10-bit module, then sets pin
 * RB14 to work in open drain configuration, remapping pin RP14 to OUTPUT_COMPARE module.
 * Setting PWM to 50 Hz, channel 1, no prescaler, timer2, and then starting the PWM.
*/
void initialize_servo_sensor()
{	
	/* Init ADC to use 10-bit module, and use Internal voltage reference(AVDD, AVSS) */
	ADC1_Init_Advanced(_ADC_INTERNAL_REF); 
	/* Set pin RB14 to work in open drain configuration */
    ODCB.F14 = 1;   							
	/* Remapping pin RP14 to OUTPUT_COMPARE module for PWM*/
    RPOR7 = 0x0012;
	/* Set 50Hz period, channel 1, no prescaler, timer 2 */
    pwm_period = PWM_Init(50, 1, 0, 2);   
    PWM_Start(1);
}


/*
 * Function for setting servo-sensor to starting position
*/
void set_servo_sensor_to_starting_pos()
{
	/* starting position is for duty_cycle 7.5 */
	duty_cycle = 7.5;
    PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
}


/*
 * Function for scaning the space with servo-sensor
*/
void scan_space_with_sensor()
{
    unsigned i = 0;
    
    /* Clear old values in storing variables and arrays */
    sensor_value_1 = 0;
    sensor_value_2 = 0;
    sensor_value_3 = 0;
    
    for(i = 0; i < 3; i++)
    {
    	front_reading[i] = 0;
    }
   
    for(i = 0; i < 10; i++)
    {
       	sensor_values[i] = 0;
    }
    
    for(i = 0; i < 5; i++)
    {
       	sensor_values_processed[i] = 0;
    }
    
    /* Get sensor values, 3 measurements are taken in each position, and there average value is store into array */
    for(i = 0; i < SENSOR_POSITIONS; i++)
    {
		duty_cycle = duty_cycles[i];
		PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
		Delay_ms(50);

		sensor_value_1 = ADC1_Get_Sample(9);
		Delay_ms(35);
		sensor_value_2 = ADC1_Get_Sample(9);
		Delay_ms(35);
		sensor_value_3 = ADC1_Get_Sample(9);
		Delay_ms(35);

		sensor_values[i] = (sensor_value_1 + sensor_value_2 + sensor_value_3) / 3;
    }

	/* Filtering sensor readings from all sensor positions, into an array of values that length 
	   corresponds with number of directions vehicle can go */
    for(i = 0; i < DIRECTIONS; i++)
    {
        sensor_values_processed[i] += sensor_values[i * 2];
        sensor_values_processed[i] += sensor_values[i * 2] + 1;
    }
    
    Delay_ms(50);
    
    /* Get values of the sensor in fron positions  */
    for(i = 0; i < FRONT_POSITIONS; i++)
    {
        duty_cycle = front_duty_cycles[i];
        PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
        Delay_ms(200);
        front_reading[i] = ADC1_Get_Sample(9);
        Delay_ms(35);
    }

    /* Get servo-sensor back to starting position */
    set_servo_sensor_to_starting_pos();
    Delay_ms(75);
}
