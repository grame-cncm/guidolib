#
# 
#

.PHONY : macos ios Qt

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = mac
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
else
ifeq ($(system), Linux)
	TARGET = linux
endif
endif
endif


OUT  := GUIDOEngine.framework GUIDOEngine.tgz libGUIDOEngine.a
MACX := MacOS
IOS  := iOS
WIN32VS := "Visual Studio 10"
WIN64VS := "Visual Studio 10 Win64"
MSVC := C:\Program Files (x86)\Microsoft Visual Studio 10.0
LINUX := linux

all :
	make $(TARGET)
	make tools
	make Qt
	make jni

#===============================================================
# building the library on Mac OS X
#===============================================================
.PHONY : macos ios $(MACX)/Release/GUIDOEngine.framework $(IOS)/Release-iphoneos/libGUIDOEngine.a

mac :
	make macos
	make ios	

macos : $(MACX)/guidolib.xcodeproj GUIDOEngine.framework GUIDOEngine.tgz

GUIDOEngine.framework :  $(MACX)/guidolib.xcodeproj  $(MACX)/Release/GUIDOEngine.framework
	rm -rf GUIDOEngine.framework
	cp -R $(MACX)/Release/GUIDOEngine.framework .

$(MACX)/Release/GUIDOEngine.framework :
	xcodebuild -jobs 8 -project $(MACX)/guidolib.xcodeproj -target ALL_BUILD -configuration Release

$(MACX)/guidolib.xcodeproj : CMakeLists.txt
	[ -d $(MACX) ] || mkdir $(MACX)
	cd $(MACX) && cmake .. -G Xcode -DMIDIEXPORT=yes

GUIDOEngine.tgz : GUIDOEngine.framework
	tar czf GUIDOEngine.tgz GUIDOEngine.framework

#===============================================================
# building the library for iOS
#===============================================================
ios : $(IOS)/guidolib.xcodeproj libGUIDOEngine.a

libGUIDOEngine.a : $(IOS)/guidolib.xcodeproj $(IOS)/Release-iphoneos/libGUIDOEngine.a
	cp $(IOS)/Release-iphoneos/libGUIDOEngine.a .

$(IOS)/Release-iphoneos/libGUIDOEngine.a :
	xcodebuild -jobs 8 -project $(IOS)/guidolib.xcodeproj -target ALL_BUILD -configuration Release
 
$(IOS)/guidolib.xcodeproj : CMakeLists.txt
	[ -d $(IOS) ] || mkdir $(IOS)
	cd $(IOS) && cmake .. -G Xcode -DMIDIEXPORT=no -DIOS=yes

macclean :
	rm -rf $(IOS) $(MACX) $(OUT)

#===============================================================
# building the library on windows
#===============================================================
.PHONY : win32 win64 win32/Release/GUIDOEngine.dll win64/Release/GUIDOEngine64.dll

windows :
	make win32
	make win64

win32 : win32/guidolib.sln GUIDOEngine.dll GUIDOEngine.lib 

GUIDOEngine.lib : GUIDOEngine.dll

GUIDOEngine.dll : win32/Release/GUIDOEngine.dll
	cp win32/Release/GUIDOEngine.dll .
	cp win32/Release/GUIDOEngine.lib .

win32/Release/GUIDOEngine.dll : win32/guidolib.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/guidolib.sln /maxcpucount:4 /p:Configuration=Release"

win32/guidolib.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G $(WIN32VS) -DMIDIEXPORT=yes


win64 : win64/guidolib.sln GUIDOEngine64.dll GUIDOEngine64.lib 

GUIDOEngine64.lib : GUIDOEngine64.dll

GUIDOEngine64.dll : win64/Release/GUIDOEngine64.dll
	cp win64/Release/GUIDOEngine64.dll .
	cp win64/Release/GUIDOEngine64.lib .

win64/Release/GUIDOEngine64.dll : win64/guidolib.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win64/guidolib.sln /maxcpucount:4 /p:Configuration=Release"

win64/guidolib.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G $(WIN64VS) -DMIDIEXPORT=yes

winclean:
	rm -rf win32 win64 GUIDOEngine*.lib GUIDOEngine.dll

#===============================================================
# building the library on linux
#===============================================================
.PHONY : $(LINUX)/libGUIDOEngine.so

linux : $(LINUX)/Makefile libGUIDOEngine.so

libGUIDOEngine.so :  $(LINUX)/Makefile  $(LINUX)/libGUIDOEngine.so
	rm -rf GUIDOEngine.so
	cp -R $(LINUX)/libGUIDOEngine.so .

$(LINUX)/libGUIDOEngine.so : CMakeLists.txt
	make -C $(LINUX)
	
$(LINUX)/Makefile : CMakeLists.txt
	[ -d $(LINUX) ] || mkdir $(LINUX)
	cd $(LINUX) && cmake .. -G "Unix Makefiles" -DMIDIEXPORT=yes

linuxclean:
	rm -rf $(LINUX)

#===============================================================
# building the javascript engine
#===============================================================
js:
	make -C ../lang/javascript


#===============================================================
# building the Guido tools
#===============================================================
clean :
	make macclean
	make winclean
	make linuxclean
	make -C ../src/tools/build clean
	make -C ../environments/Qt clean
	make -C ../lang/java/build clean

tools :
	make -C ../src/tools/build

#===============================================================
# building the Qt resources
#===============================================================
Qt :
	make -C ../environments/Qt

#===============================================================
# building the jni library
#===============================================================
jni :
	make -C ../lang/java/build