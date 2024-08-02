char string[30];
char delimiter = '\\';
unsigned response = 0;

void UART1_Receiver_Interrupt() iv 0x00002A
{
  IEC0.U1RXIE = 0;
  response = UART1_Read();
  UART2_Write(response);
  IEC0.U1RXIE = 1;
  IFS0.U1RXIF = 0;
}
void UART1_Error_Interrupt() iv 0x000096
{
  IEC4.U1ERIE = 0;
  U1STA.OERR  = 0;
  IEC4.U1ERIE = 1;
  IFS4.U1ERIF = 0;
}

void main() {
    AD1PCFG = 0xFFFF;
    TRISB = 0;
    LATB = 0;

    IPC2bits.U1RXIP = 5;
    IFS0.U1RXIF = 0;
    IEC0.U1RXIE = 1;

    IPC16bits.U1ERIP = 5;
    IFS4.U1ERIF = 0;
    IEC4.U1ERIE = 1;
    Unlock_IOLOCK();
    PPS_Mapping_NoLock(15, _OUTPUT, _U1TX);
    PPS_Mapping_NoLock(10, _INPUT, _U1RX);
    Lock_IOLOCK();
    UART1_Init(9600);
    delay_ms(1000);
    UART1_Write_Text("\r\n");
    delay_ms(500);
    UART1_Write_Text("AT+RST\r\n");
    delay_ms(2000);
    /*UART1_Write_Text("AT+CWJAP_CUR=\"etfbl.net\",\"\"\r\n");
    delay_ms(2000);
    UART1_Write_Text("AT+CIPMUX=1\r\n");
    delay_ms(1000);
    UART1_Write_Text("AT+CIPSERVER=1,1492\r\n");
    delay_ms(1000);*/
    UART1_Write_Text("AT+CWMODE_CUR=2\r\n");
    delay_ms(1000);
    while(1){
        UART1_Read_Text(string, &delimiter, 255);
        delay_ms(3000);
        if (string[0] == 'a'){
           LATB.F2 = ~LATB.F2;
           delay_ms(1000);
        }
    }
}