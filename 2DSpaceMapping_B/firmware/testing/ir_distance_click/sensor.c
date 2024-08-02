/**
 * @file sensor.c
 * @brief Testing of IR Distance Click sensor
 
 * This file contains code that is used as tester
 * for sensor used in the project. It takes sensor
 * reading as analog values and using AD convertor
 * converts it into some digital values that is then 
 * stored into some variable. It is then transformed
 * into float value that represesents voltage that 
 * sensor generates, according to sensor datasheet.
 * When some value is got (1.5 for about 10 cm )
 * LED diode connected to some pin is ON.
*/

/**
 * @name Variables for sensor reading
 * 
 * @{
*/
/** @brief Variable that takes initial sensor value */
unsigned value;
/** @brief Value of the expressed as voltage */
float 	 real_value;
/** @brief Referent value of the reading, needed for transformation */
float    Vref = 3.3;
/** @brief Quantum of input voltage */
float    delta_V;
/** @brief Number of all quantum based on ref voltage on quantum */
unsigned no_of_quantums;
/** @} */


/**
 * @brief Main function of the program, entry point.
 *
*/
void main() 
{

	/* set all pins to digital except pin AN9(RB15) */
	AD1PCFG = 0xFDFF;         
	TRISB.F15 = 1;          // set pin AN9(RB15) as input

	/* Set pin RB14 as output for LED, and set it to zero */
	TRISB.F14 = 0;   	  
	LATB.F14 = 0;  	  

 	/* Init ADC to use 10-bit module, and use Internal 
 	 voltage reference(AVDD, AVSS) */
	ADC1_Init_Advanced(_ADC_INTERNAL_REF);      

	/*
	 * Since we use 10-bit module, number of possible values is 2^10 = 1023
	 * delta_V is quantuom of input voltage
	*/
	no_of_quantums = 1023;                          
	delta_V = Vref / no_of_quantums;                

	while(1)
	{
		/*
		 * Get the value from analog input, channel 9 (pin AN9)
		 * Then calculate the value of input voltage
		*/
		value = ADC1_Get_Sample(9);                        
		real_value =  value * Vref / no_of_quantums;       
				        
		/* setting threshold */	    
		if(real_value > 1.5)
		{
		 	LATB.F14 = 1;        // LED turn on
		} else 
		{
			LATB.F14 = 0;        // LED turn off
		}

		Delay_ms(100);
	}
}
