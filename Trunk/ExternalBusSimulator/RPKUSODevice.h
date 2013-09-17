#pragma once
#include "iusodevice.h"
#include "DeviceManager.h"

#include <vector>

class RPKUSODevice :
	public IUSODevice
{
private:
	static const unsigned int TYPE = 4;
	static const unsigned int ID = 1;

	enum COMMAND
	{
		COMMAND_GET_ID = 1,
		COMMAND_INIT = 2,
		COMMAND_INITIALIZE_RESULT = 3,
		COMMAND_PUT_FRAME = 4,
		COMMAND_GET_FRAME_BY_ID = 5,
		COMMAND_GET_ANY_READY_FRAME = 6
	};

public:
	RPKUSODevice();
	~RPKUSODevice();

	virtual void keyAction(unsigned int code);
	virtual bool recv(unsigned char* frame);

private:
	void getId(unsigned char* frame);
	void init(unsigned char* frame);
	void getInitResult(unsigned char* frame);
	void putFrame(unsigned char* frame);
	void getFrameById(unsigned char* frame);
	void getAnyReadyFrame(unsigned char* frame);

	enum STATE
	{
		STATE_PROCCESS = 1,
		STATE_READY = 2,
		STATE_ERROR = 3,
		STATE_ID_NOT_FOUND = 4
	};

	struct IdItem
	{
		unsigned short id;
		STATE state;
		unsigned char frame[20];

		IdItem(unsigned short _id, unsigned char* _frame)
			:	id(_id), state(STATE_PROCCESS)
		{
			memcpy(frame, _frame, 20);
		}
	};

	std::vector<IdItem*> idList;
	bool addNewIdItem(unsigned short id, unsigned char* frame);
	IdItem* getIdItem(unsigned short id);
	bool removeIdItem(unsigned short id);

	DeviceManager* prDeviceManager;
	DeviceManager* zatvorDeviceManager;

	public:
		virtual void action();

		int delayCounter;
};

