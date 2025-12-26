
OUT  = guidolex.c++ guidoparse.c++ guidoparse.h++
YACC = bison
LEX = flex
PREFIX := guidoar

code : $(OUT)

guidoparse.c++ : guido.y
	$(YACC) -d -o guidoparse.c++  -p $(PREFIX) guido.y

guidoparse.h++ : guido.y
	$(YACC) -d -o guidoparse.c++  -p $(PREFIX) guido.y

guidolex.c++ : guido.l guidoparse.h++
	$(LEX)  -oguidolex.c++ -I -P$(PREFIX) guido.l

clean:
	rm -f $(OUT)
