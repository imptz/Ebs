#pragma once
#include "iusodevice.h"
class MonitoringUSODevice :
	public IUSODevice
{
private:
	static const unsigned int TYPE = 3;
	static const unsigned int ID = 1;

	enum COMMAND
	{
		COMMAND_GET_ID = 1,
		COMMAND_INIT = 2,
		COMMAND_INITIALIZE_RESULT = 3,
		COMMAND_SET_TIME = 4,
		COMMAND_GET_EVENTS = 5,
		COMMAND_SEND_EVENTS = 6
	};

public:
	MonitoringUSODevice();
	~MonitoringUSODevice();

	virtual void keyAction(unsigned int code);
	virtual bool recv(unsigned char* frame);

private:
	void getId(unsigned char* frame);
	void init(unsigned char* frame);
	void getInitResult(unsigned char* frame);
	void setTime(unsigned char* frame);
	void getEvents(unsigned char* frame);
	void sendEvents(unsigned char* frame);

	public:
		virtual void action();

		int systemFault;
		int messageNumber;
};

