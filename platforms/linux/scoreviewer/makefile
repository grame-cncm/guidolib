
CAIRO = $(wildcard ../src/Cairo*.cpp ../src/Cairo*.h)

#CXXFLAGS 	= $(shell pkg-config --cflags gtk+)
#LIBS 		= $(shell pkg-config --libs gtk+)
GTK = /Library/Frameworks/GTK+.framework
#CXXFLAGS 	= -I../src -I../../src/include  -I../../src/tools -I$(GTK)/Headers/gtk-2.0 -I$(GTK)/Headers/glib-2.0 -I$(GTK)/Versions/2.18.X11/Resources/lib/glib-2.0/include -I$(GTK)/Headers/cairo -I$(GTK)/Headers/pango-1.0 -I$(GTK)/Versions/2.18.X11/Resources/lib/gtk-2.0/include -I$(GTK)/Headers/atk-1.0
#LIBS 		= -L$(GTK)/Libraries -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lglib-2.0  -F../../cmake/Release -framework GUIDOEngine

CXXFLAGS = $(shell pkg-config --cflags gtk+-2.0) -I../src -I ../../src/include
LIBS = $(shell pkg-config --libs gtk+-2.0) -L../../cmake/Release -lGUIDOEngine

scoreviewer : scoreviewer.cpp ../src/CairoDevlib.o
	$(CXX) scoreviewer.cpp $(CXXFLAGS) $(LIBS) ../src/CairoDevlib.o -o scoreviewer

../src/CairoDevlib.o : $(CAIRO)
	$(MAKE) -C ../src

clean :
	rm -f scoreviewer
