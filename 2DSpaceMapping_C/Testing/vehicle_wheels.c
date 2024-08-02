/**
 * @file vehicle_wheels.c
 * @author 2DSpaceMapping_C
 * @date 18 April 2024
 * @brief File containing source code for testing wheels.
 *
 * This file tests correctess of the vehicle wheels. Test cases are
 * moving forward, moving backward, circling and braking. Comunnication
 * protocol used for wheels control is I2C.
 * @see https://github.com/codeandrelax/2DSpaceMapping_C/tree/software_branch
 */

#define DEVICE_ADDR 0xE0
/**
 * @brief Software I2C connections
 *
 * Variables that must be defined in all projects using Software_I2C library
 */
sbit Soft_I2C_Scl           at RB8_bit;
sbit Soft_I2C_Sda           at RB9_bit;
sbit Soft_I2C_Scl_Direction at TRISB8_bit;
sbit Soft_I2C_Sda_Direction at TRISB9_bit;

/**
 * @brief Function that writes data_value to the address red_addr of the addressed device
 */
void I2C_Send_Message(unsigned short dev_addr, unsigned short reg_addr, unsigned short data_value)
{
     Soft_I2C_Start();               // Issue start signal
     Soft_I2C_Write(dev_addr);       // Address the device
     Soft_I2C_Write(reg_addr);       // Start from address reg_addr
     Soft_I2C_Write(data_value);     // Set value
     Soft_I2C_Stop();                // Issue stop signal
}
void configure_PCA9633()
{
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

void main()
{
     /* Setting all analog pins as digital */
     AD1PCFG = 0xFFFF;

     /* Configures the software I2C module */
     Soft_I2C_Init();

     configure_PCA9633();

     /* FORWARD */
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 150);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 150);
     Delay_ms(2000);

     /* BACKWARD */
     I2C_Send_Message(DEVICE_ADDR, 0x02, 150);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 150);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 0);
     Delay_ms(2000);

     /* IN CIRCLE */
     I2C_Send_Message(DEVICE_ADDR, 0x02, 0);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 150);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 150);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 0);
     Delay_ms(2000);

      /* SOFT BRAKING */
     I2C_Send_Message(DEVICE_ADDR, 0x02, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x03, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x04, 255);
     I2C_Send_Message(DEVICE_ADDR, 0x05, 255);
     Delay_ms(2000);

     while(1);
}
