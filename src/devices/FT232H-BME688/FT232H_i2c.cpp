#include "FT232H_i2c.h"
#include <stdexcept>

FT232H_I2C::FT232H_I2C(unsigned int i2cChannel)
{
	Init_libMPSSE();
    DWORD channels;
    std::cout << "about to call get channels" << std::endl;
    I2C_GetNumChannels(&channels);

    ChannelConfig channelConf;
	channelConf.ClockRate = I2C_CLOCK_STANDARD_MODE;
	channelConf.LatencyTimer = 10;
	channelConf.Options =  0;

    if (channels == 0)
    {
        throw std::runtime_error("No channels found");
    }
    else if (i2cChannel >= channels)
    {
        throw std::runtime_error("Invalid channel number");
    }
    FT_STATUS status = I2C_OpenChannel(i2cChannel, &ftHandle);
    if (status != FT_OK)
    {
        throw std::runtime_error("Failed to open channel");
    }
    status = I2C_InitChannel(ftHandle, &channelConf);
    if (status != FT_OK)
    {
        throw std::runtime_error("Failed to initialize channel");
    }
}

void FT232H_I2C::printVersionCheck()
{
    DWORD verMPSSE, verD2XX;
    Ver_libMPSSE(&verMPSSE, &verD2XX);
    std::cout << "libmpsse: " << verMPSSE << std::endl;
    std::cout << "libftd2xx: " << verD2XX << std::endl;
}

DWORD FT232H_I2C::writeByte(UCHAR slaveAddress, UCHAR registerAddress, UCHAR data)
{
	DWORD bytesToTransfer = 0;
	DWORD bytesTransfered;
	BOOL writeComplete = 0;
	DWORD retry = 0;
    UCHAR buffer[I2C_DEVICE_BUFFER_SIZE];
    FT_STATUS status;
	
	bytesToTransfer = 0;
	bytesTransfered = 0;
	buffer[bytesToTransfer++] = registerAddress; //Byte addressed inside EEPROM's memory
	buffer[bytesToTransfer++] = data;
	status = I2C_DeviceWrite(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_STOP_BIT);

	while((writeComplete == 0) && (retry < I2C_WRITE_COMPLETION_RETRY))
	{
		bytesToTransfer = 0;
		bytesTransfered = 0;
		buffer[bytesToTransfer++] = registerAddress; //Byte addressed inside EEPROM's memory
		status = I2C_DeviceWrite(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT|I2C_TRANSFER_OPTIONS_BREAK_ON_NACK);
		if (FT_OK != status)
		{
            std::cout << "EEPROM write cycle isn't complete\n" << std::endl;
		}
		if (bytesToTransfer == bytesTransfered)
		{
			writeComplete = 1;
		}
		else
		{
            std::cout << "Retrying..." << std::endl;
		}
		retry++;
	}
	return 0;
}

FT_STATUS FT232H_I2C::readByte(UCHAR slaveAddress, UCHAR registerAddress, UCHAR *data)
{
	FT_STATUS status;
	DWORD bytesToTransfer = 0;
	DWORD bytesTransfered;
    UCHAR buffer[I2C_DEVICE_BUFFER_SIZE];
	
	bytesToTransfer = 0;
	bytesTransfered = 0;
	buffer[bytesToTransfer++] = registerAddress; //Byte addressed inside EEPROM's memory
	status = I2C_DeviceWrite(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT);


	bytesToTransfer = 1;
	bytesTransfered = 0;
	status |= I2C_DeviceRead(ftHandle, slaveAddress, bytesToTransfer, buffer, &bytesTransfered, I2C_TRANSFER_OPTIONS_START_BIT | I2C_TRANSFER_OPTIONS_STOP_BIT | I2C_TRANSFER_OPTIONS_NACK_LAST_BYTE);
	*data = buffer[0];
	return status;
}


FT232H_I2C::~FT232H_I2C()
{
    I2C_CloseChannel(ftHandle);
    Cleanup_libMPSSE();
}
