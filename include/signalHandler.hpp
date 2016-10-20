#ifndef __SIGNALHANDLER_H__
#define __SIGNALHANDLER_H_
#include <stdexcept>
using std::runtime_error;

class SignalException : public runtime_error
{
public:
   SignalException(const std::string& _message)
      : std::runtime_error(_message)
   {}
};

class SignalHandler
{
protected:
    static bool mbGotExitSignal;
	static bool mbGotStampSignal;

public:
    SignalHandler();
    ~SignalHandler();

    static bool gotExitSignal();
	static bool gotStampSignal();
    static void setExitSignal(bool _bExitSignal);
	static void setStampSignal(bool _bStampSignal);
	
    void        setupSignalHandlers();
    static void exitSignalHandler(int _ignored);
	static void stampSignalHandler(int _ignored);
};
#endif