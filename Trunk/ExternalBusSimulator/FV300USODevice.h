#pragma once
#include "iusodevice.h"
class FV300USODevice :
	public IUSODevice
{
private:
	static const unsigned int TYPE = 2;
	static const unsigned int ID = 1;

	enum COMMAND
	{
		COMMAND_GET_ID = 1,
		COMMAND_INIT = 2,
		COMMAND_INITIALIZE_RESULT = 3,
		COMMAND_SET_UPDATE_LIST = 4,
		COMMAND_GET_FIRE = 5,
		COMMAND_GET_DEVICES_STATE = 6,
		COMMAND_DEVICES_RESET = 7
	};

public:
	FV300USODevice(unsigned char address);
	~FV300USODevice();

	virtual bool recv(unsigned char* frame);

private:
	void getId(unsigned char* frame);
	void init(unsigned char* frame);
	void getInitResult(unsigned char* frame);
	void setUpdateList(unsigned char* frame);
	void getFire(unsigned char* frame);
	void getDevicesState(unsigned char* frame);
	void devicesReset(unsigned char* frame);	

public:
	virtual void action();

private:
	bool updateProccess;
};

