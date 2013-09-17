#include "USODeviceManager.h"

USODeviceManager::USODeviceManager(){
	devices.push_back(new BK16USODevice());
	devices.push_back(new RPKUSODevice());
	devices.push_back(new MonitoringUSODevice());
	//devices.push_back(new FV300USODevice());
	//devices.push_back(new TVUSODevice());
}

USODeviceManager::~USODeviceManager()
{
	for (unsigned int i = 0; i < devices.size(); i ++)
		delete devices[i];
}

bool USODeviceManager::recv(unsigned char* frame)
{
	for (unsigned int i = 0; i < devices.size(); i++)
		if (devices[i]->recv(frame))
			return true;

	return false;
}