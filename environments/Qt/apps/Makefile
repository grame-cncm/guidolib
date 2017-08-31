

system	:= $(shell uname -s)
ifeq ($(system), MINGW32_NT-6.1)
	LIB = GUIDOEngine.dll
endif

BINDEST ?= /usr/bin
RSRC := ../package/debianbase/usr/share

all: $(LIB)
	$(MAKE) -C guido2image/build
	$(MAKE) -C GuidoEditor/build
	$(MAKE) -C GuidoSceneComposer/build

GUIDOEngine.dll: ../../../build/GUIDOEngine.dll
	cp ../../../build/GUIDOEngine.dll .

install:
	[ $(system) = "Linux" ] && $(MAKE) linuxinstall

linuxinstall:
	install -m 755 guido2image/guido2image $(BINDEST)
	install -m 755 GuidoEditor/GuidoEditor $(BINDEST)
	install -m 755 GuidoSceneComposer/GuidoSceneComposer $(BINDEST)
	install -m 755 $(RSRC)/applications/*.desktop /usr/share/applications
	install -d -m 755 $(RSRC)/icons/hicolor/48x48/apps/guidoeditor.png /usr/share/icons/hicolor/48x48/apps/guidoeditor.png
	install -d -m 755 $(RSRC)/icons/hicolor/48x48/apps/guidoscenecomposer.png /usr/share/icons/hicolor/48x48/apps/guidoscenecomposer.png
	install -d -m 755 $(RSRC)/icons/hicolor/128x128/apps/guidoeditor.png /usr/share/icons/hicolor/128x128/apps/guidoeditor.png
	install -d -m 755 $(RSRC)/icons/hicolor/128x128/apps/guidoscenecomposer.png /usr/share/icons/hicolor/128x128/apps/guidoscenecomposer.png

clean:
	$(MAKE) -C guido2image/build clean
	$(MAKE) -C GuidoEditor/build clean
	$(MAKE) -C GuidoSceneComposer/build clean
