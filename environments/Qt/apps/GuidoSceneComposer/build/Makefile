

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := GuidoSceneComposer

ROOT    := ..
SIM     := iphoneos

.PHONY : macos ios

macos: macos/$(PROJECT).xcodeproj  $(PROJECT).app 
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

ios: ios/$(PROJECT).xcodeproj  ios/$(PROJECT).app 
	@echo ===================================
	@echo Build $(PROJECT) for ios done
	@echo ===================================

all:
	make macos
	make ios


###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngineLink.pri
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

$(PROJECT).app: ios/$(PROJECT).xcodeproj
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp -R macos/Release/$(PROJECT).app .. 


###################
# iOS part
###################
ios/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro $(ROOT)/../GUIDOEngineLink.pri
	[ -d ios ] || mkdir ios
	cd ios && $(QMAKEIOS) $(SPEC) $(ROOT)/..

ios/$(PROJECT).app: ios/$(PROJECT).xcodeproj
	xcodebuild -jobs 8 -project  ios/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release

clean:
	rm -rf ../$(PROJECT).app ios macos