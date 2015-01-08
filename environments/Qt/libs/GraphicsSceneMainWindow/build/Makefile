
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = mac
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
else

endif
endif

QMAKEWIN ?= /C/Qt/Qt5.4.0/5.4/msvc2010_opengl/bin/qmake
QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := GraphicsSceneMainWindow
ROOT    := ..
SIM     := iphoneos
LIB     := libGraphicsSceneMainWindow
MSVC 	:= C:\Program Files (x86)\Microsoft Visual Studio 10.0

.PHONY : macos ios win32 $(LIB).a $(LIB)-iOS.a $(LIB).lib

all:
	make $(TARGET)

mac:
	make macos
	make ios

macos: macos/$(PROJECT).xcodeproj  $(LIB).a 
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

ios: ios/$(PROJECT)-iOS.xcodeproj $(LIB)-iOS.a
	@echo ===================================
	@echo Build $(PROJECT) for ios done
	@echo ===================================

windows: win32/$(PROJECT).vcxproj $(LIB).lib
	@echo ===================================
	@echo Build $(PROJECT) for win32 done
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro 
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

$(LIB).a:
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release

###################
# iOS part
###################
ios/$(PROJECT)-iOS.xcodeproj : $(ROOT)/$(PROJECT).pro 
	[ -d ios ] || mkdir ios
	cd ios && $(QMAKEIOS) $(SPEC) $(ROOT)/..

$(LIB)-iOS.a:
	xcodebuild -jobs 8 -project  ios/$(PROJECT)-iOS.xcodeproj -target $(PROJECT)-iOS -configuration Release


###################
# windows part
###################
win32/$(PROJECT).vcxproj : $(ROOT)/$(PROJECT).pro 
	[ -d win32 ] || mkdir win32
	cd win32 && $(QMAKEWIN) $(SPEC) $(ROOT)/..

$(LIB).lib:
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/$(PROJECT).vcxproj /maxcpucount:4 /p:Configuration=Release"

	
clean:
	rm -rf ../../$(LIB)*.* ios macos win32