#ifndef FT232H_I2C_H
#define FT232H_I2C_H

#include "ftd2xx.h"
#include "libmpsse_i2c.h"
#include <iostream>

#define I2C_DEVICE_BUFFER_SIZE			256
#define I2C_WRITE_COMPLETION_RETRY		10

class FT232H_I2C
{
public:
    FT232H_I2C(unsigned int i2cChannel);
    DWORD writeByte(UCHAR slaveAddress, UCHAR registerAddress, UCHAR data);
    FT_STATUS readByte(UCHAR slaveAddress, UCHAR registerAddress, UCHAR *data);
    void printVersionCheck();

    ~FT232H_I2C();
private:
    FT_HANDLE ftHandle;

};

#endif
