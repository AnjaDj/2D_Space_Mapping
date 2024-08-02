/**
 * @file wifi_module.c
 * @brief File containing code for testing wifi module
 *
 * In this code wifi module is tested.  Communication with wifi module is  done
 * over UART interface. Testing is done using modules AT command. When some  AT
 * command is sent to module via UART1, it should respond with some message. In
 * this example four AT commands are  sent and response is caught from the wifi
 * module on microcontroller, and sent to other UART module, UART2. 
 * Confirmation of proper work is done connecting UART-USB converter connected
 * to UART2 and then presented on working machine with serial termina.
 * More info on the wiki page of the repository.
 * @see https://github.com/codeandrelax/2DSpaceMapping_B/wiki/03-%E2%80%90-Testiranje-pojedina%C4%8Dnih-komponenata#testiranje-wi-fi-modula
*/


/**
 * @name Pin remapping macros
 * 
 * Macros that define wifi UART pins, and other UART modules pins.
 * Also two macros for defingin baud rates to be used. Baud rate of 115200
 * is used because for wifi module esp8266 it is default baud rate. 
 * @{
*/
#define WIFI_RX_PIN F6
#define WIFI_TX_PIN F7
#define UART_RX_PIN F10
#define UART_TX_PIN F11
#define BAUD_RATE_WIFI 115200
#define BAUD_RATE 115200
/** @} */


/**
 * @name AT commands
 * 
 * Group of constant strings with AT commands that will be used in program.
 * @{
*/
/** @brief Basic AT command, wifi response is OK */
const char* CMD_AT = "AT\r\n";
/** @brief AT command for software reset */
const char* CMD_RST = "AT+RST\r\n";
/** @brief AT command for setting wifi module in station mode*/
const char* CMD_MODE = "AT+CWMODE=1\r\n";
/** @brief AT command for listing all availabale wifi networks*/
const char* CMD_LIST_AP = "AT+CWLAP\r\n";
/** @} */

unsigned response = 0;


/**
 * @brief Function that is used as UART1 Receiver Interrupt handler.
 *
 * This function is being called when UART1 Receiver Interrupt happened.
 * It takes one byte from UART1 buffer into response variable and proceeds it
 * to UART2.
*/
void UART1ReceiverInterrupt() iv 0x00002A
{
  IEC0.U1RXIE = 0;
  response = UART1_Read();
  UART2_Write(response);
  IEC0.U1RXIE = 1;
  IFS0.U1RXIF = 0;
}

/**
 * @brief Function that is used as UART2 Error Interrupt handler.
 *
 * This function is being called when UART1 Error Interrupt happened. This
 * interrupt can be caused by UART buffer overflow. In interrupt handler
 * buffer overflow bit is cleared so the UART1 can proceed working properly.  
*/
void UART1ErrorInterrupt() iv 0x000096
{
  IEC4.U1ERIE = 0;
  U1STA.OERR = 0;
  IEC4.U1ERIE = 1;
  IFS4.U1ERIF = 0;

}


/**
 * @brief Main function, entry point of the program.
 *
 * In this function some basic MCU configuration is done. After that interrupt
 * configuration is set.
*/
void main()
{
  /** Setting MCU configuration */
  CLKDIV = 0;
  AD1PCFG = 0xFFFF;
  TRISB = 0;
  LATB = 0;

  /** Configuration of UART1 Receiver Interrupt */
  IPC2bits.U1RXIP = 6;
  IFS0.U1RXIF = 0;
  IEC0.U1RXIE = 1;

  /** Configuration UART1 Error Interrupt */
  IPC16bits.U1ERIP = 7;
  IFS4.U1ERIF = 0;
  IEC4.U1ERIE = 1;
  
  
  /** Pin remapping for UART1 */ 
  Unlock_IOLOCK();
  PPS_Mapping_NoLock(6, _INPUT, _U1RX);
  PPS_Mapping_NoLock(7, _OUTPUT, _U1TX);

  /** Pin remaping for UART2 */
  PPS_Mapping_NoLock(10, _INPUT, _U2RX);
  PPS_Mapping_NoLock(11, _OUTPUT, _U2TX);
  Lock_IOLOCK();

  /** Setting baud rates for both UART modules to 115200 */
  UART1_Init(BAUD_RATE_WIFI);
  Delay_ms(500);
  UART2_Init(BAUD_RATE);
  Delay_ms(500);

  /** Sending AT commands */
  UART2_Write_Text("Sending reset command...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_RST);

  Delay_ms(5000);

  UART2_Write_Text("Sending AT command...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_AT);

  Delay_ms(5000);


  UART2_Write_Text("\nSet listing mode...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_MODE);

  Delay_ms(5000);


  UART2_Write_Text("\nListing available APs...\r\n");
  Delay_ms(100);
  UART1_Write_Text(CMD_LIST_AP);

  Delay_ms(5000);


  while(1);
}
