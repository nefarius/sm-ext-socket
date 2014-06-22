#ifndef INC_SEXT_DEFINE_H
#define INC_SEXT_DEFINE_H

enum SM_ErrorType {
	SM_ErrorType_EMPTY_HOST = 1,
	SM_ErrorType_NO_HOST,
	SM_ErrorType_CONNECT_ERROR,
	SM_ErrorType_SEND_ERROR,
	SM_ErrorType_BIND_ERROR,
	SM_ErrorType_RECV_ERROR,
	SM_ErrorType_LISTEN_ERROR,
};

enum SM_SocketType {
	SM_SocketType_Tcp = 1,
	SM_SocketType_Udp,
	//SM_SocketType_Icmp,
};

enum SM_SocketOption {
	// SourceMod level options
	SM_SO_ConcatenateCallbacks = 1,
	SM_SO_ForceFrameLock,
	SM_SO_CallbacksPerFrame,
	// Socket level options,
	SM_SO_SocketBroadcast,
	SM_SO_SocketReuseAddr,
	SM_SO_SocketKeepAlive,
	SM_SO_SocketLinger,
	SM_SO_SocketOOBInline,
	SM_SO_SocketSendBuffer,
	SM_SO_SocketReceiveBuffer,
	SM_SO_SocketDontRoute,
	SM_SO_SocketReceiveLowWatermark,
	SM_SO_SocketReceiveTimeout,
	SM_SO_SocketSendLowWatermark,
	SM_SO_SocketSendTimeout,
	// ext options
	SM_SO_DebugMode
};

struct SocketOption {
	SocketOption(SM_SocketOption so, int value) : option(so), value(value) {}
	SM_SocketOption option;
	int value;
};

enum CallbackEvent {
	CallbackEvent_Connect = 0,
	CallbackEvent_Disconnect,
	CallbackEvent_Incoming,
	CallbackEvent_Receive,
	CallbackEvent_SendQueueEmpty,
	CallbackEvent_Error,
};

#endif

