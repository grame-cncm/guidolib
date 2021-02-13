#
# Guido Engine Makefile
#
#

VERSION := $(shell cat ../version.txt)

system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep -e MINGW -e MSYS > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	PROJ ?= $(BUILDDIR)/guidolib.xcodeproj
else
ifeq ($(system), MINGW)
	PROJ ?= $(BUILDDIR)/Makefile
else
	PROJ ?= $(BUILDDIR)/Makefile
endif
endif

MAKE     ?= make
CMAKE    ?= cmake
BUILDDIR ?= guidodir
CMAKEOPT ?= # -Wno-dev
BUILDOPT ?= --config Release
GENERATOR ?= 

MSLIGHT := ../midisharelight/src

.PHONY: lib

#===============================================================
basic : $(PROJ)
	cd $(BUILDDIR) && $(CMAKE) --build .  $(BUILDOPT)

all : $(PROJ)
	$(MAKE) basic
	$(MAKE) Qt
	$(MAKE) wasm
	
$(PROJ) : CMakeLists.txt
	$(MAKE) cmake

cmake : $(BUILDDIR) $(MSLIGHT)/MidiShareLight.h
	cd $(BUILDDIR) && $(CMAKE) .. $(CMAKEOPT)

$(MSLIGHT)/MidiShareLight.h:
	git submodule update --init

$(BUILDDIR) :
	mkdir $(BUILDDIR)

travis : CMAKEOPT := -Wno-dev
travis : $(PROJ)
	git submodule update --init
	cd $(BUILDDIR) && $(CMAKE) --build .  $(BUILDOPT)
	$(MAKE) Qt


#===============================================================
help :
	@echo "GUIDOEngine makefile - Available targets are:"
	@echo "   basic (default): build the GUIDOEngine library"	
	@echo "                        the guido tools,"
	@echo "                        the guido Java library (when JDK is present),"
	@echo "   all          : build the basic target,"	
	@echo "                        the Guido Qt library and applications,"	
	@echo "                        the guido WASM library."
	@echo "   lib          : build the GUIDOEngine library"	
	@echo "   tools        : build guido tools"	
	@echo "   Qt           : build the Guido Qt library, applications and tools"	
	@echo "   java         : build the jni and java libraries"	
	@echo "   wasm         : build the web assembly library (requires emscripten)"	
	@echo "   ios          : build the GUIDOEngine static library for iOS"	
	@echo "   android      : build the GUIDOEngine static library for Android"	
	@echo "   install      : install the Guido library and public headers"
	@echo "   uninstall    : undo what install did"
	@echo "   localinstall : install the Guido tools to $$HOME/bin"
	@echo "   tags         : extract the current guido tags list"
	@echo "   guido2       : build the embedded svg font"
	@echo "   package      : build a package for distribution"
	@echo 
	@echo "Utilities:"
	@echo "   minimal      : generates a minimal project (the library only)"
	@echo "   cmakeall     : switch the MIN flag off (revert the minimal target)"
	@echo 
	@echo "Options:"
	@echo "   BUILDDIR     : the output folder (currently $(BUILDDIR))"
	@echo "   GENERATOR    : a cmake generator option (currently $(GENERATOR))"
	@echo "   CMAKEOPT     : options passed to cmake for project generation (currently $(CMAKEOPT))"
	@echo "   BUILDOPT     : options passed to cmake for building (currently $(BUILDOPT))"
	@echo 
	@echo "Installation options:"
	@echo " 'DESTDIR'    : the destination directory,"
	@echo " 'PREFIX'     : the destination prefix,"
	@echo "  Note that when using a relative path, it is relative to BUILDDIR ($(BUILDDIR))"


#===============================================================
# building the javascript engine
#===============================================================
wasm: emcc
	$(MAKE) -C ../lang/javascript

emcc:
	@which emcc || (echo emcc compiler must be available from the command line; false)

#===============================================================
# clean the project
#===============================================================
clean :
	rm -rf $(BUILDDIR)
	$(MAKE) -C ../environments/Qt clean

#===============================================================
# building the Guido library
#===============================================================
lib : $(BUILDDIR) $(PROJ)
	cd $(BUILDDIR) && $(CMAKE) --build .  $(BUILDOPT) --target GUIDOEngine 

#===============================================================
# generating a minimal project (the library only)
#===============================================================
minimal : $(BUILDDIR)
	$(MAKE) cmake CMAKEOPT="-DMIN=on"
	$(MAKE) 

cmakeall : $(BUILDDIR)
	$(MAKE) cmake CMAKEOPT="-DMIN=off"
	$(MAKE)

#===============================================================
# building the jni and java library
#===============================================================
java : $(BUILDDIR) $(PROJ)
	cd $(BUILDDIR) && $(CMAKE) --build .  $(BUILDOPT) --target jniGUIDOEngine 

#===============================================================
# building the Qt resources
#===============================================================
Qt :
	$(MAKE) -C ../environments/Qt

#===============================================================
# building the library for Android
#===============================================================
android :
	$(MAKE) -C ../platforms/android/

#===============================================================
# building the library for iOS
#===============================================================
ios : iosdir/guidolib.xcodeproj
	git submodule update --init
	cd iosdir && $(CMAKE) --build .  $(BUILDOPT) --target GUIDOEngine 

iosdir/guidolib.xcodeproj:
	@[ -d iosdir ] || mkdir iosdir
	cd iosdir && $(CMAKE) -DIOS=yes -DMIDIEXPORT=no .. -G Xcode

#===============================================================
# build the embedded svg font
#===============================================================
guido2: ../src/engine/lib/guido2.cpp ../src/svg2carray

../src/engine/lib/guido2.cpp : ../src/guido2.svg
	cd ../src && makecppfont guido2.svg > engine/lib/guido2.cpp

../src/svg2carray : ../src/svg2carray.cpp
	g++ ../src/svg2carray.cpp -o ../src/svg2carray

#===============================================================
# extract the current tags list
#===============================================================
tags:
	grep "kTag" ../src/engine/abstract/Tags.cpp | cut -d'=' -f2 | sed -e 's/ *"\([^"]*\)";/\1/'

#===============================================================
# install targets
#===============================================================
DESTDIR ?=
install: 
	$(CMAKE) --build $(BUILDDIR)  $(BUILDOPT) --target install 

installLog := $(BUILDDIR)/install_manifest.txt
uninstall: installedfiles = $(shell cat $(installLog))
uninstall: $(installLog)
	rm -f $(installedfiles) $(installLog)

localinstall:
	cp bin/guido* bin/midi* $(HOME)/bin

#===============================================================
# packaging
#===============================================================
package: PACK:=GuidoLib-$(VERSION)
package:
	$(MAKE) -C ../environments/Qt
	$(MAKE) cmake CMAKEOPT="-DPREFIX=../$(PACK)"
	$(MAKE) install
	hdiutil create $(PACK).dmg -fs HFS+ -srcfolder $(PACK) -format UDBZ -ov
	@echo "Warning: package target has changed the install PREFIX"
