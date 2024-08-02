#define DEVICE_ADDR 0xE0
const unsigned threshold_distance=300;
unsigned pwm,value;
unsigned j;
float no_of_quantums=1023;
float duty_cycles[]= {2.0, 4.0, 6.0, 7.5, 9.0, 11.0, 13.5};
unsigned sensor_values[]={0,0,0,0,0,0,0};
//unsigned close_values[]={390,500,540,590,540,500,390};      // vrijednosti kad je blizu
//float real_values[]={0,0,0,0,0,0,0};
float duty_cycle;
float Vref=3.3;



sbit Soft_I2C_Scl           at RB8_bit;
sbit Soft_I2C_Sda           at RB9_bit;
sbit Soft_I2C_Scl_Direction at TRISB8_bit;
sbit Soft_I2C_Sda_Direction at TRISB9_bit;

void initialize_ADC(){
     ADC1_Init_Advanced(_ADC_INTERNAL_REF);
}
void initialize_PWM(){
    ODCB.F2=1;    //open drain


    Unlock_IOLOCK();
    PPS_Mapping_NoLock(2, _OUTPUT, 18);
    Lock_IOLOCK();

    pwm=PWM_Init(50, 1, 0, 2);
    PWM_Start(1);
}

void I2C_Send_Message(unsigned short dev_addr, unsigned short reg_addr, unsigned short data_value)
{
     Soft_I2C_Start();               // Issue start signal
     Soft_I2C_Write(dev_addr);       // Address the device
     Soft_I2C_Write(reg_addr);       // Start from address reg_addr
     Soft_I2C_Write(data_value);     // Set value
     Soft_I2C_Stop();                // Issue stop signal
}
void initialize_I2C(){

      Soft_I2C_Init();
      Delay_ms(500);
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

     Delay_ms(500);
}


void forward(unsigned pwmValue){
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, pwmValue);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x05, pwmValue);
}
void turn(unsigned pwmValue1, unsigned pwmValue2){
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, pwmValue1);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x05, pwmValue2);
}
/*void left(unsigned pwmValue1, unsigned pwmValue2){
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, pwmValue1);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x05, pwmValue2);
}
      */
void backward(unsigned pwmValue){
     I2C_Send_Message(DEVICE_ADDR, 0x02, pwmValue);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x04, pwmValue);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 0);

}

void soft_brake(){
     I2C_Send_Message(DEVICE_ADDR, 0x02, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 255);
     delay_ms(500);

}
void hard_brake(){
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 0);
     delay_ms(500);

}
void back_to_center(){
    duty_cycle=7.5*pwm/100;
    PWM_Set_Duty(duty_cycle, 1);
}

void sensor_reading(){

    unsigned i;
    for (i=0; i<7; i++){
        duty_cycle=duty_cycles[i]*pwm/100;
        PWM_Set_Duty(duty_cycle, 1);
        sensor_values[i] = ADC1_Get_Sample(10);                        // get the value from analog input, channel 10 (AN10)
        delay_ms(100);
       }
      // back_to_center();
}


float closest_distance;
unsigned index;
void distance_reading(){
     unsigned i;
     closest_distance=sensor_values[3];
     index=3;
     for(i=0;i<7;i++){
         if((i % 6 == 0)) {
            if((sensor_values[i] + 150) < closest_distance){
              index = i;
              closest_distance = sensor_values[i];
            }
          }
          else if (i%6 ==1 || i%6==5){
            if((sensor_values[i] + 90) < closest_distance){
              index = i;
              closest_distance = sensor_values[i];
            }
          }
          else{
              if((sensor_values[i] + 50) < closest_distance){
                  index = i;
                  closest_distance = sensor_values[i];
              }
          }
     }

      if(closest_distance>= threshold_distance){
        hard_brake();
        backward(200);
        delay_ms(500);
        soft_brake();
        //UART1_Write_Text("Stopping !\r\n");
        //return turn;
      }
      //going();
      switch(index){
          case 0:
              turn(0,255);   //left
              delay_ms(50);
              soft_brake();
              break;
          case 1:
              turn(50,180);
              delay_ms(50);
              soft_brake();
              //delay_ms(200);
              break;
          case 2:
              turn(90,120);
              delay_ms(50);
              soft_brake();
              //delay_ms(200);
              break;
          case 3:

             if(sensor_values[0] < sensor_values[6]){
                 turn(0,250);
              delay_ms(50);
              soft_brake();
                 //delay_ms(200);
             }
             if(sensor_values[0] > sensor_values[6]){
                 turn(250,0);
              delay_ms(50);
              soft_brake();
                 //delay_ms(200);
             }
              break;
          case 4:
              turn(120,90);
              delay_ms(50);
              soft_brake();
              //delay_ms(200);
              break;
          case 5:
              turn(180,50);
              delay_ms(50);
              soft_brake();
              //delay_ms(200);
              break;
          case 6:
              turn(255,0);      //right
              delay_ms(50);
              soft_brake();
              //delay_ms(200);
              break;
          default:
              forward(100);
              break;
     }
}


int blind=0;
void blind_street(){
     unsigned i;
     for(i=0;i<7;i++){
         if(sensor_values[i]<350) blind=1;
     }
}
/*
void going(){
   switch(index){
    case 0:
         right(255,0);
         delay_ms(200);
         break;
    case 1:
         right(120,90);
         delay_ms(200);
         break;
    case 2:
         right(120,90);
         delay_ms(200);
         break;
    case 3:
         if(blind){
             backward(100);
             delay_ms(200);
             soft_brake();
             distance_reading();
         }
         else{
             if(sensor_values[0] < sensor_values[6]){
                 right(0,180);
                 delay_ms(200);
             }
             if(sensor_values[0] > sensor_values[6]){
                 left(0,180);
                 delay_ms(200);
             }
         }
         break;
    case 4:
         left(90,120);
         delay_ms(200);
         break;
    case 5:
         left(90,120);
         delay_ms(200);
         break;
    case 6:
         left(0,255);
         delay_ms(200);
         break;
    default:
         backward(150);
         break;
   }
}
 */
void main() {
     AD1PCFG = 0XFBFF;
     TRISB.F2 = 0; //SDA for servo output
     PORTB.F2 = 0;
     TRISB.F14=1;  //RB14 is input - MOSI

     initialize_PWM();
     delay_ms(100);
     initialize_ADC();
     delay_ms(100);
     initialize_I2C();
     delay_ms(100);
     back_to_center();
     delay_ms(300);

     while(1){
         forward(100);
         delay_ms(50);
         sensor_reading();
         distance_reading();
         //delay_ms(200);
         //back_to_center();
         /*for(j=0;j<7;j++){
           if(sensor_values[j]>close_values[j]){       //provjerava da li je preblizu
              hard_brake();
              going();
           }
         } */
         /*if(closest_distance>500){
             hard_brake();
             going();

         } */
           //real=sample*3.3/1023, real=1.5 => sample=465
           /*if(value>350){
               soft_brake();
               sensor_reading();
               distance_reading();
               going();
           } */

         delay_ms(100);
     }
}