

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := GuidoQt
ROOT    := ..
SIM     := iphoneos

.PHONY : macos ios

all:
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
#	cp ios/Release-$(SIM)/libGuidoQt.a libGuidoQt-iOS.a 

clean:
	rm -rf ../../libGuidoQt*.a ios macos