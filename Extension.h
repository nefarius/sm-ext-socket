#ifndef INC_SEXT_EXTENSION_H
#define INC_SEXT_EXTENSION_H

#include "sdk/smsdk_ext.h"
#include "SocketHandler.h"

class Extension : public SDKExtension, public IHandleTypeDispatch {
public:
	virtual bool SDK_OnLoad(char *error, size_t err_max, bool late);
	virtual void SDK_OnUnload(void);
	void OnHandleDestroy(HandleType_t type, void *object);

	SocketWrapper* GetSocketWrapperByHandle(Handle_t);

	HandleType_t socketHandleType;
};

extern Extension extension;

#endif

