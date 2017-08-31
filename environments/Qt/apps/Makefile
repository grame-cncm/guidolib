

system	:= $(shell uname -s)
ifeq ($(system), MINGW32_NT-6.1)
	LIB = GUIDOEngine.dll
endif

BINDEST ?= /usr/bin

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

clean:
	$(MAKE) -C guido2image/build clean
	$(MAKE) -C GuidoEditor/build clean
	$(MAKE) -C GuidoSceneComposer/build clean
