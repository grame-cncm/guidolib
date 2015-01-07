
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = mac
	QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
	DEST := ..
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
	QMAKEWIN ?= $(shell ../../../qt-win32)
	MSVC 	 ?= $(shell ../../../vs-win32)
	DEST := ../..
else

endif
endif

QMAKE    ?= qmake
PROJECT := GuidoSceneComposer

ROOT    := ..
SIM     := iphoneos

.PHONY : macos ios

all:
	make $(TARGET)

mac:
	make macos
	make ios

macos: macos/$(PROJECT).xcodeproj  $(PROJECT).app 
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

ios: ios/$(PROJECT).xcodeproj  ios/$(PROJECT).app 
	@echo ===================================
	@echo Build $(PROJECT) for ios done
	@echo ===================================

windows: win32/$(PROJECT).vcxproj  $(DEST)/$(PROJECT).exe 
	@echo ===================================
	@echo Build $(PROJECT) for win32
	@echo ===================================


###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngine.shared
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

$(PROJECT).app: ios/$(PROJECT).xcodeproj
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp -R macos/Release/$(PROJECT).app $(DEST) 


###################
# iOS part
###################
ios/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngine.shared
	[ -d ios ] || mkdir ios
	cd ios && $(QMAKEIOS) $(SPEC) $(ROOT)/..

ios/$(PROJECT).app: ios/$(PROJECT).xcodeproj
	xcodebuild -jobs 8 -project  ios/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release

###################
# windows part
###################
win32/$(PROJECT).vcxproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngine.shared
	[ -d win32 ] || mkdir win32
	cd win32 && $(QMAKEWIN) $(SPEC) $(ROOT)/..

../../$(PROJECT).exe:
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/$(PROJECT).vcxproj /maxcpucount:4 /p:Configuration=Release"
	cp -R win32/Release/$(PROJECT).exe $(DEST)

clean:
	rm -rf  $(DEST)/$(PROJECT).* ios macos win32
