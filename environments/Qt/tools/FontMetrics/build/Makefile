

QMAKEIOS ?= $(HOME)/Qt5.4.0/5.4/ios/bin/qmake
QMAKE    ?= qmake
PROJECT := fontmetrics

ROOT    := ..

.PHONY : macos 

all:
	make macos

macos: macos/$(PROJECT).xcodeproj  ../fontmetrics
	@echo ===================================
	@echo Build $(PROJECT) for mac os done
	@echo ===================================

###################
# MacOS part
###################
macos/$(PROJECT).xcodeproj : $(ROOT)/$(PROJECT).pro 
	[ -d macos ] || mkdir macos
	cd macos && $(QMAKE) $(SPEC) $(ROOT)/..

../fontmetrics:
	xcodebuild -jobs 8 -project  macos/$(PROJECT).xcodeproj -target $(PROJECT) -configuration Release
	cp macos/Release/fontmetrics ../fontmetrics 

clean:
	rm -rf ../fontmetrics macos