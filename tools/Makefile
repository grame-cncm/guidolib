
INC = -I../src/engine/include -I../src/engine/devices

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -stdlib=libc++ -mmacosx-version-min=10.7 -F../build/MacOS/Release/ -framework GUIDOEngine
  INC := $(INC)  -I../platforms/macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../../platforms/linux/src $(shell pkg-config --cflags cairo)
endif
endif

TOOLS = sizedebug

all: $(TOOLS)

sizedebug : sizedebug.cpp
	$(CXX) sizedebug.cpp $(INC) $(lib) -o sizedebug


clean:
	rm -f $(TOOLS)

