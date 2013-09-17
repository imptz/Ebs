#include "FR.h"
#include "debug.h"

FR::FR(unsigned char _address)
	:	IDevice(_address), psStart(0), searchStart(0), searchResult(0)
{
	std::cout << "create PR address = " << static_cast<unsigned int>(address) << std::endl;
	for (unsigned int i = 0; i < TRAJECTORY_COUNT; ++i)
		trajectory[i].clear();
}


FR::~FR()
{
}

bool FR::recv(unsigned char* frame)
{
	//if (Debug::FR_DEBUG_OUT){
	//	char str[800];
	//	std::cout << "FR::recv address: " << _itoa(address, str, 16) << " ... ";
	//	for (int i = 0; i < frame[3] + 5; i++)
	//		std::cout << _itoa(frame[i], str, 16) << " ";
	//	std::cout << std::endl;
	//}

	if (frame[0] != address)
		return false;

	switch (frame[2])
	{
		case COMMAND_FR_STOP_PS:
			stopPS(frame);
			return true;
			break;
		case COMMAND_FR_START_PS:
			startPS(frame);
			return true;
			break;
		case COMMAND_FR_SEARCH:
			startSearch(frame);
			return true;
			break;
		case COMMAND_FR_GET_FIRE:
			getFire(frame);
			return true;
			break;
		case COMMAND_FR_MOVE_TO_POINT:
			moveToPoint(frame);
			return true;
			break;
		case COMMAND_FR_GET_STATUS_MOVE_TO_POINT:
			getStatusMoveToPoint(frame);
			return true;
			break;
		case COMMAND_FR_INFO:
			getInfo(frame);
			return false;
			break;
		case COMMAND_FR_INFO_USO:
			getInfoUSO(frame);
			return true;
			break;
		case COMMAND_FR_SET_PRESSURE_TABLE:
			setPressureTable(frame);
			return true;
			break;
		case COMMAND_FR_START_PS_PARAM:
			startPsParam(frame);
			return true;
			break;
		case COMMAND_FR_SET_TRAJECTORY:
			setTrajectory(frame);
			return true;
			break;
		case COMMAND_FR_POVOROTNIY:
			povorotniy(frame);
			return true;
			break;
	}

	return false;
}

void FR::stopPS(unsigned char* frame)
{
	psStart = 0;
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_FR_STOP_PS;
	frame[5] = 0;
	
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << std::endl;
	}
}

void FR::startPS(unsigned char* frame)
{
	psStart = 1;
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_FR_START_PS;
	frame[5] = 0;

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}
}

void FR::startSearch(unsigned char* frame)
{
	searchResult = 0;

	searchStart = 2;
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_FR_SEARCH;
	frame[5] = 0;

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}
}

void FR::getFire(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 11;
	frame[4] = COMMAND_FR_GET_FIRE;
	if (address == 1)
	{
		frame[5] = 0;
		frame[6] = 42;
		frame[7] = 0;
		frame[8] = 5;
		frame[9] = 0;
		frame[10] = 48;
		frame[11] = 0;
		frame[12] = 3;
		frame[13] = 0;
		frame[14] = 22;
		frame[15] = 0;
	}
	else
	{
		frame[5] = 1;
		frame[6] = 44;
		frame[7] = 0;
		frame[8] = 5;
		frame[9] = 1;
		frame[10] = 40;
		frame[11] = 0;
		frame[12] = 3;
		frame[13] = 0;
		frame[14] = 22;
		frame[15] = 0;
	}

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}
}

void FR::getInfo(unsigned char* frame)
{
}

void FR::getInfoUSO(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 4;
	frame[4] = COMMAND_FR_INFO_USO;
	frame[5] = 0;
	if (psStart != 0)
		frame[5] = psStart;
	if (searchStart != 0)
		frame[5] = searchStart;
	frame[6] = searchResult;
	frame[7] = 0;
	frame[8] = 0;

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	searchResult = 2;
	searchStart = 0;
}

void FR::moveToPoint(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 11; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_FR_MOVE_TO_POINT;
}

void FR::getStatusMoveToPoint(unsigned char* frame)
{
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 2;
	frame[4] = COMMAND_FR_GET_STATUS_MOVE_TO_POINT;
	frame[5] = 3;

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}
}

