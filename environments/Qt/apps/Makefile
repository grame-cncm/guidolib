

system	:= $(shell uname -s)
ifeq ($(system), MINGW32_NT-6.1)
	LIB = GUIDOEngine.dll
endif


all: $(LIB)
	make -C guido2image/build
	make -C GuidoEditor/build
	make -C GuidoSceneComposer/build

GUIDOEngine.dll: ../../../build/GUIDOEngine.dll
	cp ../../../build/GUIDOEngine.dll .

clean:
	make -C guido2image/build clean
	make -C GuidoEditor/build clean
	make -C GuidoSceneComposer/build clean
