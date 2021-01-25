
TARGET 	:= undefined
MAKE 	  ?= make
QMAKE 	  ?= qmake
BUILDDIR  ?= builddir
IOSDIR    ?= iosdir

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


all:
	$(MAKE) $(TARGET)

help: 
	@echo "GraphicsSceneMainWindow library makefile"
	@echo "Available targets are:"
	@echo "  all (default): compile for the current system"
	@echo "  macos        : compile for macos using an xcode project"
	@echo "(disabled) ios : compile for iOS using an xcode project "
	@echo "  linux        : compile for linux using a Makefile"
	@echo "  windows      : compile for windows"


$(BUILDDIR):
	mkdir $(BUILDDIR)

macos: $(BUILDDIR) $(BUILDDIR)/GraphicsSceneMainWindow.xcodeproj
	xcodebuild -project  $(BUILDDIR)/GraphicsSceneMainWindow.xcodeproj -target GraphicsSceneMainWindow -configuration Release
	
$(BUILDDIR)/GraphicsSceneMainWindow.xcodeproj:
	cd $(BUILDDIR) && $(QMAKE) ../.. -spec macx-xcode

ios:
	@echo "WARNING! ios target is temporarely disabled due to project generation failure with Qt 5.15 and Xcode 12.3"
#	$(MAKE) macos QMAKE=qmake-ios BUILDDIR=$(IOSDIR)

linux: $(BUILDDIR) $(BUILDDIR)/Makefile
	$(MAKE) -C $(BUILDDIR)
	
$(BUILDDIR)/Makefile:
	cd $(BUILDDIR) && $(QMAKE) ../..


clean:
	rm -rf $(BUILDDIR)

undefined:
	$(error System is undefined, not target available)