void FR::setPressureTable(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 0;
	frame[4] = COMMAND_FR_SET_PRESSURE_TABLE;
}


void FR::startPsParam(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	psStart = 1;
	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_FR_START_PS_PARAM;
	frame[5] = 0;
}

void FR::povorotniy(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 1;
	frame[4] = COMMAND_FR_POVOROTNIY;
	frame[5] = 0;

	if (Debug::FR_DEBUG_OUT){
		char str[800];
		std::cout << __FUNCTION__ << ": " << _itoa(address, &str[32], 10) << "  ";
		for (int i = 0; i < frame[3] + 5; i++)
			std::cout << _itoa(frame[i], str, 16) << " ";
		std::cout << std::endl;
	}
}


enum SUB_COMMAND
{
	SUB_COMMAND_GENERAL_START = 1,
	SUB_COMMAND_TRAJECTORY_START = 2,
	SUB_COMMAND_SET_POINT = 3,
	SUB_COMMAND_TRAJECTORY_STOP = 4,
	SUB_COMMAND_PRESSURE_TABLE_START = 5,
	SUB_COMMAND_SET_PRESSURE_TABLE = 6,
	SUB_COMMAND_PRESSURE_TABLE_STOP = 7
};

void FR::setTrajectory(unsigned char* frame)
{
	unsigned char result = 0;
	unsigned char subCommand = frame[4];
	switch (subCommand)
	{
		case SUB_COMMAND_GENERAL_START:
			result = subCommandGeneralStart(frame);
			break;
		case SUB_COMMAND_TRAJECTORY_START:
			result = subCommandTrajectoryStart(frame);
			break;
		case SUB_COMMAND_SET_POINT:
			result = subCommandSetPoint(frame);
			break;
		case SUB_COMMAND_TRAJECTORY_STOP:
			result = subCommandTrajectoryStop(frame);
			break;
		case SUB_COMMAND_PRESSURE_TABLE_START:
			result = subCommandPressureTableStart(frame);
			break;
		case SUB_COMMAND_SET_PRESSURE_TABLE:
			result = subCommandSetPressureTable(frame);
			break;
		case SUB_COMMAND_PRESSURE_TABLE_STOP:
			result = subCommandPressureTableStop(frame);
			break;
	}
}

void FR::action()
{
}

unsigned char FR::subCommandGeneralStart(unsigned char* frame)
{
	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6]);
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 3;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_GENERAL_START;
	frame[6] = 0;

	return 0;
}

unsigned char FR::subCommandTrajectoryStart(unsigned char* frame)
{
	unsigned char trajectoryNumber = frame[5];
	unsigned int pointsCount = (frame[6] << 8) + frame[7];
	unsigned short crc = (frame[8] << 8) + frame[9];
	unsigned char result = 0;

	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7], frame[8], frame[9], frame[10], frame[11], frame[12], frame[13], frame[14], frame[15]);
	}

	if ((trajectoryNumber < 4) || (trajectoryNumber > 253) || (pointsCount > Trajectory::MAX_POINTS_COUNT)){
		if (Debug::FR_DEBUG_OUT){
				printf("Error: trajectoryNumber = %u, pointsCount = %u\n", trajectoryNumber, pointsCount);
		}
	
		result = 2;
	}else{
		if (Debug::FR_DEBUG_OUT){
			printf("trajectoryNumber = %u, pointsCount = %u crc = %u\n", trajectoryNumber, pointsCount, crc);
		}

		trajectory[trajectoryNumber].clear();
		trajectory[trajectoryNumber].pointsCount = pointsCount;
		trajectory[trajectoryNumber].crc = crc;
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 4;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_TRAJECTORY_START;
	frame[6] = result;
	frame[7] = trajectoryNumber;

	return result;
}

