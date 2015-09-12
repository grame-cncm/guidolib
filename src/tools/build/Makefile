#
# 
#

.PHONY : macos win32

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = macos
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
else
ifeq ($(system), MINGW32_NT-6.2)
	TARGET = windows
else
ifeq ($(system), Linux)
	TARGET = linux
endif
endif
endif
endif

WIN32VS := $(shell ../../../build/vs-cmake)
WIN64VS := "$(WIN32VS) Win64"
MSVC    := $(shell ../../../build/vs-path)

all :
	make $(TARGET)

#===============================================================
# building the tools on Mac OS X
#===============================================================
macos : macos/guidotools.xcodeproj mactools

mactools : macos/guidotools.xcodeproj
	xcodebuild -jobs 8 -project macos/guidotools.xcodeproj -target ALL_BUILD -configuration Release

macos/guidotools.xcodeproj : CMakeLists.txt
	[ -d macos ] || mkdir macos
	cd macos && cmake .. -G Xcode

#===============================================================
# building the tools on windows
#===============================================================
windows :
	make win32
	make win64

win32 : win32/guidotool.sln win32tools 
win64 : win64/guidotool.sln win64tools 

win32tools : win32/guidotool.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/guidotools.sln /maxcpucount:4 /p:Configuration=Release"

win32/guidotool.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G "$(WIN32VS)"

win64/guidotool.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G $(WIN64VS)

win64tools : win64/guidotool.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win64/guidotools.sln /maxcpucount:4 /p:Configuration=Release"

#===============================================================
# building the tools on linux
#===============================================================
linux : linux/Makefile linuxtools

linuxtools : linux/Makefile
	make -C linux

linux/Makefile : CMakeLists.txt
	[ -d linux ] || mkdir linux
	cd linux && cmake .. -G "Unix Makefiles"

#===============================================================
clean :
	rm -rf macos win32 win64 linux


