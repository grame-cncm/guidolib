
TARGET 	:= undefined
MAKE 	?= make
BUILDDIR  ?= builddir
IOSDIR    ?= iosdir
CMAKEOPT  ?= 

system	:= $(shell uname -s)
# normalizes MINGW versions
system := $(shell echo $(system) | grep -e MINGW  -e MSYS > /dev/null && echo MINGW || echo $(system))
ifeq ($(system), Darwin)
	TARGET = macos
else
 ifeq ($(system), MINGW)
	TARGET = windows
 else
  ifeq ($(system), Linux)
	TARGET = linux
  endif
 endif
endif

.PHONY: macos ios

all:
	$(MAKE) $(TARGET)

help: 
	@echo "GuidoQt library makefile"
	@echo "Available targets are:"
	@echo "  all (default): compile for the current system"
	@echo "  macos        : compile for macos using an xcode project"
	@echo "  ios          : compile for iOS using an xcode project"
	@echo "  linux        : compile for linux using a Makefile"
	@echo "  windows      : compile for windows"


$(BUILDDIR):
	mkdir $(BUILDDIR)

macos: $(BUILDDIR) $(BUILDDIR)/GuidoQt.xcodeproj
	cd $(BUILDDIR) && cmake --build . --config Release
	
$(BUILDDIR)/GuidoQt.xcodeproj:
	cd $(BUILDDIR) && cmake .. $(CMAKEOPT) -G Xcode

ios: 
	$(MAKE) macos BUILDDIR=$(IOSDIR) CMAKEOPT="-DIOS=on"

linux: $(BUILDDIR) $(BUILDDIR)/Makefile
	cd $(BUILDDIR) && cmake --build . --config Release
	
$(BUILDDIR)/Makefile:
	cd $(BUILDDIR) && cmake ..


clean:
	rm -rf $(BUILDDIR)

undefined:
	$(error System is undefined, not target available)
