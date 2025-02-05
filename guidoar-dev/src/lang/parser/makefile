
OUT  = glanglex.cpp glangparse.cpp glangparse.hpp
YACC = bison
LEX = flex
PREFIX := glang

code : $(OUT)

glangparse.cpp : glang.y glanglex.cpp glangparse.hpp
	$(YACC) -d -o glangparse.cpp  -p $(PREFIX) glang.y

glangparse.hpp : glang.y 
	$(YACC) -d -o glangparse.cpp  -p $(PREFIX) glang.y

glanglex.cpp : glang.l
	$(LEX)  -oglanglex.cpp -I -P$(PREFIX) glang.l

testGL : glparser.cpp glangparse.cpp glangparse.hpp glanglex.cpp
	gcc glparser.cpp -DMAIN -lstdc++ -o testGL

clean:
	rm -f $(OUT) testGL
