
FRMWKS =  -framework JavaVM -framework GUIDOEngine  -framework Cocoa  -framework CoreFoundation -F../../cmake/Release/
#LIB =  -lSGUIDOEngine -F../../cmake/Release/
#LIB =   ../../cmake/Release/libSGUIDOEngine.a
INC = -I/System/Library/Frameworks/JavaVM.framework/Headers  -I../../src/include  -I../../src/tools  -I../../macosx/src
CXXFLAGS := $(INC)
TARGET = ../libGUIDOEngine.jnilib

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC) ) guidoengine_native_paint.o

$(TARGET) : $(OBJ) 
	gcc  -dynamiclib $(FRMWKS) $(OBJ) -lstdc++ -o $(TARGET)

guidoengine_native_paint.o : macosx/guidoengine_native_paint.mm guidoengine_native_paint.h
	gcc  $(INC) -x objective-c++ macosx/guidoengine_native_paint.mm -c


install : $(TARGET)
	cp -f $(TARGET) ~/Library/Java/Extensions

clean :
	rm -f $(OBJ) $(TARGET) 
	
depend :
	makedepend $(SRC)

# DO NOT DELETE

