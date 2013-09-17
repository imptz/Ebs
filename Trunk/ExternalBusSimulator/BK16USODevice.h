#pragma once
#include "iusodevice.h"

class BK16USODevice :	public IUSODevice
{
private:
	static const unsigned int TYPE = 1;
	static const unsigned int ID = 1;

	enum COMMAND
	{
		COMMAND_GET_ID = 1,
		COMMAND_INIT = 2,
		COMMAND_INITIALIZE_RESULT = 3,
		COMMAND_SET_OUTPUT = 4,
		COMMAND_GET_INPUTS = 5
	};

public:
	BK16USODevice();
	~BK16USODevice();

	virtual bool recv(unsigned char* frame);
	virtual void keyAction(unsigned int code);

private:
	void getId(unsigned char* frame);
	void init(unsigned char* frame);
	void getInitResult(unsigned char* frame);
	void setOutput(unsigned char* frame);
	void getInputs(unsigned char* frame);

public:
	virtual void action();

	void switchInput(unsigned char number);

private:
	static const unsigned int INPUTS_NUMBER = 8;
	unsigned char* inputs;
};

