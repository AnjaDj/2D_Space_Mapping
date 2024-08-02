/**
 * @file servo_motor.c
 * @author 2DSpaceMapping_C
 * @date 19 April 2024
 * @brief File containing source code for testing servo motor.
 *
 * This file tests correctess of the servo motor. Test cases are
 * moving in seven spots 0 to 180 degrees. Servo is controled via
 * PWM module set on channel 1.
 * @see https://github.com/codeandrelax/2DSpaceMapping_C/wiki/Servo-motor-control
 */

/**
 * @brief PWM parameters
 *
 * Variables that are crucial for PWM
 */
unsigned pwm;
unsigned frequency = 50; /* 50 Hz */
float left_2, left, left_1, center, right_1, right, right_2;


void main(void)
{

  /* Configure analog pins as digital */
  AD1PCFG = 0XFFFF;

  /* Set RB2(RP2) as output */  
  TRISB = 0;
  
  /* Init start value RB2(RP2) as 0 */
  PORTB = 0;
  
  /**
  * RB2 is open-drain.
  * High value 5V - low value 0V
  */
  ODCB.F2 = 1;
     
  /* char PPS_Mapping_NoLock(char rp_num, char input_output, char funct_name);
  * rp_num    - remappable pin number
  * func_name - selects internal mcu module for usage ( 18 = OC1(Output Compare 1) )
  */
  Unlock_IOLOCK();
  PPS_Mapping_NoLock(2, _OUTPUT, 18);
  Lock_IOLOCK();
  
  /* Initialize PWM module with duty = 0
  * unsigned long freq_hz         - 50Hz
  * unsigned int enable_channel_x - channel 1
  * unsigned int timer_prescale   - no clock prescale
  * unsigned int use_timer_x      - Timer2
  * RETURNS calculated timer period
  */ 
  pwm=PWM_Init(frequency, 1, 0, 2);
  
  /* Starts PWM at requested channel 1*/
  PWM_Start(1);
     
  /* Calculating duty for 7 positions */   
  left_2  = 2.0  * pwm / 100;
  left    = 4.0  * pwm / 100;
  left_1  = 6.0  * pwm / 100;
  center  = 7.5  * pwm / 100;
  right_1 = 9.0  * pwm / 100;
  right   = 11.0 * pwm / 100;
  right_2 = 13.5 * pwm / 100;
     
  while(1){
    PWM_Set_Duty(left_2, 1);
    delay_ms(500);
              
    PWM_Set_Duty(left, 1);
    delay_ms(500);
              
    PWM_Set_Duty(left_1, 1);
    delay_ms(500);
              
    PWM_Set_Duty(center, 1);
    delay_ms(500);
              
    PWM_Set_Duty(right_1, 1);
    delay_ms(500);
              
    PWM_Set_Duty(right, 1);
    delay_ms(500);
              
    PWM_Set_Duty(right_2, 1);
    delay_ms(500);
              
    PWM_Set_Duty(right, 1);
    delay_ms(500);
              
    PWM_Set_Duty(right_1, 1);
    delay_ms(500);
              
    PWM_Set_Duty(center, 1);
    delay_ms(500);
              
    PWM_Set_Duty(left_1, 1);
    delay_ms(500);
              
    PWM_Set_Duty(left, 1);
    delay_ms(500);
     }
}
