
INC = -I../engine/include -I../engine/devices

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -stdlib=libc++ -mmacosx-version-min=10.7 -F../../build/MacOS/Release/ -framework GUIDOEngine
  debuglib =  -stdlib=libc++ -mmacosx-version-min=10.7 -F../../build/MacOS/Debug/ -framework GUIDOEngine
  INC := $(INC)  -I../../platforms/macosx/src
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../../build/win32/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
  INC := $(INC)  -I../../platforms/linux/src $(shell pkg-config --cflags cairo)
endif
endif

TOOLS = guidogetversion guidogetpagecount guidogetvoicecount guidogettimemap guidogetpagemap guidogetsystemmap guidogetstaffmap guidogetvoicemap guido2svg guido2svgmap guido2abstract guido2proll guido2rproportional midi2proll midi2rproportional guido2midi guidogetmeter guidogetmeters # guido2gmn

.PHONY: checkall

all: $(TOOLS)

guido2gmn : guido2gmn.cpp
	$(CXX) guido2gmn.cpp  $(INC) $(lib) -o guido2gmn

guidogetversion : guidogetversion.cpp
	$(CXX) guidogetversion.cpp -I../engine/include $(lib) -o guidogetversion

midi2proll : midi2proll.cpp
	$(CXX) midi2proll.cpp $(INC) $(lib) -o midi2proll

guido2proll : guido2proll.cpp
	$(CXX) guido2proll.cpp $(INC) $(lib) -o guido2proll

midi2rproportional : midi2rproportional.cpp
	$(CXX) midi2rproportional.cpp $(INC) $(lib) -o midi2rproportional

guido2rproportional : guido2rproportional.cpp
	$(CXX) guido2rproportional.cpp $(INC) $(lib) -o guido2rproportional

guido2midi : guido2midi.cpp
	$(CXX) guido2midi.cpp $(INC) $(lib) -o guido2midi

midi2guido : midi2guido.cpp
	$(CXX) midi2guido.cpp $(INC) $(lib) -o midi2guido

guido2svg : guido2svg.cpp
	$(CXX) guido2svg.cpp $(INC) $(lib) -o guido2svg

guido2abstract : guido2abstract.cpp
	$(CXX) guido2abstract.cpp $(INC) $(lib) -o guido2abstract

guido2svgmap : guido2svgmap.cpp
	$(CXX) guido2svgmap.cpp $(INC) $(lib) -o guido2svgmap

guidogetpagecount : guidogetpagecount.cpp
	$(CXX) guidogetpagecount.cpp $(INC) $(lib) -o guidogetpagecount

guidogetvoicecount : guidogetvoicecount.cpp
	$(CXX) guidogetvoicecount.cpp $(INC) $(lib) -o guidogetvoicecount

guidogetmeter : guidogetmeter.cpp
	$(CXX) guidogetmeter.cpp $(INC) $(lib) -o guidogetmeter

guidogetmeters : guidogetmeters.cpp
	$(CXX) guidogetmeters.cpp $(INC) $(lib) -o guidogetmeters

guidogettimemap : guidogettimemap.cpp
	$(CXX) guidogettimemap.cpp $(INC) $(lib) -o guidogettimemap

guidogetpagemap : guidogetpagemap.cpp
	$(CXX) guidogetpagemap.cpp $(INC) $(lib) -o guidogetpagemap

guidogetsystemmap : guidogetsystemmap.cpp
	$(CXX) guidogetsystemmap.cpp $(INC) $(lib) -o guidogetsystemmap

guidogetstaffmap : guidogetstaffmap.cpp
	$(CXX) guidogetstaffmap.cpp $(INC) $(lib) -o guidogetstaffmap

guidogetvoicemap : guidogetvoicemap.cpp
	$(CXX) guidogetvoicemap.cpp $(INC) $(lib) -o guidogetvoicemap

install:
	cp $(TOOLS) $(HOME)/bin

clean:
	rm -f $(TOOLS)

