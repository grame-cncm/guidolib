INC = -I../../../src/engine/include -I../../../src/engine/devices -I../../device -I../../ftgl/src -I../../freetype/include

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -mmacosx-version-min=10.6 -F../../../build/opengl/Release/ -framework GUIDOEngine
  #INC := $(INC)  -I../../../macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../../../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../../../linux/src $(shell pkg-config --cflags cairo)
endif
endif

gltest : GuidoViewerApp.o GuidoDoc.o main.o
	g++ -o gltest $(lib) $(shell pkg-config --cflags --libs gl) $(shell pkg-config --cflags --libs freetype2) $(shell pkg-config --cflags --libs ftgl) $(shell pkg-config --cflags --libs glu) -l glut  GuidoViewerApp.o GuidoDoc.o main.o

main.o : main.cpp
	g++ -c main.cpp $(INC) $(lib)

GuidoViewerApp.o : GuidoViewerApp.cpp GuidoViewerApp.h
	g++ -c GuidoViewerApp.cpp $(INC) $(lib)

GuidoDoc.o : GuidoDoc.cpp GuidoDoc.h
	g++ -c GuidoDoc.cpp $(INC) $(lib)
