
MAKE 	?= make
CMAKE 	?= cmake
BUILDDIR  ?= builddir
IOSDIR    ?= iosdir
CMAKEOPT  ?= 

system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep -e MINGW  -e MSYS > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	MAKEFILE?= GuidoQt.xcodeproj
endif
ifeq ($(system), MINGW)
	MAKEFILE ?= Makefile
else
	MAKEFILE ?= Makefile
endif

all: $(BUILDDIR)/$(MAKEFILE)
	$(CMAKE) --build $(BUILDDIR) --config Release

help: 
	@echo "GuidoQt library makefile"
	@echo "Available targets are:"
	@echo "  all (default): compile for the current system"
	@echo "  ios          : compile for iOS using an xcode project"
	@echo "Options:"
	@echo "  CMAKEOPT     : options passed to cmake"
	@echo "  GENERATOR    : a cmake generator"


$(BUILDDIR):
	mkdir $(BUILDDIR)
	
$(BUILDDIR)/$(MAKEFILE): $(BUILDDIR) CMakeLists.txt
	cd $(BUILDDIR) && $(CMAKE) .. $(CMAKEOPT) $(GENERATOR)

ios: $(BUILDDIR)/$(MAKEFILE)
	$(MAKE) BUILDDIR=$(IOSDIR) CMAKEOPT="-DIOS=on" GENERATOR="-G Xcode"
	
clean:
	rm -rf $(BUILDDIR)
