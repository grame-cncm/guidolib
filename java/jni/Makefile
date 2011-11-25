
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

guidoengine_guido.o: guidoengine_guido.h
guidoengine_guidodate.o: guidoengine_guidodate.h
guidoengine_guidodrawdesc.o: guidoengine_guidodrawdesc.h
guidoengine_guidoelementinfo.o: guidoengine_guidoelementinfo.h
guidoengine_guidofactory.o: guidoengine_guidofactory.h
guidoengine_guidolayout.o: guidoengine_guidolayout.h
guidoengine_guidopageformat.o: /usr/include/stdio.h /usr/include/_types.h
guidoengine_guidopageformat.o: /usr/include/sys/_types.h
guidoengine_guidopageformat.o: /usr/include/sys/cdefs.h
guidoengine_guidopageformat.o: /usr/include/machine/_types.h
guidoengine_guidopageformat.o: /usr/include/ppc/_types.h
guidoengine_guidopageformat.o: /usr/include/secure/_stdio.h
guidoengine_guidopageformat.o: /usr/include/secure/_common.h
guidoengine_guidopageformat.o: guidoengine_guidopageformat.h
guidoengine_guidopaint.o: guidoengine_guidopaint.h
guidoengine_guidorect.o: guidoengine_guidorect.h
guidoengine_guidoscore.o: guidoengine_guidoscore.h guidoengine_native_paint.h
guidoengine_guidoscore.o: map_collectors.h
guidoengine_guidosegment.o: guidoengine_guidosegment.h
map_collectors.o: map_collectors.h
