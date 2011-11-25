
INC = -I../src/include -I../src/devices

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -F../cmake/Release/ -framework GUIDOEngine
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../cmake/Release/GUIDOEngine.lib
else
  lib = -lGUIDOEngine
endif
endif

TOOLS = guidogetversion guido2midi guido2svg guido2svgmap 

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

install:
	cp $(TOOLS) $(HOME)/bin

clean:
	rm -f $(TOOLS)

