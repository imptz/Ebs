#include "RPKUSODevice.h"
#include "FR.h"
#include "Zatvor.h"
#include "debug.h"
#include "keys.h"
#include "iostream"


RPKUSODevice::RPKUSODevice()
	:	prDeviceManager(new DeviceManager()), zatvorDeviceManager(new DeviceManager()), delayCounter(0)
{
	XmlElement* xmlElement = Config::getElement("rpk");
	if (xmlElement == nullptr){
		std::cout << "element rpk ne naiden v konfiguracii" << std::endl;
	}else{
		if (xmlElement->isAttributeExists("address")){
				address = atoi(xmlElement->getAttributeValue("address").c_str());
		}else{
				std::cout << "rpk - znachenie adresa ne ukazano v konfiguracii" << std::endl;
		}

		unsigned int devCount = xmlElement->getElementCount();
		for (unsigned int i = 0; i < devCount; ++i){
			XmlElement* deviceElement = xmlElement->getElement(i);
			if (deviceElement->isAttributeExists("address")){
				if (deviceElement->getName() == "pr")
					prDeviceManager->devices.push_back(new FR(atoi(deviceElement->getAttributeValue("address").c_str())));
				else
					if (deviceElement->getName() == "zatvor")
						zatvorDeviceManager->devices.push_back(new Zatvor(atoi(deviceElement->getAttributeValue("address").c_str())));
			}else{
				std::cout << "rpk - dla ustroystva '" << deviceElement->getName() <<  "' ne ukazano znachenie adresa v konfiguracii" << std::endl;
			}

		}
	}
}


RPKUSODevice::~RPKUSODevice()
{
}

void RPKUSODevice::keyAction(unsigned int code){
	switch(code){
		case KEY_Z:
			if (reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[0])->isOpen())
				reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[0])->close();
			else
				reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[0])->open();
			break;
		case KEY_X:
			if (reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[1])->isOpen())
				reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[1])->close();
			else
				reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[1])->open();
			break;
		case KEY_C:
			if (reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[2])->isOpen())
				reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[2])->close();
			else
				reinterpret_cast<Zatvor*>(zatvorDeviceManager->devices[2])->open();
			break;
	}
}

bool RPKUSODevice::recv(unsigned char* frame)
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
		case COMMAND_PUT_FRAME:
			putFrame(frame);
			return true;
			break;
		case COMMAND_GET_FRAME_BY_ID:
			getFrameById(frame);
			return true;
			break;
		case COMMAND_GET_ANY_READY_FRAME:
			getAnyReadyFrame(frame);
			return true;
			break;
	}

	return false;
}

void RPKUSODevice::getId(unsigned char* frame)
{
	//char str[800];
	//std::cout << "RPKUSODevice::getId ";
	//for (int i = 0; i < frame[3] + frame[4] * 256 + 7; i++)
	//	std::cout << _itoa(frame[i], str, 16) << " ";
	//std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_ID;
	frame[3] = 2;
	frame[4] = 0;
	frame[5] = TYPE;
	frame[6] = ID;
}

void RPKUSODevice::init(unsigned char* frame)
{
	if (Debug::RPK_DEBUG_OUT){
		char str[800];
		std::cout << "RPKUSODevice::init ";
		for (int i = 0; i < frame[3] + frame[4] * 256 + 7; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	fInit = true;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_INIT;
	frame[3] = 0;
	frame[4] = 0;
}

void RPKUSODevice::getInitResult(unsigned char* frame)
{
	if (Debug::RPK_DEBUG_OUT){
		char str[800];
		std::cout << "RPKUSODevice::getInitResult ";
		for (int i = 0; i < frame[3] + frame[4] * 256 + 7; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_INITIALIZE_RESULT;
	frame[3] = 1;
	frame[4] = 0;
	frame[5] = fInit;
}

void RPKUSODevice::putFrame(unsigned char* frame)
{
	addNewIdItem(frame[5] + frame[6] * 256, frame);
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_PUT_FRAME;
	frame[3] = 3;
	frame[4] = 0;
	frame[7] = 0;
}

void RPKUSODevice::getFrameById(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_FRAME_BY_ID;
	frame[3] = 3;
	frame[4] = 0;
	frame[7] = 4;
}

void RPKUSODevice::getAnyReadyFrame(unsigned char* frame)
{
	if (delayCounter == 0)
	{
		delayCounter = 0;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_ANY_READY_FRAME;
	
	std::vector<unsigned int> ids;
	unsigned int offset = 5;
	for (unsigned int i = 0; i < idList.size(); i++)
	{
		if (idList[i]->state != STATE_PROCCESS)
		{
			ids.push_back(idList[i]->id);
			frame[offset++] = idList[i]->id;
			frame[offset++] = idList[i]->id >> 8;
			frame[offset++] = idList[i]->state;
			if (idList[i]->state == STATE_READY)
				for (unsigned int n = 0; n < idList[i]->frame[3] + 5; n++)
					frame[offset++] = idList[i]->frame[n];
		}
	}

	frame[3] = offset - 5;
	frame[4] = (offset - 5) >> 8;
	for (unsigned int i = 0; i < ids.size(); i++)
		removeIdItem(ids[i]);

	//char str[800];
	//std::cout << "RPKUSODevice::getAnyReadyFrame ";
	//for (int i = 0; i < 40; i++)
	//	std::cout << _itoa(frame[i], str, 16) << " ";
	//std::cout << std::endl;
	}
	else
	{
		frame[0] = frame[1];
		frame[1] = address;
		frame[2] = COMMAND_GET_ANY_READY_FRAME;
		frame[3] = 0;
		frame[4] = 0;
	//char str[800];
	//std::cout << "RPKUSODevice::getAnyReadyFrame___delayCoounter ";
	//for (int i = 0; i < 40; i++)
	//	std::cout << _itoa(frame[i], str, 16) << " ";
	//std::cout << std::endl;
		delayCounter++;
	}
}

bool RPKUSODevice::addNewIdItem(unsigned short id, unsigned char* frame)
{
	idList.push_back(new IdItem(id, &frame[7]));
	return true;
}

RPKUSODevice::IdItem* RPKUSODevice::getIdItem(unsigned short id)
{
	for (unsigned int i = 0; i < idList.size(); i++)
		if (idList[i]->id == id)
			return idList[i];

	return nullptr;
}

bool RPKUSODevice::removeIdItem(unsigned short id)
{
	for (unsigned int i = 0; i < idList.size(); i++)
		if (idList[i]->id == id)
		{
			delete idList[i];
			idList.erase(idList.begin() + i);
			return true;
		}

	return false;
}

void RPKUSODevice::action()
{
	//std::cout << "RPKUSODevice::action()" << std::endl;

	for (unsigned int i = 0; i < idList.size(); i ++)
		if (idList[i]->state == STATE_PROCCESS)
			if ((prDeviceManager->recv(idList[i]->frame)) || (zatvorDeviceManager->recv(idList[i]->frame)))
				idList[i]->state = STATE_READY;
			else
				idList[i]->state = STATE_ERROR;


	for (unsigned int i = 0; i < prDeviceManager->devices.size(); i++)
		prDeviceManager->devices[i]->action();

	for (unsigned int i = 0; i < zatvorDeviceManager->devices.size(); i++)
		zatvorDeviceManager->devices[i]->action();
}
