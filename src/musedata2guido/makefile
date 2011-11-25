TARGET=libmusedata2guido.a
APP = musedata2guido

SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

all : $(APP)

$(APP) : main.cpp $(OBJ)
	g++ $(CXXFLAGS) $(OBJ) -o $(APP)

dep:
	makedepend -- $(CXXFLAGS) -- $(SRC)

clean:
	rm -f $(TARGET) $(APP) $(OBJ)
