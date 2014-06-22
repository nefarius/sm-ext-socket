#include "SocketHandler.h"

#include <assert.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "CallbackHandler.h"

using namespace boost::asio::ip;

SocketWrapper::~SocketWrapper() {
	switch (socketType) {
		case SM_SocketType_Tcp:
			delete (Socket<tcp>*) socket;
			break;
		case SM_SocketType_Udp:
			delete (Socket<udp>*) socket;
			break;
		/*case SM_SocketType_Icmp:
			delete (Socket<icmp>*) socket;
			break;*/
	}

	callbackHandler.RemoveCallbacks(this);
}

template Socket<tcp>* SocketHandler::CreateSocket<tcp>(SM_SocketType);
template Socket<udp>* SocketHandler::CreateSocket<udp>(SM_SocketType);

SocketHandler::SocketHandler() : ioServiceProcessingThreadInitialized(false) {
	ioService = new boost::asio::io_service();
}

SocketHandler::~SocketHandler() {
	if (!socketList.empty() || ioServiceProcessingThreadInitialized) {
		Shutdown();
	}
#ifndef WIN32
	delete ioService;
#endif
}

void SocketHandler::Shutdown() {
	boost::mutex::scoped_lock l(socketListMutex);

	for (std::deque<SocketWrapper*>::iterator it=socketList.begin(); it!=socketList.end(); it++) {
		delete *it;
	}

	socketList.clear();

	if (ioServiceProcessingThreadInitialized) StopProcessing();
}

template <class SocketType>
Socket<SocketType>* SocketHandler::CreateSocket(SM_SocketType st) {
	boost::mutex::scoped_lock l(socketListMutex);

	SocketWrapper* sp = new SocketWrapper(new Socket<SocketType>(st), st);
	socketList.push_back(sp);

	return (Socket<SocketType>*) sp->socket;
}

void SocketHandler::DestroySocket(SocketWrapper* sw) {
	assert(sw);

	{ // lock
		boost::mutex::scoped_lock l(socketListMutex);

		for (std::deque<SocketWrapper*>::iterator it=socketList.begin(); it!=socketList.end(); it++) {
			if (*it == sw) {
				socketList.erase(it);
				break;
			}
		}
	} // ~lock

	delete sw;
}

void SocketHandler::StartProcessing() {
	assert(!ioServiceProcessingThreadInitialized);

	ioServiceProcessingThread = new boost::thread(boost::bind(&SocketHandler::RunIoService, this));
	ioServiceProcessingThreadInitialized = true;
}

void SocketHandler::StopProcessing() {
	assert(ioServiceProcessingThreadInitialized);

	ioService->stop();
	delete ioServiceWork;
	ioServiceProcessingThread->join();

	ioServiceProcessingThreadInitialized = false;
	delete ioServiceProcessingThread;
}

void SocketHandler::RunIoService() {
	//boost::asio::io_service::work work(*ioService);
	ioServiceWork = new boost::asio::io_service::work(*ioService);
	ioService->run();
}

SocketWrapper* SocketHandler::GetSocketWrapper(const void* socket) {
	boost::mutex::scoped_lock l(socketListMutex);

	for (std::deque<SocketWrapper*>::iterator it=socketList.begin(); it!=socketList.end(); it++) {
		if ((*it)->socket == socket) return *it;
	}

	return NULL;
}

SocketHandler socketHandler;

