
GTK = /Library/Frameworks/GTK+.framework

SRC := Cairo

SRC = $(wildcard Cairo*.cpp)
OBJ := $(patsubst %.cpp, %.o, $(SRC)) 

TARGET = CairoDevlib.o
CXXFLAGS = $(shell pkg-config --cflags cairo) -I ../../src/include
#CXXFLAGS 	= -I$(GTK)/Headers/gtk-2.0 -I$(GTK)/Headers/glib-2.0 -I$(GTK)/Versions/2.18.X11/Resources/lib/glib-2.0/include -I$(GTK)/Headers/cairo -I$(GTK)/Versions/2.18.X11/Resources/lib/gtk-2.0/include  -I ../../src/include

$(TARGET) : $(OBJ)
	ld -r -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ)
