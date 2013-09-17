#pragma once

struct TrajectoryPoint
{
	unsigned short x;
	unsigned short y;
	unsigned short nasadok;
	unsigned short pressureTableNumber;

	TrajectoryPoint(unsigned int _x = 0, unsigned int _y = 0, unsigned int _nasadok = 0, unsigned int _pressureTableNumber = 0)
		:	x(_x), y(_y), nasadok(_nasadok), pressureTableNumber(_pressureTableNumber)
	{
	}
	
	void clear()
	{
		x = 0;
		y = 0;
		nasadok = 0;
		pressureTableNumber = 0;
	}
};

struct PressureTable
{
	static const unsigned int SIZE = 10;
	unsigned short pressure[SIZE];
	unsigned short angle[SIZE];

	PressureTable()
	{
		for (unsigned int i = 0; i < SIZE; ++i)
		{
			pressure[i] = 0;
			angle[i] = 0;
		}
	}

	unsigned short getCrc()
	{
		unsigned short result = 0;
		for (unsigned int i = 0; i < SIZE; ++i)
			result += (pressure[i] = angle[i]);

		return result;
	}

	void clear()
	{
		for (unsigned int i = 0; i < SIZE; ++i)
		{
			pressure[i] = 0;
			angle[i] = 0;
		}
	}
};

struct Trajectory
{
	bool valid;
	
	static const unsigned int MAX_POINTS_COUNT = 2300;
	TrajectoryPoint points[MAX_POINTS_COUNT];
	unsigned int pointsCount;

	unsigned short crc;

	Trajectory()
		:	valid(false), pointsCount(0), crc(0)
	{
	}

	void addPoint(unsigned int x, unsigned int y, unsigned int nasadok, unsigned int pressureTableNumber)
	{
		if (pointsCount < MAX_POINTS_COUNT - 1)
		{
			points[pointsCount].x = x;
			points[pointsCount].y = y;
			points[pointsCount].nasadok = nasadok;
			points[pointsCount].pressureTableNumber = pressureTableNumber;

			pointsCount++;
		}
	}

	unsigned short getCrc()
	{
		unsigned short result = 0;
		for (unsigned int i = 0; i < pointsCount; ++i)
			result += (points[i].x + points[i].y + points[i].nasadok + points[i].pressureTableNumber);

		return result;
	}

	void clear()
	{
		for (unsigned int i = 0; i < MAX_POINTS_COUNT; ++i)
			points[i].clear();

		valid = false;
		crc = 0;
	}
};

//#pragma once
//
//struct Point
//{
//	bool valid;
//
//	unsigned int x;
//	unsigned int y;
//	unsigned int nasadok;
//	unsigned int pressureTableNumber;
//
//	Point(unsigned int _x = 0, unsigned int _y = 0, unsigned int _nasadok = 0, unsigned int _pressureTableNumber = 0)
//		:	valid(false), x(_x), y(_y), nasadok(_nasadok), pressureTableNumber(_pressureTableNumber)
//	{
//	}
//	
//	void clear()
//	{
//		x = 0;
//		y = 0;
//		nasadok = 0;
//		pressureTableNumber = 0;
//
//		valid = false;
//	}
//};
//
//struct PressureTable
//{
//	bool valid;
//	static const unsigned int SIZE = 10;
//	unsigned int pressure[SIZE];
//	unsigned int angle[SIZE];
//
//	PressureTable()
//		:	valid(false)
//	{
//		for (unsigned int i = 0; i < SIZE; ++i)
//		{
//			pressure[i] = 0;
//			angle[i] = 0;
//		}
//	}
//
//	unsigned short getCrc()
//	{
//		unsigned short result = 0;
//		for (unsigned int i = 0; i < SIZE; ++i)
//			result += (pressure[i] = angle[i]);
//
//		return result;
//	}
//
//	void clear()
//	{
//		for (unsigned int i = 0; i < SIZE; ++i)
//		{
//			pressure[i] = 0;
//			angle[i] = 0;
//		}
//			
//		valid = false;
//	}
//};
//
//struct Trajectory
//{
//	bool valid;
//
//	static const unsigned int MAX_POINTS_COUNT = 2300;
//	Point points[MAX_POINTS_COUNT];
//	unsigned int pointsCount;
//
//	static const unsigned int MAX_PRESSURE_TABLE_COUNT = 2300;
//	PressureTable pressureTable[MAX_PRESSURE_TABLE_COUNT];  таблицы коррекции в траекторию не входят
//	unsigned int pressureTableCount;
//
//	unsigned short crc;
//
//	Trajectory(unsigned char _number = 0)
//		:	valid(false), pointsCount(0), pressureTableCount(0), crc(0)
//	{
//	}
//
//	void setPoint(unsigned int pointNumber, unsigned int x, unsigned int y, unsigned int nasadok, unsigned int pressureTableNumber)
//	{
//		if (pointNumber < MAX_POINTS_COUNT)
//		{
//			points[pointNumber].x = x;
//			points[pointNumber].y = y;
//			points[pointNumber].nasadok = nasadok;
//			points[pointNumber].pressureTableNumber = pressureTableNumber;
//
//			points[pointNumber].valid = true;
//		}
//	}
//
//	void setPressureTable(unsigned int tableNumber, unsigned int startIndex, 
//		unsigned int pressure1, unsigned int angle1,
//		unsigned int pressure2, unsigned int angle2,
//		unsigned int pressure3, unsigned int angle3,
//		unsigned int pressure4, unsigned int angle4,
//		unsigned int pressure5, unsigned int angle5)
//	{
//		if (tableNumber < MAX_POINTS_COUNT)
//		{
//			pressureTable[tableNumber].pressure[startIndex] = pressure1;
//			pressureTable[tableNumber].angle[startIndex] = angle1;
//			pressureTable[tableNumber].pressure[startIndex + 1] = pressure2;
//			pressureTable[tableNumber].angle[startIndex + 1] = angle2;
//			pressureTable[tableNumber].pressure[startIndex + 2] = pressure3;
//			pressureTable[tableNumber].angle[startIndex + 2] = angle3;
//			pressureTable[tableNumber].pressure[startIndex + 3] = pressure4;
//			pressureTable[tableNumber].angle[startIndex + 3] = angle4;
//			pressureTable[tableNumber].pressure[startIndex + 4] = pressure5;
//			pressureTable[tableNumber].angle[startIndex + 4] = angle5;
//
//			pressureTable[tableNumber].valid = true;
//		}
//	}
//
//	unsigned short getCrc()
//	{
//		unsigned short result = 0;
//		for (unsigned int i = 0; i < MAX_POINTS_COUNT; ++i)
//			if (points[i].valid)
//				result += (points[i].x + points[i].y + points[i].nasadok + points[i].pressureTableNumber);
//
//		return result;
//	}
//
//	void clear()
//	{
//		for (unsigned int i = 0; i < MAX_POINTS_COUNT; ++i)
//		{
//			points[i].clear();
//			pressureTable[i].clear();
//		}
//
//		valid = false;
//	}
//};
//
