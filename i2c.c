#include <xc.h>
#include "i2c.h"
#include "uart.h"

void I2cInit()
{
    TRISC |= 0x18;
    SSPSTAT = 0x00;
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;
    SSPCON3 = 0x00;
    SSPMSK = 0xff;
    SSPADD = 19; // 400kHz
}

void I2cIdleCheck()
{
    while (SSPSTAT & 0x04 | SSPCON2 & 0x1f);
}

void I2cStart()
{
    I2cIdleCheck();
    SSPCON2bits.SEN = 1;
}

void I2cStop()
{
    I2cIdleCheck();
    SSPCON2bits.PEN = 1;
}

void I2cRestart()
{
    I2cIdleCheck();
    SSPCON2bits.RSEN = 1;
}

void I2cWriteOne(char value)
{
    I2cIdleCheck();
    SSPBUF = value;
    while (SSPCON2bits.ACKSTAT);
}

char I2cReadOne(char a)
{
    char value;
    I2cIdleCheck();
    SSPCON2bits.RCEN = 1;
    I2cIdleCheck();
    value = SSPBUF;
    SSPCON2bits.ACKDT = a;
    SSPCON2bits.ACKEN = 1;
    return value;
}

void I2cWriteByte(char address, char value)
{
    I2cStart();
    I2cWriteOne(value);
    I2cStop();
}

void I2cWrite(char address, char data[], char length)
{
    char i;
    I2cStart();
    I2cWriteOne(address << 1);
    for (i = 0; i < length; i++)
    {
        I2cWriteOne(data[i]);
    }
    I2cStop();
}

void I2cWriteRegister(char address, char subaddress, char value)
{
    I2cStart();
    I2cWriteOne(address << 1);
    I2cWriteOne(subaddress);
    I2cWriteOne(value);
    I2cStop();
}

void I2cWriteMultiRegisters(char address, char subaddress, char data[], char length)
{
    char i;
    I2cStart();
    I2cWriteOne(address << 1);
    I2cWriteOne(subaddress);
    for (i = 0; i < length; i++)
    {
        I2cWriteOne(data[i]);
    }
    I2cStop();
}

char I2cReadByte(char address)
{
    char value;
    I2cStart();
    I2cWriteOne(address << 1 | 1);
    value = I2cReadOne(1);
    I2cStop();
    return value;
}

void I2cRead(char address, char data[], char length)
{
    char i;
    I2cStart();
    I2cWriteOne(address << 1 | 1);
    for (i = 0; i < length - 1; i++)
    {
        data[i] = I2cReadOne(0);
    }
    data[length - 1] = I2cReadOne(1);
    I2cStop();
}

char I2cReadRegister(char address, char subaddress)
{
    char value;
    I2cStart();
    I2cWriteOne(address << 1);
    I2cWriteOne(subaddress);
    I2cRestart();
    I2cWriteOne(address << 1 | 1);
    value = I2cReadOne(1);
    I2cStop();
    return value;
}

void I2cReadMultiRegisters(char address, char subaddress, char data[], char length)
{
    char i;
    I2cStart();
    I2cWriteOne(address << 1);
    I2cWriteOne(subaddress);
    I2cRestart();
    I2cWriteOne(address << 1 | 1);
    for (i = 0; i < length - 1; i++)
    {
        data[i] = I2cReadOne(0);
    }
    data[length - 1] = I2cReadOne(1);
    I2cStop();
}

