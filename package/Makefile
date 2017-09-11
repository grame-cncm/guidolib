
.PHONY : doc

VERSION := 1.6.6
DOCVERS := 1.54
DEST    := .

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  bin =  ../build/MacOS/Release
else
ifeq ($(system), Linux)
  bin =  ../cmake/Release
else
  bin =  ../build/win*/Release
endif
endif

DEVKIT   := guidolib-$(VERSION)-devkit
DOCPACK  := guidolib-$(VERSION)-doc
MACOSLIB := $(DEVKIT)-macosx
LINUXLIB := $(DEVKIT)-linux-x86
WINLIB   := $(DEVKIT)-windows
QTLIB    := $(DEVKIT)-Qt
SRC 	 := guidolib-$(VERSION)-src

files   := ../changelog.txt license.txt readme.txt ../src/guido2.ttf ../src/guido2.svg ../misc/Acknowledgments.txt

modules := guidolib

######### documentation files 
DOC		:=  ../doc/GUIDO-Format_de_Notation_Musicale.pdf \
			../doc/GUIDO-Music-Notation-Format.pdf \
			../doc/GUIDO_Additional_Specifications_1*.pdf \
			../doc/refcard/GMNRefCard.pdf \
			../doc/refcard/latex/RefCardsParams.pdf \
			../doc/refcard/latex/RefCardsTags.pdf

SAMPLES := $(wildcard ../src/samples/*.cpp ../validate/guido*.cpp ../src/engine/tools/Null*.h)

######### common include files 
cominc  :=  $(wildcard ../src/engine/include/*.h ../src/engine/devices/SVG*.h )

######### win32 devices include files 
win32inc := GDeviceWin32.h GSystemWin32.h GFontWin32.h  GDeviceWin32GDIPlus.h GSystemWin32GDIPlus.h GFontWin32GDIPlus.h
win32inc := $(patsubst %, ../platforms/win32/src/%, $(win32inc))

######### qt include files 
qtinc  := $(wildcard ../src/engine/include/*.h)
qtfiles =  ../environments/Qt/LICENSE.MPL.txt ../environments/Qt/package/readme.txt

QT_SRC_FILES   = $(shell find $(QTLIB)/src -type f)
QT_SRC_FOLDERS = $(shell find $(QTLIB)/src -type d)
QTKIT_EXCLUDED = $(shell find $(QTLIB)/src -name makeBundle \
				    	-or -name makeTar \
					-or -name *.sh \
					-or -name unix-readme.txt -or -name ignore.txt)

qtKitExcludedFiles := doc package Makefile GuidoQt.dsw GuidoQt.sln GuidoQtFontViewer
qtKitExcludedFiles := $(patsubst %, $(QTLIB)/src/%, $(qtKitExcludedFiles))


help:
	@ echo this is a packaging makefile, available targets are:
	@ echo "  'srcpack': builds an archive with doc, source code and dtds"
	@ echo "  'macos'  : builds an archive with doc, dtds and binary framework"
	@ echo "  'linux'  : builds an archive with doc, dtds, includes, static and dynamic lib"
	@ echo "  'windows': builds an archive with doc, dtds, includes, dll and lib"
	@ echo "  'qt'     : builds an archive with doc, includes, src, samples"
	@ echo "  'docpack': builds the documentation package"
	@ echo "  'doc'    : rebuilds the doxygen documentation"
	@ echo "  'python' : builds the pyguido package"

doc:
	$(MAKE) -C ../doc/doxygen
	$(MAKE) -C ../environments/Qt/doc

docpack:
	$(MAKE) commondoc DEST=.
	cp -r doc $(DOCPACK)
	tar czf $(DOCPACK).tgz $(DOCPACK)
	
srcpack:
	cd .. && git archive -o __tmp__.tgz --prefix=package/$(SRC)/ HEAD && tar xzfP __tmp__.tgz && rm __tmp__.tgz
	cp readme-src_package.txt $(SRC)/readme.txt
	cp ../misc/Acknowledgments.txt $(SRC)
	tar czf $(SRC).tgz $(SRC)

macos:
	[ -d $(MACOSLIB) ] || mkdir $(MACOSLIB)
	$(MAKE) common DEST=$(MACOSLIB)
	rm -rf $(MACOSLIB)/GUIDOEngine.framework
	cp -R $(bin)/GUIDOEngine.framework $(MACOSLIB)
	tar czf $(MACOSLIB).tgz $(MACOSLIB)

linux:
	$(MAKE) version=$(VERSION) -f makedeb

windows:
	[ -d $(WINLIB) ] || mkdir $(WINLIB)
	cp -r doc $(WINLIB)
	$(MAKE) samples DEST=$(WINLIB)
	cp -f $(files) $(WINLIB)
	$(MAKE) incl DEST=$(WINLIB) SYSINC="$(win32inc)"
	cp $(bin)/GUIDOEngine*.dll $(WINLIB)
	cp $(bin)/GUIDOEngine*.lib $(WINLIB)

python:
	tar czf pyGuido.tgz ../lang/python

qt:
	rm -Rf $(QTLIB)
	#### Doc
	$(MAKE) commondoc DEST=$(QTLIB)
	mkdir -p $(QTLIB)/doc/Qt/html 
	cp -Rf ../environments/Qt/doc/html $(QTLIB)/doc/Qt
	cp "../environments/Qt/doc/Guido Qt-Binding manual.pdf" $(QTLIB)/doc/Qt/
	#### Export the sources
	cd ../environments/Qt && git archive -o __tmp__.tgz --prefix=../package/$(QTLIB)/src/ HEAD && tar xzfP __tmp__.tgz && rm __tmp__.tgz
	#### Copy GUIDOEngine includes
	mkdir -p $(QTLIB)/src/engine/include
	cp $(qtinc) $(QTLIB)/src/engine/include
	#### Copy package specific files
	cp -f ../environments/Qt/package/GUIDOEngineLink.pri $(QTLIB)/src
	cp $(qtfiles) $(QTLIB)
	#### Remove inappropriate files from the original SVN-Export
	rm -Rf $(qtKitExcludedFiles)
	rm -f $(QTKIT_EXCLUDED)
	#### Tar
	tar czf $(QTLIB).tgz $(QTLIB)

common:
	$(MAKE) commondoc DEST=$(DEST)
	$(MAKE) samples DEST=$(DEST)
	cp  $(files) $(DEST)

commondoc:
	rm -rf $(DEST)/doc
	mkdir -p $(DEST)/doc
	cp -R ../doc/doxygen/guidolib-$(DOCVERS) $(DEST)/doc || echo "WARNING ! doxygen doc not found"
	cp -R ../doc/html $(DEST)/doc
	rm -rf $(DEST)/doc/html/api
	cp $(DOC) $(DEST)/doc	

samples:
	[ -d $(DEST)/samples ] || mkdir $(DEST)/samples
	cp $(SAMPLES) $(DEST)/samples
	cp readme-samples.txt $(DEST)/samples/readme.txt
	-[ -d $(DEST)/gmn-examples ] && rm -rf $(DEST)/gmn-examples
	cp -R ../gmn-examples $(DEST)/gmn-examples || echo " === WARNING ! gmn-examples NOT exported ==="

incl:
	[ -d $(DEST)/include ] || mkdir $(DEST)/include
	cp $(cominc) $(DEST)/include
	[ -z "$(SYSINC)" ] || cp $(SYSINC) $(DEST)/include
	chmod 644 $(DEST)/include/*.h

