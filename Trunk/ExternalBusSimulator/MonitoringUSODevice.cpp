#include "MonitoringUSODevice.h"
#include "debug.h"
#include "keys.h"

#include "iostream"


MonitoringUSODevice::MonitoringUSODevice()
	:	systemFault(0), messageNumber(0)
{
	XmlElement* xmlElement = Config::getElement("monitoring");
	if (xmlElement == nullptr){
		std::cout << "element monitoring ne naiden v konfiguracii" << std::endl;
	}else{
		if (xmlElement->isAttributeExists("address")){
				address = atoi(xmlElement->getAttributeValue("address").c_str());
		}else{
				std::cout << "monitoring - znachenie adresa ne ukazano v konfiguracii" << std::endl;
		}
	}
}


MonitoringUSODevice::~MonitoringUSODevice()
{
}

void MonitoringUSODevice::keyAction(unsigned int code){
	switch(code){
	}
}

bool MonitoringUSODevice::recv(unsigned char* frame)
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
		case COMMAND_SET_TIME:
			setTime(frame);
			return true;
			break;
		case COMMAND_GET_EVENTS:
			getEvents(frame);
			return true;
			break;
		case COMMAND_SEND_EVENTS:
			sendEvents(frame);
			return true;
			break;
	}

	return false;
}

void MonitoringUSODevice::getId(unsigned char* frame)
{
	//char str[800];
	//std::cout << "MonitoringUSODevice::getId ";
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

void MonitoringUSODevice::init(unsigned char* frame)
{
	if (Debug::MONITORING_DEBUG_OUT){
		char str[800];
		std::cout << "MonitoringUSODevice::init ";
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

void MonitoringUSODevice::getInitResult(unsigned char* frame)
{
	if (Debug::MONITORING_DEBUG_OUT){
		char str[800];
		std::cout << "MonitoringUSODevice::getInitResult ";
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

void MonitoringUSODevice::setTime(unsigned char* frame)
{
	//char str[800];
	//std::cout << "MonitoringUSODevice::setTime ";
	//for (int i = 0; i < frame[3] + frame[4] * 256 + 7; i++)
	//	std::cout << _itoa(frame[i], str, 16) << " ";
	//std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_SET_TIME;
	frame[3] = 1;
	frame[4] = 0;
	frame[5] = 1;
}

void MonitoringUSODevice::getEvents(unsigned char* frame)
{
	static unsigned int id = 1;
	//char str[800];
	//std::cout << "MonitoringUSODevice::getEvents ";
	//for (int i = 0; i < frame[3] + frame[4] * 256 + 7; i++)
	//	std::cout << _itoa(frame[i], str, 16) << " ";
	//std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_EVENTS;
	frame[3] = 4;
	frame[4] = 0;
	frame[5] = id;
	frame[6] = id >> 8;
	frame[7] = systemFault;
	frame[8] = 0;

	static unsigned char msgNum = 1;

	if ((msgNum != 0) && (msgNum < 130))
		msgNum++;
	else
		msgNum = 0;

//	if (messageNumber != 0)
	//if (msgNum != 0)
	//{
	//	frame[3] += 13;

	//frame[8] = 1;
	//frame[9] = 2;

	//frame[10] = msgNum;

	//frame[11] = 0;
	//frame[12] = 16;
	//frame[13] = 9;
	//frame[14] = 56;
	//frame[15] = 9;
	//frame[16] = 2;
	//frame[17] = 1;
	//frame[18] = 1;
	//frame[19] = msgNum;
	//frame[20] = 3;
	//frame[21] = 4;
	//messageNumber = 0;
	//}
	id++;
}

void MonitoringUSODevice::sendEvents(unsigned char* frame)
{
	//char str[800];
	//std::cout << "MonitoringUSODevice::sendEvents ";
	//for (int i = 0; i < frame[3] + frame[4] * 256 + 7; i++)
	//	std::cout << _itoa(frame[i], str, 16) << " ";
	//std::cout << std::endl;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_SEND_EVENTS;
	frame[3] = 1;
	frame[4] = 0;
	frame[5] = 0;
}

void MonitoringUSODevice::action()
{

}