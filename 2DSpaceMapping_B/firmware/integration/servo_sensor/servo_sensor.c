/**
 * @file servo_sensor.c
 * @brief File with code that controlls servo-sensor component.
 
 * This file is a step into integration of the components of the system. 
 * It has a code that controlls servo-sensor. What is does is the following:
 *  - it initialies PWM module of microcontroller, 
 *  - sets appropriate pins, 
 *  - sets one pin to be analog pin, and sets AD convertor to convert sersnor
 *    analog value to a digital one which will be used in program, 
 *  - defines a functions which can scan the space by moving the servo around, 
 *    and in each position it takes sensor value. 
*/


/**
 * @name PWM variables
 * Variables for setting PWM to different values, and thus controll servo-sensor.
 * Declaring duty cycles and pwm_period variables which will be used for servo_sensor scanning
 * @{
*/

/** Declaring duty cycles which will be used for servo_sensor scanning */
unsigned pwm_period = 0;
float    duty_cycle = 0;
float    duty_cycles[] = {2.0, 4.7, 7.5, 10.2, 13.0};

/* Sensor reading value */
unsigned sensor_reading = 0;
/* Value of the sensor at which the car should stop */
const unsigned STOP_RANGE = 300;


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
 * Function that moves servo_sensor 180 degrees and stores sensor reading from different positions 
 * Moving the servo is done in for loop, by changing PWM duty cycles with some delay because
 * of the limits of servo motor. 
*/
void scan_space_with_sensor()
{
    unsigned i = 0;
    for(i = 0; i < 5; i++)
    {
    	duty_cycle = duty_cycles[i];
      	PWM_Set_Duty(duty_cycle * pwm_period / 100, 1);
      	Delay_ms(600);

    }

    /** always return sensor to face front */
    set_servo_sensor_to_starting_pos();
    Delay_ms(600);
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
	initialize_ADC();
	Delay_ms(100);
	initialize_servo_for_sensor();
	Delay_ms(100);
	set_servo_sensor_to_starting_pos();
	Delay_ms(600);


	while(1)
	{    
		/* servo_sensor stays in its initial position until obtacle within predefined STOP_RANGE is observed 
	 	, after that servo_sensor scans the space around it*/
		sensor_reading = ADC1_Get_Sample(9);
		if(sensor_reading >= STOP_RANGE){
			scan_space_with_sensor();
		}
	}
}
