#include "TVUSODevice.h"

#include "iostream"


TVUSODevice::TVUSODevice(unsigned char _address)
	:	updateProccess(true)
{
	address = _address; 
}


TVUSODevice::~TVUSODevice()
{
}

bool TVUSODevice::recv(unsigned char* frame)
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

void TVUSODevice::getId(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_ID;
	frame[3] = 2;
	frame[4] = 0;
	frame[5] = TYPE;
	frame[6] = ID;
}

void TVUSODevice::init(unsigned char* frame)
{
	char str[800];
	std::cout << "TVUSODevice::init: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_INIT;
	frame[3] = 0;
	frame[4] = 0;
}

void TVUSODevice::getInitResult(unsigned char* frame)
{
	char str[800];
	std::cout << "TVUSODevice::getInitResult: ";
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

void TVUSODevice::setUpdateList(unsigned char* frame)
{
	char str[800];
	std::cout << "TVUSODevice::setUpdateList: ";
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

void TVUSODevice::getFire(unsigned char* frame)
{
	char str[800];
	std::cout << "TVUSODevice::getFire: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_FIRE;
	frame[3] = 12;
	frame[4] = 0;
	frame[5] = 1;
	frame[6] = 1;
	frame[7] = 2;
	frame[8] = 23;
	frame[9] = 45;
	frame[10] = 12;
	frame[11] = 2;
	frame[12] = 1;
	frame[13] = 23;
	frame[14] = 45;
	frame[15] = 12;
	frame[16] = 78;
}

void TVUSODevice::getDevicesState(unsigned char* frame)
{
	char str[800];
	std::cout << "TVUSODevice::getDevicesState: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_DEVICES_STATE;
	for (unsigned int i = 5; i < frame[3] + frame[4] * 256; i++)
		frame[i] = 0;
}

void TVUSODevice::devicesReset(unsigned char* frame)
{
	char str[800];
	std::cout << "TVUSODevice::devicesReset: ";
	for (int i = 0; i < frame[3] + frame[4] * 256; i++)
		std::cout << _itoa(frame[5 + i], str, 16) << " ";
	std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
}

void TVUSODevice::action()
{

}
