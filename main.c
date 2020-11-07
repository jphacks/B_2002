#include <xc.h>
#include "uart.h"
#include "COBS.h"
#include "i2c.h"
#include "DPS310.h"

#pragma config FOSC = INTOSC, WDTE = OFF, PWRTE = ON, LVP = OFF
#define _XTAL_FREQ 32000000

void interrupt __isr__()
{
    static char clock = 0;
    if (PIR1bits.TMR2IF)
    {
        UART_IR = UART_TX_IN & clock;
        clock ^= 1;
        PIR1bits.TMR2IF = 0;
    }
}

void main()
{
    OSCCON = 0x70;
    ANSELA = 0x00;
    ANSELB = 0x00;
    TRISA = 0x01;
    TRISB = 0x00;
    TRISC = 0x00;
    TRISE = 0x00;
    
    PR2 = 104;
    T2CON = 0x00;
    
    UartInit();
    
    const char text[32] = "Madokano Project\n";
    
    I2cInit();
    DPS310_Init();
    
    double pressure_avg = 0;
    double pressure;
    char i;
    double _dp;
    char dp_data[4];
    char send_data[6];
    
    __delay_ms(1000);
    
    for (i = 0; i < 8; i++)
    {
        pressure_avg += DPS310_Pressure();
        __delay_ms(100);
    }
    pressure_avg /= 8;
    
    INTCON = 0xc0;
   
    while (1)
    {
        pressure = DPS310_Pressure();
        _dp = pressure - pressure_avg;
//        UartPrintln(pressure);
        dp_data[0] = (char)(_dp + 128);
        dp_data[1] = (char)(dp_data[0] + 5);
        dp_data[2] = 0xfd;
        
        UartWrite(dp_data, 3);
        
////        UartWrite(text, 17);
//        
//        
//        UartPrintln(DPS310_PRS());
//        UartPrintln(DPS310_TMP());
//        UartPrintln(_c0);
//        UartPrintln(_c1);
//        UartPrintln(_c00);
//        UartPrintln(_c10);
//        UartPrintln(_c01);
//        UartPrintln(_c11);
//        UartPrintln(_c20);
//        UartPrintln(_c21);
//        UartPrintln(_c30);
//        UartWriteByte('\n');
        
        __delay_ms(300);
    }
}