#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include "crc16.h"
#include "USODeviceManager.h"
#include "debug.h"
#include "keys.h"

#include "cmd.h"
#include "config.h"

#include <conio.h>

using namespace std;

#define SEND_RECV_PRINT

HANDLE hCom;

USODeviceManager* pUsoDeviceManager = nullptr;

bool serialConnect(const char* serialName)
{
	hCom = CreateFileA(serialName, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (hCom == INVALID_HANDLE_VALUE)  
	{
		cout << "ERROR: opening com port: " << GetLastError() << endl;
		return false;
	}

	DCB dcb;
	BOOL fSuccess = GetCommState(hCom, &dcb);

	if (!fSuccess) 
	{
		cout << "ERROR: GetCommState: " << GetLastError() << endl;
		CloseHandle(hCom);
		return false;
	}

	dcb.BaudRate = CBR_57600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	fSuccess = SetCommState(hCom, &dcb);

	COMMTIMEOUTS ct;

	ct.ReadIntervalTimeout = 1;
	ct.ReadTotalTimeoutConstant = 0;
	ct.ReadTotalTimeoutMultiplier = 0;
	ct.WriteTotalTimeoutConstant = 0;
	ct.WriteTotalTimeoutMultiplier = 0;

	SetCommTimeouts(hCom,&ct);
	if (!fSuccess) 
	{
		cout << "ERROR: SetCommState: " << GetLastError() << endl;
		CloseHandle(hCom);
		return false;
	}

	return true;
}

void serialDisconnect()
{
	CloseHandle(hCom);
}

const unsigned int BUFFER_SIZE = 1000;
unsigned char buffer[BUFFER_SIZE];

void recv()
{
	unsigned int pos = 0;
	DWORD nWrite = 0;

	while (pos < 5)
	{
		if (ReadFile(hCom, &buffer[pos], 1, &nWrite, NULL))
			pos += nWrite;
	}
		
	unsigned int length = buffer[4] * 256 + buffer[3];
		
	while (pos < length + 7)
	{
		if (ReadFile(hCom, &buffer[pos], 1, &nWrite, NULL))
			pos += nWrite;
	}

	if (Debug::BUS_IN_DEBUG_OUT){
		if ((Debug::BUS_DEVICE_NUMBER == -1) || (buffer[0] == pUsoDeviceManager->devices[Debug::BUS_DEVICE_NUMBER]->getAddress())){
			char str[800];
			std::cout << "recv: ";
			for (int i = 0; i < length + 7; i++)
				std::cout << _itoa(buffer[i], str, 16) << " ";
			std::cout << std::endl;
		}
	}
}

void send()
{
	DWORD nWrite = 0;
	unsigned int length = buffer[4] * 256 + buffer[3];
	unsigned short crc = calcCRC16(buffer, length + 5);
	buffer[length + 5] = crc;
	buffer[length + 6] = crc >> 8;
	WriteFile(hCom,buffer,length + 7,&nWrite,NULL);

	if (Debug::BUS_OUT_DEBUG_OUT){
		if ((Debug::BUS_DEVICE_NUMBER == -1) || (buffer[1] == pUsoDeviceManager->devices[Debug::BUS_DEVICE_NUMBER]->getAddress())){
			char sstr[800];
			std::cout << "send: ";
			for (int i = 0; i < nWrite; i++)
				std::cout << _itoa(buffer[i], sstr, 16) << " ";
			std::cout << std::endl;
		}
	}
}

int getKey(){
	if (kbhit() != 0) {
		int hcode = -1;
		int lcode = getch();
		if ((lcode == 0) || (lcode == 224)){
			hcode = lcode;
			lcode = getch();
		}

//		std::cout << hcode << " : " << lcode << std::endl;

		switch(hcode){
			case -1:
				return lcode;
			case 0:
				return 256 + lcode;
			case 224:
				return 257 + lcode;
		}
	}else
		return -1;
}

int main(int argc, CHAR* argv[])
{
	unsigned int usoDeviceActionNumber = 0;
	try{
		Cmd::init(argc, argv);

		if (Cmd::isParameterExists(Cmd::CMD_CONFIG_FILE))
			Config::init(Cmd::getParameterValue(Cmd::CMD_CONFIG_FILE).c_str());
		else{
			std::cout << "ERROR: Not specified the name of the configuration file. Exit." << std::endl;
			return 0;
		}
	}
	catch(ExceptionXml& e){
		cout << "ExceptionConfig what = " << e.what() << " stingNumber = " << e.getStringNumber() << " columnNumber = " << e.getColumnNumber() << endl;
		return 0;
	}
	
	XmlElement* xmlElement = Config::getElement("com_port");
	if (xmlElement == nullptr){
		std::cout << "imya com porta ne ukazano" << std::endl;
		return 0;
	}

	std::string comPortName;
	if (xmlElement->isAttributeExists("name"))
		comPortName = xmlElement->getAttributeValue("name");
	else{
		std::cout << "imya com porta ne ukazano" << std::endl;
		return 0;
	}
	
	if (!serialConnect(comPortName.c_str()))
		return false;

	pUsoDeviceManager = new USODeviceManager;

	while (true)
	{
		int key = getKey();
		
		if (key != -1){
			for (unsigned int i = 0; i < pUsoDeviceManager->devices.size(); ++i)
				pUsoDeviceManager->devices[i]->keyAction(key);

			switch(key){
				case KEY_ESC:
					return 0;
				case KEY_F1:
					if (Debug::BUS_IN_DEBUG_OUT){
						std::cout << "BUS_IN_DEBUG_OUT = false" << std::endl;
						Debug::BUS_IN_DEBUG_OUT = false;
					}else{
						std::cout << "BUS_IN_DEBUG_OUT = true" << std::endl;
						Debug::BUS_IN_DEBUG_OUT = true;
					}
					break;
				case KEY_F2:
					if (Debug::BUS_OUT_DEBUG_OUT){
						std::cout << "BUS_OUT_DEBUG_OUT = false" << std::endl;
						Debug::BUS_OUT_DEBUG_OUT = false;
					}else{
						std::cout << "BUS_OUT_DEBUG_OUT = true" << std::endl;
						Debug::BUS_OUT_DEBUG_OUT = true;
					}
					break;
				case KEY_F3:
					if (Debug::BK16_DEBUG_OUT){
						std::cout << "BK16_DEBUG_OUT = false" << std::endl;
						Debug::BK16_DEBUG_OUT = false;
					}else{
						std::cout << "BK16_DEBUG_OUT = true" << std::endl;
						Debug::BK16_DEBUG_OUT = true;
					}
					break;
				case KEY_F4:
					if (Debug::FR_DEBUG_OUT){
						std::cout << "FR_DEBUG_OUT = false" << std::endl;
						Debug::FR_DEBUG_OUT = false;
					}else{
						std::cout << "FR_DEBUG_OUT = true" << std::endl;
						Debug::FR_DEBUG_OUT = true;
					}
					break;
				case KEY_F5:
					if (Debug::MONITORING_DEBUG_OUT){
						std::cout << "MONITORING_DEBUG_OUT = false" << std::endl;
						Debug::MONITORING_DEBUG_OUT = false;
					}else{
						std::cout << "MONITORING_DEBUG_OUT = true" << std::endl;
						Debug::MONITORING_DEBUG_OUT = true;
					}
					break;
				case KEY_F6:
					if (Debug::RPK_DEBUG_OUT){
						std::cout << "RPK_DEBUG_OUT = false" << std::endl;
						Debug::RPK_DEBUG_OUT = false;
					}else{
						std::cout << "RPK_DEBUG_OUT = true" << std::endl;
						Debug::RPK_DEBUG_OUT = true;
					}
					break;
				case KEY_F10:
					if (Debug::BUS_DEVICE_NUMBER == pUsoDeviceManager->devices.size() - 1){
						Debug::BUS_DEVICE_NUMBER = -1;
						std::cout << "BUS_DEVICE_NUMBER = -1" << std::endl;
					}else{
						++Debug::BUS_DEVICE_NUMBER;
						std::cout << "BUS_DEVICE_NUMBER = " << static_cast<int>(pUsoDeviceManager->devices[Debug::BUS_DEVICE_NUMBER]->getAddress()) << std::endl;
					}
					break;
			}
		}
		
		pUsoDeviceManager->devices[usoDeviceActionNumber]->action();
		recv();
		if (pUsoDeviceManager->recv(buffer))
			send();

		memset(buffer, 0, BUFFER_SIZE);

		++usoDeviceActionNumber;
		if (usoDeviceActionNumber == pUsoDeviceManager->devices.size())
			usoDeviceActionNumber = 0;
	}
	
	serialDisconnect();

	return true;
}
