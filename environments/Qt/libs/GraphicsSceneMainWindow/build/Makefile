
TARGET 	:= undefined
MAKE 	  ?= make
CMAKE 	?= cmake
#QMAKE 	  ?= qmake
BUILDDIR  ?= builddir
IOSDIR    ?= iosdir

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
#	$(MAKE) $(TARGET)

help: 
	@echo "GraphicsSceneMainWindow library makefile"
	@echo "Available targets are:"
	@echo "  all (default): compile for the current system"
	@echo "  ios          : compile for iOS using an xcode project"
	@echo "Options:"
	@echo "  CMAKEOPT     : options passed to cmake"
	@echo "  GENERATOR    : a cmake generator"
# 	@echo "  macos        : compile for macos using an xcode project"
# 	@echo "(disabled) ios : compile for iOS using an xcode project "
# 	@echo "  linux        : compile for linux using a Makefile"
# 	@echo "  windows      : compile for windows"


$(BUILDDIR):
	mkdir $(BUILDDIR)

$(BUILDDIR)/$(MAKEFILE): $(BUILDDIR) CMakeLists.txt
	cd $(BUILDDIR) && $(CMAKE) .. $(CMAKEOPT) $(GENERATOR)

ios: $(BUILDDIR)/$(MAKEFILE)
	$(MAKE) BUILDDIR=$(IOSDIR) CMAKEOPT="-DIOS=on" GENERATOR="-G Xcode"

# macos: $(BUILDDIR) $(BUILDDIR)/GraphicsSceneMainWindow.xcodeproj
# 	xcodebuild -project  $(BUILDDIR)/GraphicsSceneMainWindow.xcodeproj -target GraphicsSceneMainWindow -configuration Release
# 	
# $(BUILDDIR)/GraphicsSceneMainWindow.xcodeproj:
# 	cd $(BUILDDIR) && $(QMAKE) ../.. -spec macx-xcode
# 
# ios:
# 	@echo "WARNING! ios target is temporarely disabled due to project generation failure with Qt 5.15 and Xcode 12.3"
# #	$(MAKE) macos QMAKE=qmake-ios BUILDDIR=$(IOSDIR)
# 
# windows: $(BUILDDIR) $(BUILDDIR)/GraphicsSceneMainWindow.vcxproj
# 	msbuild $(BUILDDIR)/GraphicsSceneMainWindow.vcxproj -property:Configuration=Release
# 
# $(BUILDDIR)/GraphicsSceneMainWindow.vcxproj:
# 	cd $(BUILDDIR) && qmake ../..
# 
# linux: $(BUILDDIR) $(BUILDDIR)/Makefile
# 	$(MAKE) -C $(BUILDDIR)
# 	
# $(BUILDDIR)/Makefile:
# 	cd $(BUILDDIR) && $(QMAKE) ../..
# 

clean:
	rm -rf $(BUILDDIR)

# undefined:
# 	$(error System is undefined, not target available)