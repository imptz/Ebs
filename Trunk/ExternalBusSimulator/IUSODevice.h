#pragma once

#include "DeviceManager.h"
#include "config.h"

class IUSODevice
{
protected:
	unsigned char address;

public:
	IUSODevice();
	virtual ~IUSODevice();

	virtual bool recv(unsigned char* frame) = 0;
	virtual void action() = 0;

	bool fInit;
	unsigned char getAddress(){
		return address;
	}

	virtual void keyAction(unsigned int code) = 0;
};

