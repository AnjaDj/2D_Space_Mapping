/**
 * @file drive_servo_sensor.c
 * @brief File with second stop of project integration.
 *
 * This file contains code that integrates driving the car module
 * with servo-sensor module. So this code does all the stuff that
 * is shown in integration/servo_sensor.c program, but also has the 
 * ability to drive the car around based on sensor reading.
 * Besides servo-sensor functions this code has:
 * - initialization of I2C for driving
 * - functions for driving forward, and braking the car
 * 
*/

/**
 * @name Macros used in program 
 * @{
*/
/** @brief Macro that defines speed of the vehicle (0-255) */ 
#define SPEED 125
/** @brief I2C address of the device */
#define DEVICE_ADDR 0xE0
/** @} */

/** @name Declaring duty cycles which will be used for servo_sensor scanning 
 * @{
*/
unsigned pwm_period = 0;
float    duty_cycle = 0;
float    duty_cycles[] = {2.0, 4.7, 7.5, 10.2, 13.0};
/** @} */

/**
 * @name Sensor reading values
 * @{
*/
unsigned sensor_reading = 0;
unsigned sensor_values[5] = {0};
/** @} */

/**
 * @name Enable Software I2C communication, through the desired pins
 * @{
 
*/
sbit Soft_I2C_Sda           at RB2_bit;
sbit Soft_I2C_Scl           at RB3_bit;
sbit Soft_I2C_Sda_Direction at TRISB2_bit;
sbit Soft_I2C_Scl_Direction at TRISB3_bit;
/** @} */

/**  
 * Function that initializes servo_sensor, by setting a PWM pin, and configures PWM.
*/
void initialize_servo_for_sensor()
{
    
    /** set pin RB14 to work in open drain configuration */
    ODCB.F14 = 1;   

    /** set up PWM pin, RB(P)14  */
    Unlock_IOLOCK();
    PPS_Mapping_NoLock(14, _OUTPUT, 18);     
    Lock_IOLOCK();

    /** get PWM period for duty cycle count */
    /** set 50Hz period, channel 1, no prescaler, timer 2 */
    pwm_period = PWM_Init(50, 1, 0, 2);  
    PWM_Start(1);
}


/**
 * Functions that sets servo to its starting position,
 * by setting appropriate duty cycle of the PWM
*/
void set_servo_sensor_to_starting_pos()
{
    duty_cycle = 7.5;
    PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
}



/**
 * Function that initializes ADC needed for sensor reading 
*/
void initialize_ADC()
{
	/** Init ADC to use 10-bit module, and use Internal voltage reference(AVDD, AVSS) */
    ADC1_Init_Advanced(_ADC_INTERNAL_REF);       
}




/**
 * Function for I2C controller configuration. 
 * It addresses device, then desired register, and puts some value in it
*/
void I2C_Send_Message(unsigned short dev_addr, unsigned short reg_addr, unsigned short data_value)
{
    Soft_I2C_Start();
    Soft_I2C_Write(dev_addr);
    Soft_I2C_Write(reg_addr);
    Soft_I2C_Write(data_value);
    Soft_I2C_Stop();
}


/**
 * Function for I2C controller ,on the driver board, initialization.
 * What is does is that is addresses some registers of the PCA9633 I2C driver
 * ,according to the datasheet, and puts appropriate values in them, in order for
 * the car to be able to drive. 
*/
void initialize_I2C()
{
	Soft_I2C_Init();
	Delay_ms(100);
	 /* Set MODE1 register of PCA9633
	  * AIx                   - Auto-Increment disabled
	  * SLEEP bit set to 0    - Normal Mode
	  * SUBx bit set to 0     - PCA does not respond to subadresses
	  * ALLCALL bit set to 1  - PCA9633 responds to ALLCALL address (0xE0)
	 */
	 I2C_Send_Message(DEVICE_ADDR, 0x00, 0x01);
	 /* Set LEDOUT register of PCA9633
	  * LEDx output state control - all set to 0x10 which results in 0xAA value put into LEDOUT register
	 */
	 I2C_Send_Message(DEVICE_ADDR, 0xE8, 0xAA);
}


/**
 * Function for driving the car forward by setting PWM values and thus
 * controlling the DC motors on the wheels.
*/
void drive_forward()
{
    /* Set PWMx (x = 0, 1, 2, 3) registers to values according to datasheet in order for vehice to go forward*/
    I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x03, SPEED);
    I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x05, SPEED);
}


/**
 * Function for braking the wheels of the car, that are
 * effectively stopping the car. 
*/
void brake()
{
    I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x03, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
    I2C_Send_Message(DEVICE_ADDR, 0x05, 0);
}



/**
 * Function that moves servo_sensor 180 degrees and stores sensor reading from different positions 
 * Moving the servo is done in for loop, by changing PWM duty cycles with some delay because
 * of the limits of servo motor. 
*/
void scan_space_with_sensor()
{
    unsigned i = 0;
    /* Clear the old sensor values */
    for(i=0; i < 5; i++)
    {
       sensor_values[i] = 0;
    }

	/* Move the servo around with PWM, and take sensor readings */
    for(i = 0; i<5; i++) 
    {
    	duty_cycle = duty_cycles[i];
      	PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
      	sensor_values[i] = ADC1_Get_Sample(9);
      	Delay_ms(600);

    }

     /* always return sensor to face front */
     set_servo_sensor_to_starting_pos();
     Delay_ms(600);
}


/**
 * Main function, entry point of the program
*/
void main()
{

	unsigned i = 0;

	AD1PCFG = 0xFDFF;        // set all pins to digital except pin AN9(RB15)
	LATB = 0;
	TRISB = 0;
	TRISB.F15 = 1;          // set pin AN9(RB15) as input

	/* Initialization of the needed components */
	initialize_I2C();
	Delay_ms(100);
	initialize_ADC();
	Delay_ms(100);
	initialize_servo_for_sensor();
	Delay_ms(100);

	/* Set servo to defaul position that is to face front */
	set_servo_sensor_to_starting_pos();
	Delay_ms(600);


	drive_forward();

	while(1)
	{
		/* 
		 * The car drives forward, and servo_sensor stays in its initial position 
		 * until obtacle within predefined stop range of 300 is observed 
		 * ,after that the car stop and  servo_sensor scans the space around it*/
		sensor_reading = ADC1_Get_Sample(9);
		if(sensor_reading >= 300){
			brake();
		   	scan_space_with_sensor();
		}
	}
}
