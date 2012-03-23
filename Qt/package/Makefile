VERSION=2.41

system	:= $(shell uname -s)
ifeq ($(system), Linux)
APPLS := guidoeditor guidoscenecomposer guido2image
else
APPLS := GuidoEditor GuidoSceneComposer guido2image
endif


APPLS := GuidoEditor GuidoSceneComposer guido2image
TOOLS = guido2midi guido2svg guidogetversion guidogetpagecount guidogetvoicecount

MAKEFILES 	= makeBundleGSC makeBundleGQV
MACOSX_NAME = GuidoQtApplsMacOSX-$(VERSION)
WIN32_NAME	= GuidoQtApplsWin32-$(VERSION)


MACTOOLS  := $(patsubst %, ../../tools/cmake/release/%, $(TOOLS))

WIN32QTPATH := /c/Qt/2010.05/qt
WIN32QT		:= QtCore4.dll QtGui4.dll QtXml4.dll
WIN32QT		:= $(patsubst %, $(WIN32QTPATH)/bin/%, $(WIN32QT))
WIN32APPS	:= $(patsubst %, ../bin/%.exe, $(APPLS))
WIN32TOOLS  := $(patsubst %, ../../tools/cmake/release/%.exe, $(TOOLS))
WIN32QTPLUGS := qgif4.dll qtiff4.dll qjpeg4.dll
WIN32QTPLUGS := $(patsubst %, $(WIN32QTPATH)/plugins/imageformats/%, $(WIN32QTPLUGS))


nothing:
	echo $(WIN32QT)
	@ echo " Make guidolib Qt packages:"
	@ echo " 'macos' :  deployed binaries package for macosx"
	@ echo " 'win32' :  deployed binaries package for win32. Works with MSys."
	@ echo " 'linux' :  deployed binaries package for Ubuntu."

macapp:
	cp -r ../bin/$(APPL).app $(MACOSX_NAME)
	macdeployqt $(MACOSX_NAME)/$(APPL).app
	cd $(MACOSX_NAME) && ../localise $(APPL)
	cp ../$(APPL)/rsc/guido.icns $(MACOSX_NAME)/$(APPL).app/Contents/Resources/
	cp ../../src/guido2.ttf $(MACOSX_NAME)/$(APPL).app/Contents/Resources/

macos:
	[ -d $(MACOSX_NAME) ] || mkdir $(MACOSX_NAME) && rm -rf $(MACOSX_NAME)/*
	make common PACKAGE_NAME=$(MACOSX_NAME)
	make macapp APPL=GuidoEditor
	make macapp APPL=GuidoSceneComposer
	cp $(MACTOOLS) $(MACOSX_NAME)
	cd $(MACOSX_NAME) && ../localise-tool $(TOOLS) guido2image
	cp -r ../../cmake/Release/GUIDOEngine.framework $(MACOSX_NAME)
	cp ../../src/guido2.ttf $(MACOSX_NAME)
	cp ../bin/guido2image $(MACOSX_NAME)
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
	[ -d $(WIN32_NAME)/imageformats ] || mkdir -p $(WIN32_NAME)/imageformats
	cp $(WIN32APPS) $(WIN32_NAME)
	cp $(WIN32TOOLS) $(WIN32_NAME)
	cp -f ../../cmake/release/GUIDOEngine.dll $(WIN32_NAME)
	cp -f ../../src/guido2.ttf $(WIN32_NAME)
	cp $(WIN32QT) $(WIN32_NAME)
	cp *80.* $(WIN32_NAME)
	cp 	$(WIN32QTPLUGS) $(WIN32_NAME)/imageformats
	make common PACKAGE_NAME=$(WIN32_NAME)

doc:

common:
	[ -d $(PACKAGE_NAME)/doc ] || mkdir -p $(PACKAGE_NAME)/doc
	cp "../../doc/GUIDO-Music Notation Format.pdf" $(PACKAGE_NAME)/doc
	cp "../../doc/GUIDO-Format_de_Notation_Musicale.pdf" $(PACKAGE_NAME)/doc
	cp "../../doc/GUIDO_Additional_Specifications_1.3.8.pdf" $(PACKAGE_NAME)/doc
	cp "../../doc/refcard/GMNRefCard.pdf" $(PACKAGE_NAME)/doc
	cp ../../changelog.txt $(PACKAGE_NAME)/guido-changelog.txt
	cp ../GuidoEditor/changelog.txt $(PACKAGE_NAME)/GuidoEditor-changelog.txt
	cp ../guido2image/changelog.txt $(PACKAGE_NAME)/guido2image-changelog.txt
	cp ../GuidoSceneComposer/changelog.txt $(PACKAGE_NAME)/GuidoSceneComposer-changelog.txt
	cp appPack-readme.txt $(PACKAGE_NAME)/readme.txt
	make commongit

commongit:
	cd ../../doc/html && git archive -o __tmp__.tgz --prefix=../../Qt/package/$(PACKAGE_NAME)/doc/html/ HEAD && tar xzfP __tmp__.tgz && rm __tmp__.tgz
	cd ../../gmn-examples && git archive -o __tmp__.tgz --prefix=../Qt/package/$(PACKAGE_NAME)/gmn-examples/ HEAD && tar xzfP __tmp__.tgz && rm __tmp__.tgz
