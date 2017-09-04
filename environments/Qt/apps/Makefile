

system	:= $(shell uname -s)
ifeq ($(system), MINGW32_NT-6.1)
	LIB = GUIDOEngine.dll
endif

DESTDIR ?= /usr
BINDEST = $(DESTDIR)$(PREFIX)/bin
SHARED  = $(DESTDIR)$(PREFIX)/share
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
	mkdir -p $(SHARED)/applications
	install -m 644 $(RSRC)/applications/*.desktop $(SHARED)/applications
	mkdir -p $(SHARED)/icons/hicolor/48x48/apps
	install -m 644 $(RSRC)/icons/hicolor/48x48/apps/guidoeditor.png $(SHARED)/icons/hicolor/48x48/apps/guidoeditor.png
	install -m 644 $(RSRC)/icons/hicolor/48x48/apps/guidoscenecomposer.png $(SHARED)/icons/hicolor/48x48/apps/guidoscenecomposer.png
	mkdir -p $(SHARED)/icons/hicolor/128x128/apps
	install -m 644 $(RSRC)/icons/hicolor/128x128/apps/guidoeditor.png $(SHARED)/icons/hicolor/128x128/apps/guidoeditor.png
	install -m 644 $(RSRC)/icons/hicolor/128x128/apps/guidoscenecomposer.png $(SHARED)/icons/hicolor/128x128/apps/guidoscenecomposer.png

clean:
	$(MAKE) -C guido2image/build clean
	$(MAKE) -C GuidoEditor/build clean
	$(MAKE) -C GuidoSceneComposer/build clean
