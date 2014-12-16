
GTK = /Library/Frameworks/GTK+.framework
CXXFLAGS 	= -I../src -I../../src/include -I$(GTK)/Headers/gtk-2.0 -I$(GTK)/Headers/glib-2.0 -I$(GTK)/Versions/2.18.X11/Resources/lib/glib-2.0/include -I$(GTK)/Headers/cairo -I$(GTK)/Headers/pango-1.0 -I$(GTK)/Versions/2.18.X11/Resources/lib/gtk-2.0/include -I$(GTK)/Headers/atk-1.0
LIBS 		= -L$(GTK)/Libraries -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lgdk_pixbuf-2.0 -lpangocairo-1.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lglib-2.0

CXXFLAGS = $(shell pkg-config --cflags gtk+-2.0) -I../src -I ../../src/include
LIBS = $(shell pkg-config --libs gtk+-2.0)

fontviewer : fontviewer.cpp ../src/CairoDevlib.o
	g++ fontviewer.cpp $(CXXFLAGS) $(LIBS) ../src/CairoDevlib.o -o fontviewer
	
