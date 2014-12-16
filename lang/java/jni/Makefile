
FRMWKS =  -framework JavaVM -framework GUIDOEngine  -framework Cocoa  -framework CoreFoundation -F../../build/MacOS
#LIB =  -lSGUIDOEngine -F../../cmake/Release/
#LIB =   ../../cmake/Release/libSGUIDOEngine.a
INC = -I/System/Library/Frameworks/JavaVM.framework/Headers  -I../../src/engine/include  -I../../src/engine/tools  -I../../macosx/src
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

guidoengine_bitmap_paint.o: guidoengine_bitmap_paint.h
guidoengine_guido.o: guidoengine_guido.h musicxml.h
guidoengine_guido2midiparams.o: guidoengine_guido2midiparams.h javaIDs.h
guidoengine_guidodate.o: guidoengine_guidodate.h javaIDs.h
guidoengine_guidodrawdesc.o: guidoengine_guidodrawdesc.h javaIDs.h
guidoengine_guidoelementinfo.o: guidoengine_guidoelementinfo.h javaIDs.h
guidoengine_guidofactory.o: guidoengine_guidofactory.h javaIDs.h
guidoengine_guidolayout.o: guidoengine_guidolayout.h javaIDs.h
guidoengine_guidopageformat.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
guidoengine_guidopageformat.o: /usr/include/sys/_symbol_aliasing.h
guidoengine_guidopageformat.o: /usr/include/sys/_posix_availability.h
guidoengine_guidopageformat.o: /usr/include/Availability.h
guidoengine_guidopageformat.o: /usr/include/AvailabilityInternal.h
guidoengine_guidopageformat.o: /usr/include/_types.h
guidoengine_guidopageformat.o: /usr/include/sys/_types.h
guidoengine_guidopageformat.o: /usr/include/machine/_types.h
guidoengine_guidopageformat.o: /usr/include/i386/_types.h
guidoengine_guidopageformat.o: /usr/include/sys/_types/_va_list.h
guidoengine_guidopageformat.o: /usr/include/sys/_types/_size_t.h
guidoengine_guidopageformat.o: /usr/include/sys/_types/_null.h
guidoengine_guidopageformat.o: /usr/include/sys/_types/_off_t.h
guidoengine_guidopageformat.o: /usr/include/sys/_types/_ssize_t.h
guidoengine_guidopageformat.o: /usr/include/secure/_stdio.h
guidoengine_guidopageformat.o: /usr/include/secure/_common.h
guidoengine_guidopageformat.o: guidoengine_guidopageformat.h javaIDs.h
guidoengine_guidopaint.o: guidoengine_guidopaint.h javaIDs.h
guidoengine_guidorect.o: guidoengine_guidorect.h javaIDs.h
guidoengine_guidoscore.o: guidoengine_guidoscore.h guidoengine_bitmap_paint.h
guidoengine_guidoscore.o: map_collectors.h javaIDs.h
guidoengine_guidoscoremap.o: guidoengine_guidoscoremap.h javaIDs.h
guidoengine_guidosegment.o: guidoengine_guidosegment.h javaIDs.h
javaIDs.o: javaIDs.h
map_collectors.o: map_collectors.h
musicxml.o: /usr/include/string.h /usr/include/_types.h
musicxml.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
musicxml.o: /usr/include/sys/_symbol_aliasing.h
musicxml.o: /usr/include/sys/_posix_availability.h
musicxml.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
musicxml.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
musicxml.o: /usr/include/sys/_types/_size_t.h /usr/include/sys/_types/_null.h
musicxml.o: /usr/include/sys/_types/_rsize_t.h
musicxml.o: /usr/include/sys/_types/_errno_t.h
musicxml.o: /usr/include/sys/_types/_ssize_t.h /usr/include/strings.h
musicxml.o: /usr/include/secure/_string.h /usr/include/secure/_common.h
musicxml.o: /usr/include/dlfcn.h /usr/include/stdbool.h musicxml.h
