
system	:= $(shell uname -s)

PROJECT := GuidoQt
LIBDEST ?= /usr/lib
INCDEST ?= /usr/include/guido

all:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT)

install:
	[ $(system) = "linux" ] && $(MAKE) linuxinstall

linuxinstall:
	cp -d linux/libGuidoQt.so* $(LIBDEST)
	install -m "rw-r--r--" ../include/*.h $(INCDEST)


macos:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) macos

ios:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) ios

clean:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) clean
