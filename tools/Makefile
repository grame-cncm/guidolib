
INC = -I../src/include -I../src/devices

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -F../build/MacOS/Release/ -framework GUIDOEngine
  INC := $(INC)  -I../macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../linux/src $(shell pkg-config --cflags cairo)
endif
endif

TOOLS = guidogetversion guido2midi guido2svg guidogetpagecount guidogetvoicecount guido2svgmap guidogetpagemap guidogetsystemmap guidogetstaffmap guidogetvoicemap

.PHONY: checkall

all: $(TOOLS)

guidogetversion : guidogetversion.cpp
	g++ guidogetversion.cpp -I../src/include $(lib) -o guidogetversion

guido2midi : guido2midi.cpp
	g++ guido2midi.cpp $(INC) $(lib) -o guido2midi

guido2svg : guido2svg.cpp
	g++ guido2svg.cpp $(INC) $(lib) -o guido2svg

guido2svgmap : guido2svgmap.cpp
	g++ guido2svgmap.cpp $(INC) $(lib) -o guido2svgmap

guidogetpagecount : guidogetpagecount.cpp
	g++ guidogetpagecount.cpp $(INC) $(lib) -o guidogetpagecount

guidogetvoicecount : guidogetvoicecount.cpp
	g++ guidogetvoicecount.cpp $(INC) $(lib) -o guidogetvoicecount

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

