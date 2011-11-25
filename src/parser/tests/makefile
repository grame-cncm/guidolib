
CXXFLAGS =  -I.. -I../../include -I../../misc
OBJ =  guido.o guidoimpl.o

all: parsefile parsebuffer

parsefile : $(OBJ) parsefile.o
	g++ $(CXXFLAGS) parsefile.o $(OBJ) -o parsefile

parsebuffer : $(OBJ) parsebuffer.o
	g++ $(CXXFLAGS) parsebuffer.o $(OBJ) -o parsebuffer

guido.o : ../guido.cpp
	g++ $(CXXFLAGS) ../guido.cpp -c 
	

clean:
	rm -f parsefile parsebuffer parse*.o $(OBJ)
