
CXXFLAGS =  -I.. -I../../include -I../../misc -I../../abstract
OBJ =  guido.o guidoparse.o guidolex.o GuidoParser.o

all: parsefile parsebuffer

parsefile : $(OBJ) parsefile.o
	$(CXX) $(CXXFLAGS) parsefile.o $(OBJ) -o parsefile

parsebuffer : $(OBJ) parsebuffer.o
	$(CXX) $(CXXFLAGS) parsebuffer.o $(OBJ) -o parsebuffer

GuidoParser.o : ../GuidoParser.cpp
	$(CXX) $(CXXFLAGS) ../GuidoParser.cpp -c 

guido.o : ../guido.cpp
	$(CXX) $(CXXFLAGS) ../guido.cpp -c 

guidoparse.o : ../guidoparse.cpp
	$(CXX) $(CXXFLAGS) ../guidoparse.cpp -c 

guidolex.o : ../guidolex.cpp
	$(CXX) $(CXXFLAGS) ../guidolex.cpp -c 
	

clean:
	rm -f parsefile parsebuffer parse*.o $(OBJ)
