#include "Callback.h"

#include <assert.h>
#include <string>
#include <boost/asio.hpp>

#include "Define.h"
#include "Extension.h"
#include "Socket.h"
#include "SocketHandler.h"

using namespace boost::asio::ip;

Callback::Callback(CallbackEvent callbackEvent,
				   const void* socket) : callbackEvent(callbackEvent) {
	assert(callbackEvent == CallbackEvent_Connect || callbackEvent == CallbackEvent_Disconnect || callbackEvent == CallbackEvent_SendQueueEmpty);

	socketWrapper = socketHandler.GetSocketWrapper(socket);
}

Callback::Callback(CallbackEvent callbackEvent,
				   const void* socket,
				   const char* data,
				   size_t dataLength) : callbackEvent(callbackEvent) {
	assert(callbackEvent == CallbackEvent_Receive);

	socketWrapper = socketHandler.GetSocketWrapper(socket);
	additionalData[0] = new std::string(data, dataLength);
}

Callback::Callback(CallbackEvent callbackEvent,
				   const void* socket,
				   const void* newSocket,
	   			   const tcp::endpoint& remoteEndPoint) : callbackEvent(callbackEvent) {
	assert(callbackEvent == CallbackEvent_Incoming);

	socketWrapper = socketHandler.GetSocketWrapper(socket);
	additionalData[0] = socketHandler.GetSocketWrapper(newSocket);
	additionalData[1] = new tcp::endpoint(remoteEndPoint);
}

Callback::Callback(CallbackEvent callbackEvent,
				   const void* socket,
				   SM_ErrorType errorType,
				   int errorNumber) : callbackEvent(callbackEvent) {
	assert(callbackEvent == CallbackEvent_Error);

	socketWrapper = socketHandler.GetSocketWrapper(socket);
	additionalData[0] = new SM_ErrorType(errorType);
	additionalData[1] = new int(errorNumber);
}

Callback::~Callback() {
	if (callbackEvent == CallbackEvent_Receive) {
		delete (std::string*) additionalData[0];
	} else if (callbackEvent == CallbackEvent_Incoming) {
		delete (tcp::endpoint*) additionalData[1];
	} else if (callbackEvent == CallbackEvent_Error) {
		delete (SM_ErrorType*) additionalData[0];
		delete (int*) additionalData[1];
	}
}

bool Callback::IsExecutable() {
	if (!socketWrapper) return false;

	switch (socketWrapper->socketType) {
		case SM_SocketType_Tcp: {
			Socket<tcp>* socket = (Socket<tcp>*) socketWrapper->socket;

			switch (callbackEvent) {
				case CallbackEvent_Connect:
					return (socket->connectCallback != NULL);
				case CallbackEvent_Disconnect:
					return (socket->disconnectCallback != NULL);
				case CallbackEvent_Incoming:
					return (socket->incomingCallback != NULL);
				case CallbackEvent_Receive:
					return (socket->receiveCallback != NULL);
				case CallbackEvent_SendQueueEmpty:
					return (socket->sendqueueEmptyCallback != NULL);
				case CallbackEvent_Error:
					return (socket->errorCallback != NULL);
				default:
					return false;
			}
			break;
		}
		case SM_SocketType_Udp: {
			Socket<udp>* socket = (Socket<udp>*) socketWrapper->socket;

			switch (callbackEvent) {
				case CallbackEvent_Connect:
					return (socket->connectCallback != NULL);
				case CallbackEvent_Disconnect:
					return (socket->disconnectCallback != NULL);
				case CallbackEvent_Incoming:
					return (socket->incomingCallback != NULL);
				case CallbackEvent_Receive:
					return (socket->receiveCallback != NULL);
				case CallbackEvent_SendQueueEmpty:
					return (socket->sendqueueEmptyCallback != NULL);
				case CallbackEvent_Error:
					return (socket->errorCallback != NULL);
				default:
					return false;
			}
			break;
		}
	}

	return false;
}

bool Callback::IsValid() {
	if (!socketWrapper) return false;
	if (callbackEvent == CallbackEvent_Incoming && (!additionalData[0] || !additionalData[1])) return false;

	return true;
}

void Callback::Execute() {
	switch (socketWrapper->socketType) {
		case SM_SocketType_Tcp: {
			ExecuteHelper<tcp>();
			break;
		}
		case SM_SocketType_Udp: {
			ExecuteHelper<udp>();
			break;
		}
	}
}

template<class SocketType>
void Callback::ExecuteHelper() {
	if (!IsValid()) return;

	Socket<SocketType>* socket = (Socket<SocketType>*) socketWrapper->socket;

	switch (callbackEvent) {
		case CallbackEvent_Connect:
			if (!socket->connectCallback) return;

			socket->connectCallback->PushCell(socket->smHandle);
			socket->connectCallback->PushCell(socket->smCallbackArg);
			socket->connectCallback->Execute(NULL);

			return;
		case CallbackEvent_Disconnect:
			if (!socket->disconnectCallback) return;

			socket->disconnectCallback->PushCell(socket->smHandle);
			socket->disconnectCallback->PushCell(socket->smCallbackArg);
			socket->disconnectCallback->Execute(NULL);

			return;
		case CallbackEvent_Incoming: {
			if (!socket->incomingCallback) return;

			Socket<SocketType>* socket2 = (Socket<SocketType>*) ((SocketWrapper*)additionalData[0])->socket;
			socket2->smHandle = handlesys->CreateHandle(extension.socketHandleType, socketHandler.GetSocketWrapper(socket2), socket->incomingCallback->GetParentContext()->GetIdentity(), myself->GetIdentity(), NULL);

			socket->incomingCallback->PushCell(socket->smHandle);
			socket->incomingCallback->PushCell(socket2->smHandle);
			socket->incomingCallback->PushString(((typename SocketType::endpoint*)additionalData[1])->address().to_string().c_str());
			socket->incomingCallback->PushCell(((typename SocketType::endpoint*)additionalData[1])->port());
			socket->incomingCallback->PushCell(socket->smCallbackArg);
			socket->incomingCallback->Execute(NULL);

			return;
		}
		case CallbackEvent_Receive: {
			if (!socket->receiveCallback) return;

			size_t strLen = ((std::string*) additionalData[0])->length();
			char* tmp = new char[strLen+1];
			memcpy(tmp, ((std::string*) additionalData[0])->c_str(), strLen+1);

			socket->receiveCallback->PushCell(socket->smHandle);
			socket->receiveCallback->PushStringEx(tmp, strLen+1, SM_PARAM_STRING_COPY|SM_PARAM_STRING_BINARY, 0);
			socket->receiveCallback->PushCell(strLen);
			socket->receiveCallback->PushCell(socket->smCallbackArg);
			socket->receiveCallback->Execute(NULL);

			delete[] tmp;
			return;
		}
		case CallbackEvent_SendQueueEmpty:
			if (!socket->sendqueueEmptyCallback) return;

			socket->sendqueueEmptyCallback->PushCell(socket->smHandle);
			socket->sendqueueEmptyCallback->PushCell(socket->smCallbackArg);
			socket->sendqueueEmptyCallback->Execute(NULL);

			return;
		case CallbackEvent_Error:
			if (!socket->errorCallback) return;

			socket->errorCallback->PushCell(socket->smHandle);
			socket->errorCallback->PushCell(*(SM_ErrorType*) additionalData[0]);
			socket->errorCallback->PushCell(*(int*) additionalData[1]);
			socket->errorCallback->PushCell(socket->smCallbackArg);
			socket->errorCallback->Execute(NULL);

			return;
	}
}
