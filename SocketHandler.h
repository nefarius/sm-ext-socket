#ifndef INC_SEXT_SOCKETHANDLER_H
#define INC_SEXT_SOCKETHANDLER_H

#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "Socket.h"

struct SocketWrapper {
	SocketWrapper(void* socket, SM_SocketType socketType) : socket(socket), socketType(socketType) {}
	~SocketWrapper();

	void* socket;
	SM_SocketType socketType;
};

class SocketHandler {
public:
	SocketHandler();
	~SocketHandler();

	void Shutdown();

	SocketWrapper* GetSocketWrapper(const void* socket);

	template <class SocketType> Socket<SocketType>* CreateSocket(SM_SocketType st);
	void DestroySocket(SocketWrapper* sw);

	void StartProcessing();
	void StopProcessing();

	//friend class Socket;
	boost::asio::io_service* ioService;

private:
	std::deque<SocketWrapper*> socketList;
	boost::mutex socketListMutex;

	boost::asio::io_service::work* ioServiceWork;

	boost::thread* ioServiceProcessingThread;
	bool ioServiceProcessingThreadInitialized;

	void RunIoService();
};

extern SocketHandler socketHandler;

#endif

