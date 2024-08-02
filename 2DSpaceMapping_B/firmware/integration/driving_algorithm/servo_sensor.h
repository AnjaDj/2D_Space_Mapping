/**
 * @file servo_sensor.h
 * @brief File containing function declarations for servo_sensor module.
 * This file contains function declarations of functions that are used
 * for controlling the servo_sensor component of the vehicle.
*/


#ifndef SERVO_SENSOR
#define SERVO_SENSOR

#define DIRECTIONS 5

/**
 * @brief Function for initializing servo-sensor module.
 *
 * This function is used for initialising servo-sensor module. It should be
 * called at the begining of the program,
 * before module is used in the rest of the program.
*/
void initialize_servo_sensor();


/**
 * @brief Function for scanning the space with servo-sensor module.
 *
 * This function is used for scanning the space with servo-sensor module.
 * After the vehicle is stopped because of obstacle, this function is called.
 * It scans the space around, takes the values, and saves them. Based on that
 * values, the vehicle decides in which direction it drives.
*/
void scan_space_with_sensor();

/**
 * @brief Function for setting servo-sensor module to starting position.
 *
 * This function sets servo-sensor to starting position, which is facing
 * straight forward. It is called each time after scanning the space, and
 * before moving forward.
*/
void set_servo_sensor_to_starting_pos();

#endif  /* SERVO_SENSOR */
