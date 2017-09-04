
system	:= $(shell uname -s)

DESTDIR ?= /usr

all:
	$(MAKE) -C GuidoQt/build
	$(MAKE) -C GraphicsSceneMainWindow/build

install:
	[ $(system) = "Linux" ] && $(MAKE) linuxinstall

linuxinstall:
	mkdir -p $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/include/guido
	install -m 755 libGuidoQt.* $(DESTDIR)$(PREFIX)/lib
	install -m 644 GuidoQt/include/*.h $(DESTDIR)$(PREFIX)/include/guido

clean:
	$(MAKE) -C GuidoQt/build clean
	$(MAKE) -C GraphicsSceneMainWindow/build clean
