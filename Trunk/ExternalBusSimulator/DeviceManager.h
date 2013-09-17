#pragma once

#include "Zatvor.h"
#include "FR.h"

#include <vector>

class DeviceManager
{
public:
	std::vector<IDevice*> devices;

	DeviceManager();
	~DeviceManager();

	bool recv(unsigned char* frame);
};

