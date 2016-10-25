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
#include <ctime>		// timestamp

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
	USER INPUT MODE:

	cerr << "Baudrate: ";
	int baudrate;
	cin >> baudrate;

	cerr << "Serial port number: ";
	int serialPortNumber;
	cin >> serialPortNumber;
*/

	int baudrate = 9600;
	int serialPortNumber = 16;
	if(!openPort(serialPortNumber, baudrate)) exit(EXIT_FAILURE);
	uint8_t rxBuffer[4096];

	try
	{
		SignalHandler signalHandler;
		signalHandler.setupSignalHandlers();

		while(!signalHandler.gotExitSignal())
		{
			if (int numberOfReceivedBytes = RS232_PollComport(serialPortNumber, rxBuffer, 4096))
			{
				for (uint16_t i = 0; i < numberOfReceivedBytes; i++)
				{
					cout << rxBuffer[i];
					if (rxBuffer[i] == '\n')
					{
						time_t timer;
						if ((timer = time(nullptr)) == -1) exit(EXIT_FAILURE);
						cout << timer << '\t';
					}
					rxBuffer[i] = '\0';
				}
			}
			this_thread::sleep_for(chrono::milliseconds(1));
		}

		cerr << " ... kill signal detected. Exiting dataLogger." << endl;
		exit(EXIT_SUCCESS);
	}
	catch (SignalException& e)
	{
		std::cerr << "SignalException: " << e.what() << std::endl;
	    exit(EXIT_FAILURE);
	}
}
