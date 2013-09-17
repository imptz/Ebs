#include "FV300USODevice.h"

#include "iostream"


FV300USODevice::FV300USODevice(unsigned char _address)
	:	updateProccess(true)
{
	address = _address;
}


FV300USODevice::~FV300USODevice()
{
}

bool FV300USODevice::recv(unsigned char* frame)
{
	if (frame[0] != address)
		return false;

	switch(frame[2])
	{
		case COMMAND_GET_ID:
			getId(frame);
			return true;
			break;
		case COMMAND_INIT:
			init(frame);
			return true;
			break;
		case COMMAND_INITIALIZE_RESULT:
			getInitResult(frame);
			return true;
			break;
		case COMMAND_SET_UPDATE_LIST:
			setUpdateList(frame);
			return true;
			break;
		case COMMAND_GET_FIRE:
			getFire(frame);
			return true;
			break;
		case COMMAND_GET_DEVICES_STATE:
			getDevicesState(frame);
			return true;
			break;
		case COMMAND_DEVICES_RESET:
			devicesReset(frame);
			return true;
			break;
	} 		

	return false;
}

void FV300USODevice::getId(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_ID;
	frame[3] = 2;
	frame[4] = 0;
	frame[5] = TYPE;
	frame[6] = ID;
}

void FV300USODevice::init(unsigned char* frame)
{
	char str[800];
	std::cout << "FV300USODevice::init: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_INIT;
	frame[3] = 0;
	frame[4] = 0;
}

void FV300USODevice::getInitResult(unsigned char* frame)
{
	char str[800];
	std::cout << "FV300USODevice::getInitResult: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_INITIALIZE_RESULT;
	frame[3] = 2;
	frame[4] = 0;
	frame[5] = 1;
	if (updateProccess)
		frame[6] = 1;
	else
		frame[6] = 0;
}

void FV300USODevice::setUpdateList(unsigned char* frame)
{
	char str[800];
	std::cout << "FV300USODevice::setUpdateList: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_SET_UPDATE_LIST;
	frame[3] = 1;
	frame[4] = 0;
	frame[5] = 0;
}

void FV300USODevice::getFire(unsigned char* frame)
{
	char str[800];
	std::cout << "FV300USODevice::getFire: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_FIRE;
	frame[3] = 12;
	frame[4] = 0;
	frame[5] = 2;
	frame[6] = 1;
	frame[7] = 255 - 27;
	frame[8] = 0;
	frame[9] = 10;
	frame[10] = 10;
	frame[11] = 1;
	frame[12] = 1;
	frame[13] = 255 - 64;
	frame[14] = 0;
	frame[15] = 10;
	frame[16] = 10;
	//frame[17] = 3;
	//frame[18] = 1;
	//frame[19] = 10;
	//frame[20] = 10;
	//frame[21] = 10;
	//frame[22] = 10;
}

void FV300USODevice::getDevicesState(unsigned char* frame)
{
	char str[800];
	std::cout << "FV300USODevice::getDevicesState: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_DEVICES_STATE;
	for (unsigned int i = 5; i < frame[3] + frame[4] * 256; i++)
		frame[i] = 0;
}

void FV300USODevice::devicesReset(unsigned char* frame)
{
	char str[800];
	std::cout << "FV300USODevice::devicesReset: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
}

void FV300USODevice::action()
{

}
