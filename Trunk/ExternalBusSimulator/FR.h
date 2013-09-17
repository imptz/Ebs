#pragma once
#include "idevice.h"
#include "trajectory.h"

class FR :
	public IDevice
{
private:
	unsigned char psStart;
	unsigned char searchStart;
	unsigned char searchResult;

public:
	FR(unsigned char _address);
	virtual ~FR();
	virtual bool recv(unsigned char* frame);

	void stopPS(unsigned char* frame);
	void startPS(unsigned char* frame);
	void startSearch(unsigned char* frame);
	void getInfo(unsigned char* frame);
	void getInfoUSO(unsigned char* frame);
	void moveToPoint(unsigned char* frame);
	void getStatusMoveToPoint(unsigned char* frame);
	void setPressureTable(unsigned char* frame);
	void startPsParam(unsigned char* frame);
	void povorotniy(unsigned char* frame);
	void setTrajectory(unsigned char* frame);
	void getFire(unsigned char* frame);

	virtual void action();

	static const unsigned int TRAJECTORY_COUNT = 253;
	Trajectory trajectory[TRAJECTORY_COUNT];

	unsigned char subCommandGeneralStart(unsigned char* frame);
	unsigned char subCommandTrajectoryStart(unsigned char* frame);
	unsigned char subCommandSetPoint(unsigned char* frame);
	unsigned char subCommandTrajectoryStop(unsigned char* frame);
	unsigned char subCommandPressureTableStart(unsigned char* frame);
	unsigned char subCommandSetPressureTable(unsigned char* frame);
	unsigned char subCommandPressureTableStop(unsigned char* frame);
};

