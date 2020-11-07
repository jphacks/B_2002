#ifndef I2C_H
#define	I2C_H

extern void I2cInit();
extern void I2cWriteByte(char address, char value);
extern void I2cWrite(char address, char data[], char length);
extern void I2cWriteRegister(char address, char subaddress, char value);
extern void I2cWriteMultiRegisters(char address, char subaddress, char data[], char length);
extern char I2cReadByte(char address);
extern void I2cRead(char address, char data[], char length);
extern char I2cReadRegister(char address, char subaddress);
extern void I2cReadMultiRegisters(char address, char subaddress, char data[], char length);

#endif