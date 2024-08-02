/**
 * @file car_drive.h
 * @brief File containing function declarations for vehicle moving.
 * All function definitions of this module is in car_drive.c.
*/

#ifndef CAR_DRIVE
#define CAR_DRIVE

/**
 * @brief Function for initializing necessary things for driving.
 *
 * This function initializes I2C interface that is needed for configuring
 * the I2C driver on the vehicles driver board. It also then configures
 * I2C driver in way that driving of the vehicle is enabled.
*/
void initialize_car_drive();


/**
 * @brief Function for driving the car forward.
 *
 * This function is used when the desired direction of the car is forward. The
 * will be simply just go forward with desired speed that is set with macro SPEED,
 * defined in car_drive.c file.
*/
void drive_forward();


/**
 * @brief Function for driving the car backwards.
 *
 * This function is used when the desired direction of the car is backwards. 
 * This is the special possible direction of the car, it is used in special cases when the
 * car is too close to the obstacle in front.
*/
void drive_reverse();


/**
 * @brief Function for stopping the car
 *
 * This function is used when the car needs to stop.
*/
void stop();


/**
 * @brief Function for driving the car left.
 *
 * This function is used when the desired direction of the car is 90 degrees left,
 * that is the direction that is exactly on the left side of the current vehicle position.
 * This directions is one of the five possible directions of the vehicle 
 * (besides the reverse)
*/
void drive_full_left();

/**
 * @brief Function for driving the car left.
 *
 * This function is used when the desired direction of the car is 45 degrees left,
 * that is directions between full left and forward. This directions is one of the
 * five possible directions of the vehicle (besides the reverse).
*/
void drive_small_left();

/**
 * @brief Function for driving the car right.
 *
 * This function is used when the desired direction of the car is 90 degrees right,
 * that is the direction that is exactly on the right side of the current vehicle position.
 * This directions is one of the five possible directions of the vehicle
 * (besides the reverse)
*/
void drive_full_right();


/**
 * @brief Function for driving the car right.
 *
 * This function is used when the desired direction of the car is 45 degrees right,
 * that is directions between full right and forward. This directions is one of the
 * five possible directions of the vehicle (besides the reverse).
*/
void drive_small_right();

#endif /* CAR_DRIVE.H */
