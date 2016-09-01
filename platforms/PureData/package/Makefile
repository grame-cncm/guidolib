VERSION :=1.01
DATE := $(shell date "+%y-%m-%d")

MACTARGET=guido-pd-mac-$(VERSION)
WINTARGET=guido-pd-win-$(VERSION)

menu:
	@ echo this is a packaging makefile for Pure Data external, available targets are:
	@ echo "  'macos'  : package the Max external for Mac OS"
	@ echo "  'windows': package the Max external for Windows"


macos:
	make common DEST=$(MACTARGET)
	cp -Rf libmusicxml2.framework $(MACTARGET)
	cp -Rf ../../cmake/Release/GUIDOEngine.framework $(MACTARGET)
	cp -R ../guido.pd_darwin $(MACTARGET)
	./localise $(MACTARGET)/guido.pd_darwin
	hdiutil create -srcfolder $(MACTARGET) -format UDZO $(MACTARGET).dmg

windows:
	make common DEST=$(WINTARGET)
	cp -R ../guido.dll	$(WINTARGET)
	cp ../../cmake/release/GUIDOEngine.dll $(WINTARGET)
	cp ../../Max/package/libmusicxml2.dll $(WINTARGET)

common:
	[ -d $(DEST) ] || mkdir $(DEST)
	cp readme.txt	$(DEST)
	cp ../help-guido.pd	$(DEST)
	cp ../bach.gmn	$(DEST)
	cp ../../src/guido2.ttf	$(DEST)
	cp ../changelog.txt	$(DEST)