unsigned char FR::subCommandSetPoint(unsigned char* frame)
{
	unsigned char trajectoryNumber = frame[5];
	unsigned int pointNumber = (frame[6] << 8) + frame[7];
	unsigned int x = (frame[8] << 8) + frame[9];
	unsigned int y = (frame[10] << 8) + frame[11];
	unsigned int nasadok = (frame[12] << 8) + frame[13];
	unsigned int pressureTableNumber = frame[14];
	unsigned char result = 0;

	if ((trajectoryNumber < 4) || (trajectoryNumber > 253) || (pointNumber > trajectory[trajectoryNumber].pointsCount))
		result = 2;
	else
		trajectory[trajectoryNumber].addPoint(x, y, nasadok, pressureTableNumber);

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 6;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_SET_POINT;
	frame[6] = result;
	frame[7] = trajectoryNumber;
	frame[8] = pointNumber >> 8;
	frame[9] = pointNumber;

	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7], frame[8], frame[9]);
	}
	return result;
}

unsigned char FR::subCommandTrajectoryStop(unsigned char* frame)
{
	unsigned char trajectoryNumber = frame[5];
	unsigned char result = 0;

	if ((trajectoryNumber < 4) || (trajectoryNumber > 253))
		result = 2;
	else
		if (trajectory[trajectoryNumber].crc != trajectory[trajectoryNumber].getCrc())
			result = 4;

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 6;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_TRAJECTORY_STOP;
	frame[6] = result;
	frame[7] = trajectoryNumber;
	frame[8] = trajectory[trajectoryNumber].crc >> 8;
	frame[9] = trajectory[trajectoryNumber].crc;

	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7], frame[8], frame[9]);
	}
	return result;
}

unsigned char FR::subCommandPressureTableStart(unsigned char* frame)
{
	unsigned int pressureTableCount = frame[5];
	unsigned short crc = (frame[6] << 8) + frame[7];
	unsigned char result = 0;

	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7]);
	}
	
	if (pressureTableCount > 250)
	{
	if (Debug::FR_DEBUG_OUT){
			printf("Error: pressureTableCount = %u\n", pressureTableCount);
	}
		result = 2;
	}
	else
	{
	if (Debug::FR_DEBUG_OUT){
			printf("pressureTableCount = %u, crc = %u\n", pressureTableCount, crc);
	}
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 3;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_PRESSURE_TABLE_START;
	frame[6] = result;

	return result;
}

unsigned char FR::subCommandSetPressureTable(unsigned char* frame)
{
	unsigned char number = frame[5];
	unsigned char numberf = frame[6];
	unsigned char p1 = frame[7];
	unsigned char a1 = frame[8];
	unsigned char p2 = frame[9];
	unsigned char a2 = frame[10];
	unsigned char p3 = frame[11];
	unsigned char a3 = frame[12];
	unsigned char p4 = frame[13];
	unsigned char a4 = frame[14];
	unsigned char p5 = frame[15];
	unsigned char a5 = frame[16];
	unsigned char result = 0;

	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7], frame[8], frame[9], frame[10], frame[11], frame[12], frame[13], frame[14], frame[15], frame[16]);
		printf("номер = %u, часть = %u, p1 = %u, a1 = %u, p2 = %u, a2 = %u, p3 = %u, a3 = %u, p4 = %u, a4 = %u, p5 = %u, a5 = %u\n", number, numberf, p1, a1, p2, a2, p3, a3, p4, a4, p5, a5);
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 5;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_SET_PRESSURE_TABLE;
	frame[6] = result;
	frame[7] = number;
	frame[8] = numberf;

	return result;
}

unsigned char FR::subCommandPressureTableStop(unsigned char* frame)
{
	unsigned char result = 0;

	if (Debug::FR_DEBUG_OUT){
		printf("%s: %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n", __FUNCTION__, frame[0], frame[1], frame[2], frame[3], frame[4], frame[5], frame[6], frame[7], frame[8], frame[9], frame[10], frame[11], frame[12], frame[13], frame[14], frame[15], frame[16]);
	}

	frame[0] = frame[1];
	frame[1] = address;
	frame[2] = 120;
	frame[3] = 6;
	frame[4] = COMMAND_FR_SET_TRAJECTORY;
	frame[5] = SUB_COMMAND_PRESSURE_TABLE_STOP;
	frame[6] = result;
	frame[7] = 0;
	frame[8] = 1;
	frame[9] = 2;

	return result;
}

