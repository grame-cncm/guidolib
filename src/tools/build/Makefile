#
# 
#

.PHONY : macos win32

TARGET := undefined

system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep MINGW > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	TARGET = macos
else
ifeq ($(system), MINGW)
	TARGET = windows
else
ifeq ($(system), Linux)
	TARGET = linux
endif
endif
endif


# try to detect Visual Studio version used
ifeq ($(TARGET), windows)
 MSVC ?= C:\Program Files (x86)\Microsoft Visual Studio 12.0
 ifneq ($(wildcard "$(MSVC)"), "") 
  WIN32VS ?= Visual Studio 12 2013
  WIN64VS ?= $(WIN32VS) Win64
  endif
 else
 MSVC := C:\Program Files (x86)\Microsoft Visual Studio 10.0
 ifneq ($(wildcard "$(MSVC)"), "") 
  WIN32VS ?= Visual Studio 10
  WIN64VS ?= $(WIN32VS) Win64
 endif
endif

all :
	$(MAKE) $(TARGET)

undefined:
	$(error System is undefined, not target available)

#===============================================================
# building the tools on Mac OS X
#===============================================================
macos : macos/guidotools.xcodeproj mactools

mactools : macos/guidotools.xcodeproj
	xcodebuild -jobs 8 -project macos/guidotools.xcodeproj -target ALL_BUILD -configuration Release

macos/guidotools.xcodeproj : CMakeLists.txt
	[ -d macos ] || mkdir macos
	cd macos && cmake .. -G Xcode -Wno-dev

#===============================================================
# building the tools on windows
#===============================================================
windows :
	$(MAKE) win32
	$(MAKE) win64

win32 : win32/guidotool.sln win32tools 
win64 : win64/guidotool.sln win64tools 

win32tools : win32/guidotool.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/guidotools.sln /maxcpucount:4 /p:Configuration=Release"

win32/guidotool.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G "$(WIN32VS)" -Wno-dev

win64/guidotool.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G "$(WIN64VS)" -Wno-dev

win64tools : win64/guidotool.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win64/guidotools.sln /maxcpucount:4 /p:Configuration=Release"

#===============================================================
# building the tools on linux
#===============================================================
linux : linux/Makefile linuxtools

linuxtools : linux/Makefile
	$(MAKE) -C linux

linux/Makefile : CMakeLists.txt
	[ -d linux ] || mkdir linux
	cd linux && cmake .. -G "Unix Makefiles" -Wno-dev

#===============================================================
clean :
	rm -rf macos win32 win64 linux


