
MAKE 	?= make
CMAKE 	?= cmake
BUILDDIR  ?= builddir
IOSDIR    ?= iosdir
QMAKE    ?= qmake
QTVERS := $(shell $(QMAKE) --version | grep Using | sed -e 's/[^0-9.]//g' | cut -d'.' -f 1)
ifeq ($(QTVERS), 6)
	CMAKEOPT := $(CMAKEOPT) -DQT6=on
else
	CMAKEOPT := $(CMAKEOPT) -DQT6=off
endif

all: 
	$(MAKE) cmake
	$(CMAKE) --build $(BUILDDIR) --config Release

help: 
	@echo "Guido Qt libraries makefile"
	@echo "Available targets are:"
	@echo "  all (default): compile for the current system"
	@echo "  ios          : compile for iOS using an xcode project"
	@echo "Options:"
	@echo "  CMAKEOPT     : options passed to cmake"
	@echo "  GENERATOR    : a cmake generator"


$(BUILDDIR):
	mkdir $(BUILDDIR)
	$(MAKE) cmake
	
cmake: $(BUILDDIR)
	cd $(BUILDDIR) && $(CMAKE) .. $(CMAKEOPT) $(GENERATOR)

ios: $(BUILDDIR)
	$(MAKE) BUILDDIR=$(IOSDIR) CMAKEOPT="-DIOS=on" GENERATOR="-G Xcode"
	
clean:
	rm -rf $(BUILDDIR)
