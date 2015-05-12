VERSION=2.54

system	:= $(shell uname -s)
ifeq ($(system), Linux)
APPLS := guidoeditor guidoscenecomposer guido2image
else
APPLS := GuidoEditor GuidoSceneComposer guido2image
endif

ROOT := ../../..

APPLS := GuidoEditor GuidoSceneComposer guido2image
TOOLS = guido2midi guido2svg guidogetversion guidogetpagecount guidogetvoicecount

MAKEFILES 	= makeBundleGSC makeBundleGQV
MACOSX_NAME = GuidoQtApplsMacOSX-$(VERSION)
WIN32_NAME	= GuidoQtApplsWin32-$(VERSION)


MACTOOLS  := $(patsubst %, $(ROOT)/src/tools/build/macos/release/%, $(TOOLS))

QT5 := /c/Qt/Qt5.4.0
WIN32QTPATH := $(QT5)/5.4/msvc2010_opengl/
WIN32QT		:= QT5PRINTSUPPORT.DLL QT5WIDGETS.DLL QT5GUI.DLL QT5CORE.DLL QT5XML.DLL ICUIN53.DLL ICUUC53.DLL ICUDT53.DLL
WIN32QT		:= $(patsubst %, $(WIN32QTPATH)/bin/%, $(WIN32QT))
WIN32APPS	:= $(patsubst %, ../apps/%.exe, $(APPLS))
WIN32TOOLS  := $(patsubst %, $(ROOT)/src/tools/build/win32/release/%.exe, $(TOOLS))
WIN32QTPLUGS := qgif.dll qtiff.dll qjpeg.dll
WIN32QTPLATFORMS := qminimal.dll qoffscreen.dll qwindows.dll
WIN32QTPLUGS := $(patsubst %, $(WIN32QTPATH)/plugins/imageformats/%, $(WIN32QTPLUGS))
WIN32QTPLATFORMS := $(patsubst %, $(WIN32QTPATH)/plugins/platforms/%, $(WIN32QTPLATFORMS))

nothing:
	@ echo " Make guidolib Qt packages:"
	@ echo " 'macos' :  deployed binaries package for macosx"
	@ echo " 'win32' :  deployed binaries package for win32. Works with MSys."
	@ echo " 'linux' :  deployed binaries package for Ubuntu."

macapp:
	cp -r ../apps/$(APPL)/$(APPL).app $(MACOSX_NAME)
	macdeployqt $(MACOSX_NAME)/$(APPL).app
	cd $(MACOSX_NAME) && ../localise $(APPL)
	cp ../apps/$(APPL)/rsc/guido.icns $(MACOSX_NAME)/$(APPL).app/Contents/Resources/
	cp $(ROOT)/src/guido2.ttf $(MACOSX_NAME)/$(APPL).app/Contents/Resources/

macos:
	[ -d $(MACOSX_NAME) ] || mkdir $(MACOSX_NAME) && rm -rf $(MACOSX_NAME)/*
	[ -h $(ROOT)/build/MacOS/lib ] || ( cd $(ROOT)/build/MacOS && ln -s Release lib )
	make common PACKAGE_NAME=$(MACOSX_NAME)
	make macapp APPL=GuidoEditor
	make macapp APPL=GuidoSceneComposer
	cp $(MACTOOLS) $(MACOSX_NAME)
	cp -r $(ROOT)/build/MacOS/Release/GUIDOEngine.framework $(MACOSX_NAME)
	cp $(ROOT)/src/guido2.ttf $(MACOSX_NAME)
	cp ../apps/guido2image/guido2image $(MACOSX_NAME)
	cd $(MACOSX_NAME) && ../localise-tool $(TOOLS) guido2image
	hdiutil create -srcfolder $(MACOSX_NAME) -ov $(MACOSX_NAME).dmg

check:
	@echo ----------------------------------------------------------------
	@ echo "Checking WIN32QTPATH..."
	@[ -d "$(WIN32QTPATH)" ] || echo Invalid path to Qt: $(WIN32QTPATH)
	-@[ -d "$(WIN32QTPATH)" ] && echo Make package using Qt path: $(WIN32QTPATH)
	@echo Type 'make win32 WIN32QTPATH=/your/path' to change or edit this makefile.
	@echo ----------------------------------------------------------------
	@[ -d "$(WIN32QTPATH)" ]

samples:
	svn export ../../gmn-examples ./gmn-examples
	tar czf gmn-examples.tgz gmn-examples

linux:
	make version=$(VERSION) -f makedeb

win32: 
	@make -s check
	[ -d $(WIN32_NAME)/plugins/imageformats ] || mkdir -p $(WIN32_NAME)/plugins/imageformats
	[ -d $(WIN32_NAME)/plugins/platforms ] || mkdir -p $(WIN32_NAME)/plugins/platforms
	cp $(WIN32APPS) $(WIN32_NAME)
	cp $(WIN32TOOLS) $(WIN32_NAME)
	cp -f $(ROOT)/build/GUIDOEngine.dll $(WIN32_NAME)
	cp -f $(ROOT)/src/guido2.ttf $(WIN32_NAME)
	cp $(WIN32QT) $(WIN32_NAME)
	cp $(QT5)/vcredist/vcredist_x86.exe $(WIN32_NAME)
	cp qt.conf $(WIN32_NAME)
	cp 	$(WIN32QTPLUGS) $(WIN32_NAME)/plugins/imageformats
	cp 	$(WIN32QTPLATFORMS) $(WIN32_NAME)/plugins/platforms
	make common PACKAGE_NAME=$(WIN32_NAME)

common:
	cp -R $(ROOT)/package/doc $(PACKAGE_NAME)
	rm -rf $(PACKAGE_NAME)/doc/guidolib-?.*
	cp $(ROOT)/changelog.txt $(PACKAGE_NAME)/guido-changelog.txt
	cp -R $(ROOT)/misc/Acknowledgments.txt $(PACKAGE_NAME)
	cp ../apps/GuidoEditor/changelog.txt $(PACKAGE_NAME)/GuidoEditor-changelog.txt
	cp ../apps/guido2image/changelog.txt $(PACKAGE_NAME)/guido2image-changelog.txt
	cp ../apps/GuidoSceneComposer/changelog.txt $(PACKAGE_NAME)/GuidoSceneComposer-changelog.txt
	cp appPack-readme.txt $(PACKAGE_NAME)/readme.txt
	cp -R $(ROOT)/gmn-examples $(PACKAGE_NAME)/gmn-examples
