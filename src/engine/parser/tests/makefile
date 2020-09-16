
CXXFLAGS = -std=c++11 -DTEST -I.. -I../../include -I../../misc -I../../abstract -I../../lib -I../../operations
OBJ =  guidoparse.o guidolex.o
ROOT := ../../../..

gmnfiles := $(shell find $(ROOT)/gmn-examples -name "*.gmn") $(shell find $(ROOT)/regression-tests -name "*.gmn" | grep -v "regression-tests/crash") $(shell find $(ROOT)/open-issues -name "*.gmn")
testout  := $(gmnfiles:%.gmn=%.foo)

all: parsefile # parsebuffer

parser:
	make -C ..
	make

parsefile : $(OBJ) parsefile.o
	$(CXX) $(CXXFLAGS) parsefile.o $(OBJ) -o parsefile

parsebuffer : $(OBJ) parsebuffer.o
	$(CXX) $(CXXFLAGS) parsebuffer.o $(OBJ) -o parsebuffer

GuidoParser.o : ../GuidoParser.cpp
	$(CXX) $(CXXFLAGS) ../GuidoParser.cpp -c 

guidoimpl.o : guidoimpl.cpp
	$(CXX) $(CXXFLAGS) guidoimpl.cpp -c 

guidoparse.o : ../guidoparse.cpp
	$(CXX) $(CXXFLAGS) ../guidoparse.cpp -c 

guidolex.o : ../guidolex.cpp
	$(CXX) $(CXXFLAGS) ../guidolex.cpp -c 


clean:
	rm -f parsefile parsebuffer parse*.o $(OBJ)

tests: $(testout)

%.foo: %.gmn
	./parsefile $<
