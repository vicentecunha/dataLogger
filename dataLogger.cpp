//========================================================================
// dataLogger.cpp
// Author: Vicente Cunha
// Date: October 2016
// Redirect data from serial port.
//========================================================================

#include <iostream>	// io streams
#include <cstdint>	// int types
#include <cstdlib>	// exit
#include <thread>		// thread
#include <fstream>	// open file
#include <stdlib.h>

#include "signalHandler.hpp"
#include "rs232.h"

using namespace std;

bool openPort(uint8_t serialPortNumber, uint32_t baudrate)
{
	char config[] = "8N1";
	if (RS232_OpenComport(serialPortNumber, baudrate, config))
		return false;
	else
		return true;
}

int main()
{
	cerr << "Starting dataLogger." << endl;

/*
	cerr << "Baudrate: ";
	int baudrate;
	cin >> baudrate;

	cerr << "Serial port number: ";
	int serialPortNumber;
	cin >> serialPortNumber;
*/

	int baudrate = 9600;
	int serialPortNumber = 24;

	cerr << "Opening serial port " << serialPortNumber << " (COM" << serialPortNumber+1 << ")." << endl;
	if(!openPort(serialPortNumber, baudrate))
	{
		exit(EXIT_FAILURE);
	}

	cerr << "Filename to log data: ";
	string filename;
	cin >> filename;
	ofstream logFile;
	logFile.open(filename, ios::out);

	cerr << "Press CTRL+Z at any time to print a stamp line to logFile." << endl;

	uint8_t rxBuffer[4096];
	try
	{
		SignalHandler signalHandler;
		signalHandler.setupSignalHandlers();

		while(!signalHandler.gotExitSignal())
		{
			if (signalHandler.gotStampSignal())
			{
				signalHandler.setStampSignal(false);
				cout    << "========== User Stamp ==========" << endl;
				logFile << "========== User Stamp ==========" << endl;
			}
			if (int numberOfReceivedBytes = RS232_PollComport(serialPortNumber, rxBuffer, 4096))
			{
				for (uint16_t i = 0; i < numberOfReceivedBytes; i++)
				{
					cout    << rxBuffer[i];
					logFile << rxBuffer[i];
					rxBuffer[i] = '\0';
				}
				cout.flush();
			}
			this_thread::sleep_for(chrono::milliseconds(1));
		}

		cout << " ... kill signal detected. Exiting dataLogger." << endl;
		logFile.flush();
		logFile.close();
		exit(EXIT_SUCCESS);
	}
	catch (SignalException& e)
	{
		std::cerr << "SignalException: " << e.what() << std::endl;
	    exit(EXIT_FAILURE);
	}
}
