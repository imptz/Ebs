#include "Zatvor.h"
#include "debug.h"

Zatvor::Zatvor(unsigned char _address)
	:	IDevice(_address), state(STATE_CLOSE)
{
	std::cout << "create Zatvor address = " << static_cast<unsigned int>(address) << std::endl;
}


Zatvor::~Zatvor()
{
}

bool Zatvor::recv(unsigned char* frame)
{
	if (frame[0] != address)
		return false;

	switch (frame[2])
	{
		case COMMAND_GATE_OPEN:
			open(frame);
			return true;
			break;
		case COMMAND_GATE_CLOSE:
			close(frame);
			return true;
			break;
		case COMMAND_GATE_INFO:
			getInfo(frame);
			return true;
			break;
	}

	return false;
}

bool Zatvor::isOpen(){
	return (state == STATE_OPEN);
}

void Zatvor::open(){
	state = STATE_OPEN;
	std::cout << __FUNCTION__ << " address = " << static_cast<unsigned int>(address) << std::endl;
}

void Zatvor::open(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << ": type - " << _itoa(frame[5], &str[32], 10) << std::endl;
	}

	state = STATE_OPEN;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_GATE_OPEN;
	frame[5] = 0;
}

void Zatvor::close(){
	state = STATE_CLOSE;
	std::cout << __FUNCTION__ << " address = " << static_cast<unsigned int>(address) << std::endl;
}

void Zatvor::close(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << ": type - " << _itoa(frame[5], &str[32], 10) << std::endl;
	}

	state = STATE_CLOSE;
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_GATE_CLOSE;
	frame[5] = 0;
}

void Zatvor::getInfo(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 3;
	frame[4] = COMMAND_GATE_INFO;
	if (state == STATE_OPEN)
		frame[5] = 0x10;
	else
		frame[5] = 0x04;
	frame[6] = 0;
	frame[7] = 0;

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}
}


void Zatvor::action()
{
	//std::cout << "Zatvor::action()" << std::endl;
}
