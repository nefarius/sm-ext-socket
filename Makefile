# makefile

SMSDK = /home/m/build/sourcemod-1-0
SOURCEMM = /home/m/build/mmsource-1-4

PROJECT = socket

OBJECTS = Socket.cpp SocketHandler.cpp Callback.cpp CallbackHandler.cpp
OBJECTS_C =
OBJECTS_EXTENSION = Extension.cpp sdk/smsdk_ext.cpp
OBJECTS_TEST = test.cpp

##############################################
### CONFIGURE ANY OTHER FLAGS/OPTIONS HERE ###
##############################################

# symstore.exe add /r /l /f *.* /s c:\symbols /t sm-ext-socket /v version
C_OPT_FLAGS = -O3 -funroll-loops -s -pipe -fno-strict-aliasing
C_DEBUG_FLAGS = -g -ggdb3
CPP = gcc

LINK = -lpthread -Wl,-Bstatic -static-libgcc -lboost_thread -lboost_system -lstdc++ -Wl,-Bdynamic
INCLUDE = -I. -I$(SOURCEMM) -I$(SOURCEMM)/sourcehook -I$(SOURCEMM)/sourcemm \
	-I$(SMSDK)/public -I$(SMSDK)/public/sourcepawn -I$(SMSDK)/public/extensions

CFLAGS = -D_LINUX -DSOURCEMOD_BUILD -Wall -fPIC -m32
CPPFLAGS =

################################################
### DO NOT EDIT BELOW HERE FOR MOST PROJECTS ###
################################################

ifeq "$(DEBUG)" "true"
	BIN_DIR = Debug
	CFLAGS += $(C_DEBUG_FLAGS)
else
	BIN_DIR = Release
	CFLAGS += $(C_OPT_FLAGS)
endif


GCC_VERSION := $(shell $(CPP) -dumpversion >&1 | cut -b1)
ifeq "$(GCC_VERSION)" "4"
	CPPFLAGS += $(CPP_GCC4_FLAGS)
endif

BINARY = $(PROJECT)

OBJ_LINUX := $(OBJECTS:%.cpp=$(BIN_DIR)/%.ox)
OBJ_LINUX_C := $(OBJECTS_C:%.c=$(BIN_DIR)/%.oc)
OBJ_LINUX_EXTENSION := $(OBJECTS_EXTENSION:%.cpp=$(BIN_DIR)/%.ox)
OBJ_LINUX_TEST := $(OBJECTS_TEST:%.cpp=$(BIN_DIR)/%.ox)

$(BIN_DIR)/%.ox: %.cpp
	$(CPP) $(INCLUDE) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

$(BIN_DIR)/%.oc: %.c
	$(CPP) $(INCLUDE) $(CFLAGS) -o $@ -c $<

all:
	mkdir -p $(BIN_DIR)/sdk
	$(MAKE) extension
#	$(MAKE) test

extension: $(OBJ_LINUX) $(OBJ_LINUX_C) $(OBJ_LINUX_EXTENSION)
	$(CPP) $(OBJ_LINUX) $(OBJ_LINUX_C) $(OBJ_LINUX_EXTENSION) $(LINK) -shared -m32 -o$(BIN_DIR)/$(BINARY).ext.so

debug:
	$(MAKE) all DEBUG=true

test: $(OBJ_LINUX) $(OBJ_LINUX_C) $(OBJ_LINUX_TEST)
	$(CPP) $(OBJ_LINUX) $(OBJ_LINUX_C) $(OBJ_LINUX_TEST) $(LINK) -o$(BIN_DIR)/$(BINARY)

default: all

clean:
	rm -rf Release
	rm -rf Debug
