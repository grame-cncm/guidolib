
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = mac
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
	QMAKEWIN ?= $(shell ../../../qt-win32)
	MSVC 	 ?= $(shell ../../../vs-win32)
else

endif
endif

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := GuidoQt
ROOT    := ..

.PHONY : macos ios 

all:
	make $(TARGET)

mac:
	make macos
	make ios

macos: macos/$(PROJECT).xcodeproj  libGuidoQt.a
	@echo ===================================
	@echo Build GuidoQt for mac os
	@echo ===================================

ios: ios/$(PROJECT).xcodeproj  libGuidoQt-iOS.a
	@echo ===================================
	@echo Build GuidoQt for ios
	@echo ===================================

windows: win32/$(PROJECT).vcxproj GuidoQt.lib
	@echo ===================================
	@echo Build $(PROJECT) for win32 done
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro 
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

libGuidoQt.a:
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
#	cp macos/Release/libGuidoQt.a libGuidoQt.a 
#	cp libGuidoQt.a ../..

###################
# iOS part
###################
ios/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro 
	[ -d ios ] || mkdir ios
	cd ios && $(QMAKEIOS) $(SPEC) $(ROOT)/..

libGuidoQt-iOS.a:
	xcodebuild -jobs 8 -project  ios/$(PROJECT)-iOS.xcodeproj -target $(PROJECT)-iOS -configuration Release

###################
# windows part
###################
win32/$(PROJECT).vcxproj : $(ROOT)/$(PROJECT).pro 
	[ -d win32 ] || mkdir win32
	cd win32 && $(QMAKEWIN) $(SPEC) $(ROOT)/..

GuidoQt.lib:
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/$(PROJECT).vcxproj /maxcpucount:4 /p:Configuration=Release"

clean:
	rm -rf ../../libGuidoQt*.* ios macos win32