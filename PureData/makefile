
ifeq ($(MAKECMDGOALS), macos)
PD = $(HOME)/src/Pd-0.42.5-extended/pd/src
CXXFLAGS = -arch i386 -I../macosx/src
endif

ifeq ($(MAKECMDGOALS), linux)
PD=/home/fober/src/Pd-0.41.4-extended/pd/src
CXXFLAGS = -I../linux/src $(shell pkg-config --cflags cairo) -fPIC
endif

CXXFLAGS := $(CXXFLAGS) -DMUSICXML -I../src/include -I$(PD) 

GUIDOMACLIB = -F../cmake/Release -framework GUIDOEngine
GUIDOLINUXLIB = -lGUIDOEngine
LIBS = -lc -lm

info:
	@echo available targets are 'linux' and 'macos'
	@echo make sure the PD variable points to the pd source.

linux: guido.pd_linux

macos: guido.pd_darwin

guido.pd_linux: guido.o musicxml.o
	gcc -shared -o guido.pd_linux guido.o musicxml.o $(LIBS) $(GUIDOLINUXLIB)

guido.pd_darwin: guido.o musicxml.o
	gcc -arch i386 -shared -undefined suppress -flat_namespace -o guido.pd_darwin guido.o musicxml.o -lstdc++ $(LIBS) $(GUIDOMACLIB)

guido: guido.cpp
	gcc $(CFLAGS) $(LINUXCFLAGS) $(LINUXINCLUDE) -o guido.o -c guido.cpp
	ld -export_dynamic  -shared -o guido.pd_linux guido.o -lc -lm 
	strip --strip-unneeded guido.pd_linux
	rm guido.o

install:
	cp help-guido.pd /usr/local/lib/pd/doc/5.reference
	cp guido.pd_linux /usr/local/lib/pd/externs

clean:
	rm -f guido.o musicxml.o guido.pd_darwin guido.pd_linux

