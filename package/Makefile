VERSION := 1.54
DOCVERS := 1.52
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

files   := ../changelog.txt license.txt readme.txt ../src/guido2.ttf ../src/guido2.svg
modules := guidolib

######### documentation files 
DOC		:= ../doc/GUIDO-Format_de_Notation_Musicale.pdf \
			"../doc/GUIDO-Music Notation Format.pdf" \
			../doc/GUIDO_Additional_Specifications_1.3.8.pdf \
			../doc/GUIDO_Additional_Specifications_1.5.1.pdf \
			../doc/refcard/GMNRefCard.pdf \
			../doc/refcard/latex/RefCardsParams.pdf \
			../doc/refcard/latex/RefCardsTags.pdf

SAMPLES := $(wildcard ../samples/*.cpp ../validate/guido*.cpp ../src/tools/Null*.h)

######### common include files 
cominc  :=  $(wildcard ../src/include/GUIDO*.h ../src/include/VG*.h ) ../src/include/TRect.h

######### win32 devices include files 
win32inc := GDeviceWin32.h GSystemWin32.h GFontWin32.h  GDeviceWin32GDIPlus.h GSystemWin32GDIPlus.h GFontWin32GDIPlus.h
win32inc := $(patsubst %, ../win32/src/%, $(win32inc))

######### qt include files 
qtinc 	:= 	GUIDOEngine.h GUIDOExport.h MusicalSymbols.h VGColor.h VGFont.h VGSystem.h VGDevice.h
qtinc  := $(patsubst %, ../src/include/%, $(qtinc))

qtfiles =  ../Qt/LICENSE.LGPL ../Qt/package/readme.txt

QT_SRC_FILES   = $(shell find $(QTLIB)/src -type f)
QT_SRC_FOLDERS = $(shell find $(QTLIB)/src -type d)
QTKIT_EXCLUDED = $(shell find $(QTLIB)/src -name makeBundle \
				    	-or -name makeTar \
					-or -name *.sh \
					-or -name unix-readme.txt -or -name ignore.txt)

qtKitExcludedFiles := doc package Makefile GuidoQt.dsw GuidoQt.sln GuidoQtFontViewer
qtKitExcludedFiles := $(patsubst %, $(QTLIB)/src/%, $(qtKitExcludedFiles))


menu:
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
	make -C ../doc/doxygen
	make -C ../Qt/doc

docpack:
	make commondoc DEST=.
	mv doc $(DOCPACK)
	tar czf $(DOCPACK).tgz $(DOCPACK)
	
srcpack:
	cd .. && git archive -o __tmp__.tgz --prefix=package/$(SRC)/ HEAD && tar xzfP __tmp__.tgz && rm __tmp__.tgz
	cp readme-src_package.txt $(SRC)/readme.txt
	tar czf $(SRC).tgz $(SRC)

macos:
	[ -d $(MACOSLIB) ] || mkdir $(MACOSLIB)
	make common DEST=$(MACOSLIB)
	rm -rf $(MACOSLIB)/GUIDOEngine.framework
	cp -R $(bin)/GUIDOEngine.framework $(MACOSLIB)
	tar czf $(MACOSLIB).tgz $(MACOSLIB)

linux:
	make version=$(VERSION) -f makedeb

windows:
	[ -d $(WINLIB) ] || mkdir $(WINLIB)
	make common DEST=$(WINLIB)
	make incl DEST=$(WINLIB) SYSINC="$(win32inc)"
	cp $(bin)/GUIDOEngine*.dll $(WINLIB)
	cp $(bin)/GUIDOEngine*.lib $(WINLIB)

python:
	tar czf pyGuido.tgz ../python

qt:
	rm -Rf $(QTLIB)
	#### Doc
	make commondoc DEST=$(QTLIB)
	mkdir -p $(QTLIB)/doc/Qt/html 
	cp -Rf ../Qt/doc/html $(QTLIB)/doc/Qt
	cp "../Qt/doc/Guido Qt-Binding manual.pdf" $(QTLIB)/doc/Qt/
	#### Export the sources
	cd ../Qt && git archive -o __tmp__.tgz --prefix=../package/$(QTLIB)/src/ HEAD && tar xzfP __tmp__.tgz && rm __tmp__.tgz
	#### Copy GUIDOEngine includes
	mkdir -p $(QTLIB)/src/include
	cp $(qtinc) $(QTLIB)/src/include
	#### Copy package specific files
	cp -f ../Qt/package/GUIDOEngineLink.pri $(QTLIB)/src
	cp $(qtfiles) $(QTLIB)
	#### Remove inappropriate files from the original SVN-Export
	rm -Rf $(qtKitExcludedFiles)
	rm -f $(QTKIT_EXCLUDED)
	#### Tar
	tar czf $(QTLIB).tgz $(QTLIB)

common:
	make commondoc DEST=$(DEST)
	make samples DEST=$(DEST)
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

