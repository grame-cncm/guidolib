

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := GraphicsSceneMainWindow
ROOT    := ..
SIM     := iphoneos
LIB     := libGraphicsSceneMainWindow

.PHONY : macos ios

all:
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

clean:
	rm -rf ../../$(LIB)*.a ios macos