
INC = -I../src/include -I../src/devices

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -mmacosx-version-min=10.6 -F../build/MacOS/Release/ -framework GUIDOEngine
  debuglib =  -mmacosx-version-min=10.6 -F../build/MacOS/Debug/ -framework GUIDOEngine
  INC := $(INC)  -I../macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../linux/src $(shell pkg-config --cflags cairo)
endif
endif

TOOLS = guidogetversion guidogetpagecount guidogetvoicecount guidogettimemap guidogetpagemap guidogetsystemmap guidogetstaffmap guidogetvoicemap guido2svg guido2svgmap guido2abstract guido2proll midi2proll guido2rproportional midi2rproportional

.PHONY: checkall

all: $(TOOLS) guido2midi

tools: $(TOOLS)

guidogetversion : guidogetversion.cpp
	g++ guidogetversion.cpp -I../src/include $(lib) -o guidogetversion

midi2rproportional : midi2rproportional.cpp
	g++ midi2rproportional.cpp $(INC) $(debuglib) -o midi2rproportional

midi2proll : midi2proll.cpp
	g++ midi2proll.cpp $(INC) $(lib) -o midi2proll

guido2rproportional : guido2rproportional.cpp
	g++ guido2rproportional.cpp $(INC) $(debuglib) -o guido2rproportional

guido2proll : guido2proll.cpp
	g++ guido2proll.cpp $(INC) $(lib) -o guido2proll

guido2midi : guido2midi.cpp
	g++ guido2midi.cpp $(INC) $(lib) -o guido2midi

midi2guido : midi2guido.cpp
	g++ midi2guido.cpp $(INC) $(lib) -o midi2guido

guido2svg : guido2svg.cpp
	g++ guido2svg.cpp $(INC) $(lib) -o guido2svg

guido2abstract : guido2abstract.cpp
	g++ guido2abstract.cpp $(INC) $(lib) -o guido2abstract

guido2svgmap : guido2svgmap.cpp
	g++ guido2svgmap.cpp $(INC) $(lib) -o guido2svgmap

guidogetpagecount : guidogetpagecount.cpp
	g++ guidogetpagecount.cpp $(INC) $(lib) -o guidogetpagecount

guidogetvoicecount : guidogetvoicecount.cpp
	g++ guidogetvoicecount.cpp $(INC) $(lib) -o guidogetvoicecount

guidogettimemap : guidogettimemap.cpp
	g++ guidogettimemap.cpp $(INC) $(lib) -o guidogettimemap

guidogetpagemap : guidogetpagemap.cpp
	g++ guidogetpagemap.cpp $(INC) $(lib) -o guidogetpagemap

guidogetsystemmap : guidogetsystemmap.cpp
	g++ guidogetsystemmap.cpp $(INC) $(lib) -o guidogetsystemmap

guidogetstaffmap : guidogetstaffmap.cpp
	g++ guidogetstaffmap.cpp $(INC) $(lib) -o guidogetstaffmap

guidogetvoicemap : guidogetvoicemap.cpp
	g++ guidogetvoicemap.cpp $(INC) $(lib) -o guidogetvoicemap

install:
	cp $(TOOLS) $(HOME)/bin

clean:
	rm -f $(TOOLS)

