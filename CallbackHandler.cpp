#include "CallbackHandler.h"

#include <iostream>

#include "Callback.h"

void CallbackHandler::AddCallback(Callback* callback) {
	boost::mutex::scoped_lock l(callbackQueueMutex);

	if (!callback->IsValid()) {
		std::cout << "[SERR] invalid callback (event=" << callback->callbackEvent << ")" << std::endl;
		delete callback;
	} else {
		callbackQueue.push_back(callback);
	}
}

void CallbackHandler::RemoveCallbacks(SocketWrapper* sw) {
	boost::mutex::scoped_lock l(callbackQueueMutex);

	for (std::deque<Callback*>::iterator it=callbackQueue.begin(); it!=callbackQueue.end(); ) {
		if ((*it)->socketWrapper == sw) {
			/*if (!(*it)->isExecuting)*/ delete *it;
			it = callbackQueue.erase(it);
		} else {
			it++;
		}
	}
}

void CallbackHandler::ExecuteQueuedCallbacks() {
	Callback* cb = FetchFirstCallback();
	if (!cb) return;

//	cb->isExecuting = true;
	cb->Execute();
	delete cb;
}

Callback* CallbackHandler::FetchFirstCallback() {
	boost::mutex::scoped_lock l(callbackQueueMutex);

	if (!callbackQueue.empty()) {
		for (std::deque<Callback*>::iterator it=callbackQueue.begin(); it!=callbackQueue.end(); it++) {
			Callback* ret = callbackQueue.front();

			if (!ret->IsExecutable()) {
				std::cout << "[SERR] callback not executable (event=" << ret->callbackEvent << ")" << std::endl;
				continue;
			}

			callbackQueue.erase(it);
			return ret;
		}
	}

	return NULL;
}

CallbackHandler callbackHandler;

