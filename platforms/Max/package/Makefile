VERSION :=1.01
DATE := $(shell date "+%y-%m-%d")

MACTARGET=guido-max-mac-$(VERSION)
WINTARGET=guido-max-win-$(VERSION)

menu:
	@ echo this is a packaging makefile for Max external, available targets are:
	@ echo "  'macos'  : package the Max external for Mac OS"
	@ echo "  'windows': package the Max external for Windows"


macos:
	make common DEST=$(MACTARGET)
	cp -R ../guido.mxo	$(MACTARGET)
	cp -R ../../cmake/Release/GUIDOEngine.framework $(MACTARGET)
	cp -R libmusicxml2.framework $(MACTARGET)
	./Localise $(MACTARGET)/guido.mxo/Contents/MacOS/guido
	hdiutil create -srcfolder $(MACTARGET) -format UDZO $(MACTARGET).dmg

windows:
	make common DEST=$(WINTARGET)
	cp -R ../guido.mxe	$(WINTARGET)
	cp ../../cmake/release/GUIDOEngine.dll $(WINTARGET)
	cp libmusicxml2.dll $(WINTARGET)

common:
	[ -d $(DEST) ] || mkdir $(DEST)
	cp readme.txt	$(DEST)
	cp ../guido.maxhelp	$(DEST)
	cp ../bach.gmn	$(DEST)
	cp ../../src/guido2.ttf	$(DEST)
	cp ../changelog.txt	$(DEST)

