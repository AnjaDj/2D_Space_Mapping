/**
 * @file distance_sensor_test.c
 * @author 2DSpaceMapping_C
 * @date 21 April 2024
 * @brief File containing source code for testing distance sensor.
 *
 * This file tests correctess of the distance sensor.
 * Set one pin as analog and read the data from sensor as unsigned.
 * If the value is greater than threshold - the obstacle is detected - turn LED ON
 * @see https://github.com/codeandrelax/2DSpaceMapping_C/wiki/Određivanje-udaljenosti-auta-od-predmeta
 */

/**
 * @brief Variables for reading values from analog input 
 */
unsigned value;
float real_value;

float Vref = 3.3;
float delta_V;
unsigned no_of_quantums;


void main(void)
{

  /* set all analog pins to digital except pin AN10 ( RB14 ) */
  AD1PCFG = 0xFBFF;
  
  /* set pin AN10 ( RB14 ) as input */
  TRISB.F14 = 1;          
  
  /* Set pin RB13 as output and initializing it with zero for LED */
  TRISB.F13 = 0;
  LATB.F13  = 0;
   
  /* Init ADC to use 10-bit module, and use Internal voltage reference(from AVDD to AVSS) */ 
  ADC1_Init_Advanced(_ADC_INTERNAL_REF);

  /* 10-bit module - number of possible values is 2^10 = 1023 */
  no_of_quantums = 1023;      
  
  /* delta_V is quantum of input voltage */
  delta_V = Vref / no_of_quantums;               

  while(1){
    /* get the value from analog input, channel 10 (AN10) */
    value = ADC1_Get_Sample(10);      
                      
    /* calculate the value of the input voltage */
    real_value =  value * Vref / no_of_quantums;

    /* setting threshold */
    if(real_value > 1.7){
      LATB.F13 = 1;        // LED turn on
    } else {
      LATB.F13 = 0;        // LED turn off
    }

    Delay_ms(100);
  }
}
