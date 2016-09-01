
CXXFLAGS =  -I.. -I../../include -I../../misc -I../../abstract
OBJ =  guido.o guidoparse.o guidolex.o GuidoParser.o

all: parsefile parsebuffer

parsefile : $(OBJ) parsefile.o
	g++ $(CXXFLAGS) parsefile.o $(OBJ) -o parsefile

parsebuffer : $(OBJ) parsebuffer.o
	g++ $(CXXFLAGS) parsebuffer.o $(OBJ) -o parsebuffer

GuidoParser.o : ../GuidoParser.cpp
	g++ $(CXXFLAGS) ../GuidoParser.cpp -c 

guido.o : ../guido.cpp
	g++ $(CXXFLAGS) ../guido.cpp -c 

guidoparse.o : ../guidoparse.cpp
	g++ $(CXXFLAGS) ../guidoparse.cpp -c 

guidolex.o : ../guidolex.cpp
	g++ $(CXXFLAGS) ../guidolex.cpp -c 
	

clean:
	rm -f parsefile parsebuffer parse*.o $(OBJ)
