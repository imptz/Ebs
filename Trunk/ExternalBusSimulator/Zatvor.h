#pragma once
#include "idevice.h"
class Zatvor :
	public IDevice
{
private:
	enum STATE
	{
		STATE_OPEN = 0,
		STATE_CLOSE = 1,
		STATE_NULL = 3
	};
	STATE state;

public:

	Zatvor(unsigned char _address);
	virtual ~Zatvor();
	virtual bool recv(unsigned char* frame);

	bool isOpen();
	void open();
	void close();
	void open(unsigned char* frame);
	void close(unsigned char* frame);
	void getInfo(unsigned char* frame);

	virtual void action();
};

