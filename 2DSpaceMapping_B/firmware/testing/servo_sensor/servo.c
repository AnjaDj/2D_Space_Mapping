/**
 * @file servo.c
 * @brief Testing servo program.
 
 * This file contains testing program for servo module
 * of the system. It sets pin for PWM which is used for controlling
 * the servo. Then the PWM is initialized using mikroC libraries, 
 * and after that a couple of duty cycles are changed on sent
 * to PWM pin, in order for move the servo around, for about 180 degrees.
*/

/**
 * @name Variables for PWM
 *
 * Variable for pwm period
 * and for two duty cycles that will
 * change periodically 
 * @{
*/
/** @brief Macro that defines speed of the vehicle (0-255) */ 
unsigned pwm_period;
unsigned duty_cycle1;
unsigned duty_cycle2;
/** @} */


/*
 * @brief Main function, entry point of the program
 *
 * In the main function is the complete functionality of the 
 * program as described in the header of the file
*/
void main() 
{
	
	/*
	 * Set all analog pins as digital
	 * Set pins as output pins, and set them to zero
	*/
	AD1PCFG= 0xFFFF;  
	TRISB = 0;
	PORTB = 0;

	/* set pin RB11 to work in open drain configuration */
	ODCB.F11 = 1;  

	/* 
	 * Set up PWM pin
	 * pin RP11, RB11 is chosen
	*/
	Unlock_IOLOCK();
	PPS_Mapping_NoLock(11, _OUTPUT, 18);     
	Lock_IOLOCK();

	/* set 50Hz period, channel 1, no prescaler, timer 2 */
	pwm_period = PWM_Init(50, 1, 0, 2);   
	PWM_Start(1);

	// set duty cycle to turn on one side
	duty_cycle1 = 2.5 * pwm_period / 100; 
	// set duty cycle to turn to the other side   
	duty_cycle2 = 12.5 * pwm_period / 100;  

	 while(1)
	 {
		PWM_Set_Duty(duty_cycle1, 1);
		Delay_ms(2000);
		PWM_Set_Duty(duty_cycle2, 1);
		Delay_ms(2000);
	 }

}
