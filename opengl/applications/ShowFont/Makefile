INC = -I../../../src/include -I../../../src/devices -I../../device -I../../ftgl/src/ -I../../freetype/include

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -mmacosx-version-min=10.6 -F../../../build/MacOS/Release/ -framework GUIDOEngine -l freetype -L ../../freetype/objs/.libs/ -l ftgl -L ../../ftgl/src/.libs/ -framework GLUT -framework OpenGL 
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
	g++ -o gltest main.cpp $(INC) $(lib)
