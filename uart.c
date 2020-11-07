#include <xc.h>
#include "uart.h"

#define _XTAL_FREQ 32000000

void UartInit()
{
//    BAUDCON = 0x00;
    BAUDCON = 0x10;
    SPBRG = 207;
    SPBRGH = 0;
    TXSTA = 0x20;
    RCSTA = 0x80;
}

void UartWriteByte(char value)
{
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CONbits.TMR2ON = 1;
    while (!PIR1bits.TXIF);
    TXREG = value;
    while (!PIR1bits.TXIF);
    while (!TXSTAbits.TRMT);
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 0;
    T2CONbits.TMR2ON = 0;
    UART_IR = 0;
}

void UartWrite(char data[], char length)
{
    char i = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CONbits.TMR2ON = 1;
    for (i = 0; i < length; i++)
    {
        while (!PIR1bits.TXIF);
        TXREG = data[i];
    }
    while (!PIR1bits.TXIF);
    while (!TXSTAbits.TRMT);
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 0;
    T2CONbits.TMR2ON = 0;
    UART_IR = 0;
}

void UartPrint(signed long n)
{
    char i;
    char data[11];
    if (n < 0)
    {
        n = -n;
        data[0] = '-';
    }
    else
    {
        data[0] = '+';
    }
    for (i = 1; i < 11; i++)
    {
        data[11-i] = '0' + n % 10;
        n /= 10;
    }
    UartWrite(data, 11);
}

void UartPrintln(signed long n)
{
    UartPrint(n);
    UartWriteByte('\n');
}