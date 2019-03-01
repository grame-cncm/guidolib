#
# Guido Engine Makefile
#
#

VERSION = 1.6.6

system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep -e MINGW -e MSYS > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	GENERATOR ?= Xcode
	PREFIX ?= /usr/local
	PROJ ?= $(BUILDDIR)/guidolib.xcodeproj
	JOBS ?= -jobs 4
else
ifeq ($(system), MINGW)
	GENERATOR ?= "Visual Studio 14 2015 Win64"
	PROJ ?= $(BUILDDIR)/guidolib.sln
	JOBS ?= /maxcpucount:4
else
	GENERATOR ?= "Unix Makefiles"
	PREFIX ?= /usr/local
	PROJ ?= $(BUILDDIR)/Makefile
	JOBS ?= -j 4
endif
endif

BUILDDIR ?= guidodir
CMAKEOPT ?= -DMIDIEXPORT=yes -Wno-dev
BUILDOPT ?= --config Release

.PHONY: lib

#===============================================================
basic : $(PROJ)
	cd $(BUILDDIR) && cmake --build .  $(BUILDOPT) -- $(JOBS)

all : $(PROJ)
	$(MAKE) basic
	$(MAKE) Qt
	$(MAKE) js
	$(MAKE) wasm
	
$(PROJ) : $(BUILDDIR)
	cd $(BUILDDIR) && cmake $(CMAKEOPT) .. -G $(GENERATOR)

cmake : $(BUILDDIR)
	cd $(BUILDDIR) && cmake $(CMAKEOPT) .. 

$(BUILDDIR) :
	mkdir $(BUILDDIR)

travis : CMAKEOPT := -DMIDIEXPORT=no -Wno-dev
travis : $(PROJ)
	cd $(BUILDDIR) && cmake --build .  $(BUILDOPT) -- $(JOBS)
	$(MAKE) Qt


#===============================================================
help :
	@echo "GUIDOEngine makefile - Available targets are:"
	@echo "   basic (default): build the GUIDOEngine library"	
	@echo "                        the guido tools,"
	@echo "                        the guido Java library (when JDK is present),"
	@echo "   all          : build the basic target,"	
	@echo "                        the Guido Qt library and applications,"	
	@echo "                        the guido Javascript library."
	@echo "   lib          : build the GUIDOEngine library"	
	@echo "   tools        : build guido tools"	
	@echo "   Qt           : build the Guido Qt library, applications and tools"	
	@echo "   java         : build the jni and java libraries"	
	@echo "   js           : build the javascript library (requires emscripten)"	
	@echo "   wasm         : build the web assembly library (requires emscripten)"	
	@echo "   ios          : build the GUIDOEngine static library for iOS"	
	@echo "   android      : build the GUIDOEngine static library for Android"	
	@echo "   install      : install the Guido library and public headers"
	@echo "   uninstall    : undo what install did"
	@echo 
	@echo "Options:"
	@echo "   BUILDDIR     : the output folder (currently $(BUILDDIR))"
	@echo "   GENERATOR    : a cmake generator (currently $(GENERATOR))"
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
js: emcc
	$(MAKE) -C ../lang/javascript
wasm: emcc
	$(MAKE) -C ../lang/javascript WASM=yes

emcc:
	which emcc || echo emcc compiler must be available crom the command line

#===============================================================
# clean the project
#===============================================================
clean :
	rm -rf $(BUILDDIR)
	$(MAKE) -C ../environments/Qt clean

#===============================================================
# building the Guido library
#===============================================================
lib :
	cd $(BUILDDIR) && cmake --build .  $(BUILDOPT) --target GUIDOEngine -- $(JOBS)

#===============================================================
# building the jni and java library
#===============================================================
java :
	cd $(BUILDDIR) && cmake --build .  $(BUILDOPT) --target jniGUIDOEngine -- $(JOBS)

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
	cd iosdir && cmake --build .  $(BUILDOPT) --target GUIDOEngine -- $(JOBS)

iosdir/guidolib.xcodeproj:
	@[ -d iosdir ] || mkdir iosdir
	cd iosdir && cmake -DIOS=yes -DMIDIEXPORT=no .. -G Xcode

#===============================================================
# install targets
#===============================================================
DESTDIR ?=
install: 
	cd $(BUILDDIR) && cmake .. -DCMAKE_INSTALL_PREFIX=$(PREFIX) 
	cmake --build $(BUILDDIR)  $(BUILDOPT) --target install -- $(JOBS)

installLog := $(BUILDDIR)/install_manifest.txt
uninstall: installedfiles = $(shell cat $(installLog))
uninstall: $(installLog)
	rm -f $(installedfiles) $(installLog)

#===============================================================
# packaging
#===============================================================
package: PACK:=GuidoLib-$(VERSION)
package: 
	make install PREFIX=../$(PACK)
	hdiutil create $(PACK).dmg -fs HFS+ -srcfolder $(PACK) -format UDBZ -ov

