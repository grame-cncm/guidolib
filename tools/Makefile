
INC = -I../src/engine/include -I../src/engine/devices
LIBDIR = ../build/lib

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -stdlib=libc++ -L$(LIBDIR) -l GUIDOEngine
  INC := $(INC)  -I../platforms/macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  $(LIBDIR)/GUIDOEngine.lib
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

