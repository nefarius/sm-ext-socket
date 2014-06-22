#ifndef INC_SEXT_CALLBACKHANDLER_H
#define INC_SEXT_CALLBACKHANDLER_H

#include <deque>
#include <boost/thread.hpp>

class Callback;
struct SocketWrapper;

/**
 * manages the callbacks for asynchronous operations.
 *
 * @note No destructor, objects will be freed by ~SocketHandler -> ~SocketWrapper -> CallbackHandler::RemoveCallbacks
 */
class CallbackHandler {
public:
	void AddCallback(Callback* callback);
	void RemoveCallbacks(SocketWrapper* sw);
	void ExecuteQueuedCallbacks();

private:
	Callback* FetchFirstCallback();

	std::deque<Callback*> callbackQueue;
	boost::mutex callbackQueueMutex;
};

extern CallbackHandler callbackHandler;

#endif

