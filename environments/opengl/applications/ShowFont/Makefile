INC = -I../../../src/include -I../../../src/devices -I../../device -I/usr/local/include/freetype2

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -mmacosx-version-min=10.9 -F../../../../build -framework GUIDOEngine -l freetype -l ftgl -framework GLUT -framework OpenGL 
  INC := $(INC)  -I../../../macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../../../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../../../linux/src $(shell pkg-config --cflags cairo)
endif
endif

fonttest : main.cpp
	g++ -o fonttest main.cpp $(INC) $(lib)
