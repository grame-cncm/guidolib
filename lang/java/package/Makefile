VERSION := 1.23
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
	cp -R ../../cmake/Release/GUIDOEngine.framework $(PACK)

win32 :
	make common
	cp ../cmake/release/jniGUIDOEngine.dll $(PACK)
	cp ../../cmake/release/GUIDOEngine.dll $(PACK)
	cp ../jniGUIDOEngine64.dll $(PACK)
	cp ../../win64/release/GUIDOEngine64.dll $(PACK)
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
	cp ../../src/guido2.ttf $(PACK)

clean :
	rm -rf $(PACK)

update :
	make -C .. doc
	make -C .. jar


