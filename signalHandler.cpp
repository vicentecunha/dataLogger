#include <signal.h>
#include <errno.h>

#include "signalHandler.hpp"

bool SignalHandler::mbGotExitSignal = false;
bool SignalHandler::mbGotStampSignal = false;

/**
* Default Contructor.
*/
SignalHandler::SignalHandler()
{
}

/**
* Destructor.
*/
SignalHandler::~SignalHandler()
{
}

/**
* Returns the bool flag indicating whether we received a signal
*/
bool SignalHandler::gotExitSignal()
{
    return mbGotExitSignal;
}

bool SignalHandler::gotStampSignal()
{
    return mbGotStampSignal;
}

/**
* Sets the bool flag indicating whether we received a signal
*/
void SignalHandler::setExitSignal(bool _bExitSignal)
{
    mbGotExitSignal = _bExitSignal;
}

void SignalHandler::setStampSignal(bool _bStampSignal)
{
    mbGotStampSignal = _bStampSignal;
}


/**
* Sets signal to true.
*/
void SignalHandler::exitSignalHandler(int _ignored)
{
    mbGotExitSignal = true;
}
void SignalHandler::stampSignalHandler(int _ignored)
{
    mbGotStampSignal = true;
}

/**
* Set up the signal handlers for CTRL-C and CTRL-Z.
*/
void SignalHandler::setupSignalHandlers()
{
    if (signal((int) SIGINT, SignalHandler::exitSignalHandler) == SIG_ERR ||
	    signal((int) SIGTSTP, SignalHandler::stampSignalHandler) == SIG_ERR)
    {
        throw SignalException("!!!!! Error setting up signal handlers !!!!!");
    }
}

