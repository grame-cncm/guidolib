
OUT  = guidolex.cxx guidoparse.cxx guidoparse.hxx
YACC = bison
LEX = flex
PREFIX := guido

code : $(OUT)

guidoparse.cpp : guido.y guido.h 
	$(YACC) -d -o guidoparse.cxx  -p $(PREFIX) guido.y

guidoparse.hxx : guido.y guido.h 
	$(YACC) -d -o guidoparse.cxx  -p $(PREFIX) guido.y

guidolex.cxx : guido.l guido.h guidoparse.hxx
	$(LEX)  -oguidolex.cxx -I -P$(PREFIX) guido.l

clean:
	rm -f $(OUT)
