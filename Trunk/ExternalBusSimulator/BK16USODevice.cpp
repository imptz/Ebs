#include "BK16USODevice.h"
#include "debug.h"
#include "keys.h"

#include "iostream"

BK16USODevice::BK16USODevice(){
	XmlElement* xmlElement = Config::getElement("bk16");
	if (xmlElement == nullptr){
		std::cout << "element bk16 ne naiden v konfiguracii" << std::endl;
	}else{
		if (xmlElement->isAttributeExists("address")){
				address = atoi(xmlElement->getAttributeValue("address").c_str());
		}else{
				std::cout << "bk16 - znachenie adresa ne ukazano v konfiguracii" << std::endl;
		}
	}

	inputs = new unsigned char[INPUTS_NUMBER];
	for (unsigned int i = 0; i < INPUTS_NUMBER; ++i)
		inputs[i] = 1;
}


BK16USODevice::~BK16USODevice()
{
}

void BK16USODevice::keyAction(unsigned int code){
	switch(code){
		case KEY_0:
			switchInput(0);
			break;
		case KEY_1:
			switchInput(1);
			break;
		case KEY_2:
			switchInput(2);
			break;
	}
}

bool BK16USODevice::recv(unsigned char* frame)
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
		case COMMAND_SET_OUTPUT:
			setOutput(frame);
			return true;
			break;
		case COMMAND_GET_INPUTS:
			getInputs(frame);
			return true;
			break;
	}

	return false;
}

void BK16USODevice::getId(unsigned char* frame)
{
	//char str[800];
	//std::cout << "BK16USODevice::getId ";
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

void BK16USODevice::init(unsigned char* frame)
{
	if (Debug::BK16_DEBUG_OUT){
		char str[800];
		std::cout << "BK16USODevice::init ";
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

void BK16USODevice::getInitResult(unsigned char* frame)
{
	if (Debug::BK16_DEBUG_OUT){
		char str[800];
		std::cout << "BK16USODevice::getInitResult ";
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

void BK16USODevice::setOutput(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_SET_OUTPUT;
	frame[3] = 0;
	frame[4] = 0;
}

void BK16USODevice::getInputs(unsigned char* frame)
{
	unsigned int inCount = frame[6] * 256 + frame[5];
	unsigned int sdatalength = inCount * 3 + 3;

	unsigned char* inData = new unsigned char[sdatalength + 7];
	memcpy(inData, frame, sdatalength + 7);

	for (unsigned int i = 0; i < inCount; i++){
		frame[7 + i * 3] = inData[7 + i * 2];
		frame[7 + i * 3 + 1] = inData[7 + i * 2 + 1];
		frame[7 + i * 3 + 2] = 1;
	}

	for (unsigned int i = 0; i < INPUTS_NUMBER; i++){
		frame[7 + i * 3] = i;
		frame[7 + i * 3 + 1] = i >> 8;
		frame[7 + i * 3 + 2] = inputs[i];
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = COMMAND_GET_INPUTS;
	frame[3] = sdatalength;
	frame[4] = sdatalength >> 8;
	frame[5] = inCount;
	frame[6] = inCount >> 8;
	frame[7 + inCount * 3] = 0;

	if (Debug::BK16_DEBUG_OUT){
		char str[800];
		std::cout << "BK16USODevice::getInputs: ";
		for (int i = 0; i < frame[3] + frame[4] * 256; i++)
			std::cout << _itoa(frame[5 + i], str, 16) << " ";
		std::cout << std::endl;
	}
}

void BK16USODevice::action()
{

}

void BK16USODevice::switchInput(unsigned char number){
	if (inputs[number] == 1){
		std::cout << __FUNCTION__ << " input = " << static_cast<unsigned int>(number) << " ON" << std::endl;
		inputs[number] = 2;
	}else{
		std::cout << __FUNCTION__ << " input = " << static_cast<unsigned int>(number) << " OFF" << std::endl;
		inputs[number] = 1;
	}
}
