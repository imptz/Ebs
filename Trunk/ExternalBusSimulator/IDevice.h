#pragma once

#include <iostream>

class IDevice
{
protected:
	unsigned char address;
public:
	enum COMMAND
	{
		COMMAND_GATE_OPEN = 9,
		COMMAND_GATE_CLOSE = 10,
		COMMAND_FR_STOP_PS = 31,
		COMMAND_FR_START_PS = 29,
		COMMAND_FR_START_PS_PARAM = 30,
		COMMAND_FR_INFO_USO = 100,
		COMMAND_FR_POVOROTNIY = 103,
		COMMAND_FR_MOVE_TO_POINT = 117,
		COMMAND_FR_GET_STATUS_MOVE_TO_POINT = 118,
		COMMAND_FR_SET_TRAJECTORY = 130,
		COMMAND_FR_INFO = 155,
		COMMAND_GATE_INFO = 158,
		COMMAND_FR_SEARCH = 233,
		COMMAND_FR_GET_FIRE = 235,
		COMMAND_FR_SET_PRESSURE_TABLE = 236
	};

	IDevice(unsigned char _address);
	virtual ~IDevice();
	virtual bool recv(unsigned char* frame) = 0;
	virtual void action() = 0;
};

