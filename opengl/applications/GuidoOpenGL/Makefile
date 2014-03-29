INC = -I../../../src/include -I../../../src/devices -I../../device

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -mmacosx-version-min=10.6 -F../../../build/MacOS/Release/ -framework GUIDOEngine
  INC := $(INC)  -I../../../macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../../../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../../../linux/src $(shell pkg-config --cflags cairo)
endif
endif

gltest : GuidoViewerApp.o GuidoDoc.o
	g++ -o gltest GuidoViewerApp.o GuidoDoc.o

GuidoViewerApp.o : GuidoViewerApp.cpp GuidoViewerApp.h
	g++ -c GuidoViewerApp.cpp $(INC) $(lib)

GuidoDoc.o : GuidoDoc.cpp GuidoDoc.h
	g++ -c GuidoDoc.cpp $(INC) $(lib)
