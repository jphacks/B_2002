#ifndef UART_H
#define	UART_H

#define UART_TX_IN PORTAbits.RA0
#define UART_IR LATAbits.LATA1

extern void UartInit();
extern void UartWriteByte(char value);
extern void UartWrite(char data[], char length);
extern void UartPrint(signed long n);
extern void UartPrintln(signed long n);

#endif
