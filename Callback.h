#ifndef INC_SEXT_CALLBACK_H
#define INC_SEXT_CALLBACK_H

#include <string>
#include <boost/asio.hpp>

#include "Define.h"

struct SocketWrapper;

class Callback {
public:
	/**
	 * construct a connect, disconnect or sendqueueempty callback
	 */
	Callback(CallbackEvent callbackEvent, const void* socket);

	/**
	 * construct a receive callback
	 */
	Callback(CallbackEvent callbackEvent, const void* socket, const char* data, size_t dataLength);

	/**
	 * construct an incoming callback
	 */
	Callback(CallbackEvent callbackEvent, const void* socket, const void* newSocket, const boost::asio::ip::tcp::endpoint& remoteEndPoint);

	/**
	 * construct an error callback
	 */
	Callback(CallbackEvent callbackEvent, const void* socket, SM_ErrorType errorType, int errorNumber);

	~Callback();

	bool IsExecutable();
	bool IsValid();

	void Execute();

	friend class CallbackHandler;

private:
	template<class SocketType> void ExecuteHelper();

	const CallbackEvent callbackEvent;
	SocketWrapper* socketWrapper;
	const void* additionalData[2];
	
//	volatile bool isExecuting;
};

#endif

