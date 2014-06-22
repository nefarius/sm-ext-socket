/**
 * Socket extension sendto selftest
 */

#include <sourcemod>
#include <socket>

public Plugin:myinfo = {
	name = "socket extension sendto selftest",
	author = "Player",
	description = "basic functionality testing",
	version = "1.0.0",
	url = "http://www.player.to/"
};
 
public OnPluginStart() {
	SocketSetOption(INVALID_HANDLE, DebugMode, 1);
	
	new port = 12346;

	new Handle:socket = SocketCreate(SOCKET_UDP, OnLSocketError);

	SocketBind(socket, "0.0.0.0", port);
	SocketListen(socket, OnLSocketIncoming);

	new Handle:socket2 = SocketCreate(SOCKET_UDP, OnCSocketError);
	//SocketConnect(socket2, OnCSocketConnect, OnCSocketReceive, OnCSocketDisconnect, "127.0.0.1", port);
}

public OnLSocketIncoming(Handle:socket, Handle:newSocket, String:remoteIP[], remotePort, any:arg) {
	PrintToServer("%s:%d connected", remoteIP, remotePort);

	SocketSetReceiveCallback(newSocket, OnChildSocketReceive);
	SocketSetDisconnectCallback(newSocket, OnChildSocketDisconnect);
	SocketSetErrorCallback(newSocket, OnChildSocketError);

	SocketSend(newSocket, "\x00abc\x00def\x01\x02\x03\x04", 12);
	SocketSetSendqueueEmptyCallback(newSocket, OnChildSocketSQEmpty);
}

public OnLSocketError(Handle:socket, const errorType, const errorNum, any:arg) {
	LogError("listen socket error %d (errno %d)", errorType, errorNum);
	CloseHandle(socket);
}

public OnChildSocketReceive(Handle:socket, String:receiveData[], const dataSize, any:arg) {
	// send (echo) the received data back
	//SocketSend(socket, receiveData);
	// close the connection/socket/handle if it matches quit
	//if (strncmp(receiveData, "quit", 4) == 0) CloseHandle(socket);
}

public OnChildSocketSQEmpty(Handle:socket, any:arg) {
	PrintToServer("sq empty");
	CloseHandle(socket);
}

public OnChildSocketDisconnect(Handle:socket, any:arg) {
	// remote side disconnected
	PrintToServer("disc");
	CloseHandle(socket);
}

public OnChildSocketError(Handle:socket, const errorType, const errorNum, any:arg) {
	// a socket error occured

	LogError("child socket error %d (errno %d)", errorType, errorNum);
	CloseHandle(socket);
}

public OnCSocketConnect(Handle:socket, any:arg) {
	// send (echo) the received data back
	//SocketSend(socket, receiveData);
	// close the connection/socket/handle if it matches quit
	//if (strncmp(receiveData, "quit", 4) == 0) CloseHandle(socket);
}

new String:recvBuffer[128];
new recvBufferPos = 0;

public OnCSocketReceive(Handle:socket, String:receiveData[], const dataSize, any:arg) {
	PrintToServer("received %d bytes", dataSize);

	if (recvBufferPos < 512) {
		for (new i=0; i<dataSize && recvBufferPos<sizeof(recvBuffer); i++, recvBufferPos++) {
			recvBuffer[recvBufferPos] = receiveData[i];
		}
	}
	// send (echo) the received data back
	//SocketSend(socket, receiveData);
	// close the connection/socket/handle if it matches quit
	//if (strncmp(receiveData, "quit", 4) == 0) CloseHandle(socket);
}

public OnCSocketDisconnect(Handle:socket, any:arg) {
	new String:cmp[] = "\x00abc\x00def\x01\x02\x03\x04";
	new i;
	for (i=0; i<recvBufferPos && i<12; i++) {
		if (recvBuffer[i] != cmp[i]) {
			PrintToServer("comparison failed");
			break;
		}
	}

	PrintToServer("comparison finished at pos %d/%d", i, recvBufferPos);

	CloseHandle(socket);
}

public OnCSocketError(Handle:socket, const errorType, const errorNum, any:arg) {
	// a socket error occured

	LogError("connect socket error %d (errno %d)", errorType, errorNum);
	CloseHandle(socket);
}

