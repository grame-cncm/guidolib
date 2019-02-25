#
# 
#

TARGET := undefined
system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep -e MINGW -e MSYS > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	GENERATOR ?= Xcode
	PREFIX ?= /usr/local
else
ifeq ($(system), MINGW)
	GENERATOR ?= "Visual Studio 14 2015 Win64"
else
	GENERATOR ?= "Unix Makefiles"
	PREFIX ?= /usr/local
endif
endif

BUILDDIR ?= guidodir

#===============================================================
# platform dependant settings
#===============================================================
ifeq ($(GENERATOR), Xcode)
	PROJ = $(BUILDDIR)/guidolib.xcodeproj
	JOBS ?= -jobs 4
endif
ifneq (,$(findstring Makefile, $(GENERATOR)))
	PROJ = $(BUILDDIR)/Makefile
	JOBS ?= -j 4
endif
ifneq (,$(findstring "Visual Studio",$(GENERATOR)))
	PROJ = $(BUILDDIR)/guidolib.sln
	JOBS ?= /maxcpucount:4
endif

CMAKEOPT ?= -DMIDIEXPORT=yes -Wno-dev
BUILDOPT ?= --config Release

#===============================================================
all : $(PROJ)
	cd $(BUILDDIR) && cmake --build .  $(BUILDOPT) -- $(JOBS)
	$(MAKE) java
	$(MAKE) js
	$(MAKE) wasm
	
$(PROJ) : $(BUILDDIR)
	cd $(BUILDDIR) && cmake $(CMAKEOPT) .. -G $(GENERATOR)

cmake : $(BUILDDIR)
	cd $(BUILDDIR) && cmake $(CMAKEOPT) .. 

$(BUILDDIR) :
	mkdir $(BUILDDIR)


libs : $(PROJ)
	cd $(BUILDDIR) && cmake --build .  $(BUILDOPT)

#===============================================================
help :
	@echo "GUIDOEngine makefile - Available targets are:"
	@echo "   base (default): build the GUIDOEngine libraries and tools,"	
	@echo "   all          : build the GUIDOEngine library,"	
	@echo "                        the guido tools,"
	@echo "                        the Guido Qt library and applications,"	
	@echo "                        the guido Java library,"
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
js:
	$(MAKE) -C ../lang/javascript
wasm:
	$(MAKE) -C ../lang/javascript WASM=yes

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
# install target - linux only
#===============================================================
DESTDIR ?=
install: 
	cd $(BUILDDIR) && cmake .. -DCMAKE_INSTALL_PREFIX=$(PREFIX) 
	cmake --build $(BUILDDIR)  $(BUILDOPT) --target install -- $(JOBS)

installLog := $(BUILDDIR)/install_manifest.txt
uninstall: installedfiles = $(shell cat $(installLog))
uninstall: $(installLog)
	rm -f $(installedfiles) $(installLog)

