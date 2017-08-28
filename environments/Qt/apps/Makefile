

system	:= $(shell uname -s)
ifeq ($(system), MINGW32_NT-6.1)
	LIB = GUIDOEngine.dll
endif


all: $(LIB)
	$(MAKE) -C guido2image/build
	$(MAKE) -C GuidoEditor/build
	$(MAKE) -C GuidoSceneComposer/build

GUIDOEngine.dll: ../../../build/GUIDOEngine.dll
	cp ../../../build/GUIDOEngine.dll .

clean:
	$(MAKE) -C guido2image/build clean
	$(MAKE) -C GuidoEditor/build clean
	$(MAKE) -C GuidoSceneComposer/build clean
