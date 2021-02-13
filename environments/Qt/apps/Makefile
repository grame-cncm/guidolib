
MAKE 	?= make
CMAKE 	?= cmake
BUILDDIR  ?= builddir
IOSDIR    ?= iosdir
CMAKEOPT  ?= 

all: 
	$(MAKE) cmake
	$(CMAKE) --build $(BUILDDIR) --config Release

help: 
	@echo "Guido Qt applications makefile"
	@echo "Available targets are:"
	@echo "  all (default): compile for the current system"
	@echo "Options:"
	@echo "  BUILDDIR     : the build folder (currently $(BUILDDIR))"
	@echo "  CMAKEOPT     : options passed to cmake"
	@echo "  GENERATOR    : a cmake generator"


$(BUILDDIR):
	mkdir $(BUILDDIR)
	$(MAKE) cmake
	
cmake: $(BUILDDIR)
	cd $(BUILDDIR) && $(CMAKE) .. $(CMAKEOPT) $(GENERATOR)

clean:
	rm -rf $(BUILDDIR)
