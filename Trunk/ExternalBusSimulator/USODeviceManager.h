#pragma once

#include <vector>

#include "BK16USODevice.h"
#include "RPKUSODevice.h"
#include "FV300USODevice.h"
#include "TVUSODevice.h"
#include "MonitoringUSODevice.h"

enum DEVICES{
	DEVICES_BK16 = 0,
	DEVICES_RPK = 1,
	DEVICES_MONITORING = 2
};

class USODeviceManager
{
public:
	std::vector<IUSODevice*> devices;

	USODeviceManager();
	~USODeviceManager();

	bool recv(unsigned char* frame);
};

