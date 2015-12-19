VERSION := 1.24
PACK := GuidoJava-v.$(VERSION)
SAMPLES := ../factorysample.java ../guidoviewer.java
JAR := ../guidoengine.jar

usage:
	@echo This makefile builds the Guido java package.
	@echo It should be run on every supported platform.
	@echo usage:
	@echo "  make macosx   # on Mac OS X"
	@echo "  make win32    # on Windows"

macosx :
	make common
	cp ../libjniGUIDOEngine.dylib $(PACK)
	cd  $(PACK) && ../localize libjniGUIDOEngine.dylib
	rm -rf $(PACK)/GUIDOEngine.framework
	cp -R ../../../build/GUIDOEngine.framework $(PACK)

win32 :
	make common
	cp ../build/jniGUIDOEngine.dll $(PACK)
	cp ../../../build/GUIDOEngine.dll $(PACK)
	cp ../build/jniGUIDOEngine64.dll $(PACK)
	cp ../../../build/GUIDOEngine64.dll $(PACK)
#	cp msvcr80.dll $(PACK)

common: 
	make update
	[ -d $(PACK) ] || mkdir $(PACK)
	cp -Rf ../doc $(PACK)
	[ -d $(PACK)/guidoengine ] || mkdir $(PACK)/guidoengine
	cp -Rf ../guidoengine/*.java $(PACK)/guidoengine
	cp -f $(SAMPLES) $(PACK)
	cp -f $(JAR) $(PACK)
	cp readme.txt $(PACK)
	cp ../changelog.txt $(PACK)
	cp ../../../src/guido2.ttf $(PACK)

clean :
	rm -rf $(PACK)

update :
	make -C ../build doc
	make -C ../build jar


