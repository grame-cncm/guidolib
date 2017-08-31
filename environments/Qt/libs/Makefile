
system	:= $(shell uname -s)

LIBDEST ?= /usr/lib
INCDEST ?= /usr/include/guido

all:
	$(MAKE) -C GuidoQt/build
	$(MAKE) -C GraphicsSceneMainWindow/build

install:
	[ $(system) = "Linux" ] && $(MAKE) linuxinstall

linuxinstall:
	cp  libGuidoQt.a $(LIBDEST)
	install -m 644 GuidoQt/include/*.h $(INCDEST)

clean:
	$(MAKE) -C GuidoQt/build clean
	$(MAKE) -C GraphicsSceneMainWindow/build clean
