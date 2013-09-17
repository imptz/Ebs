#include "DeviceManager.h"


DeviceManager::DeviceManager()
{
}


DeviceManager::~DeviceManager()
{
	for (unsigned int i = 0; i < devices.size(); i ++)
		delete devices[i];
}

bool DeviceManager::recv(unsigned char* frame)
{
	for (unsigned int i = 0; i < devices.size(); i++)
		if (devices[i]->recv(frame))
			return true;

	return false;
}
