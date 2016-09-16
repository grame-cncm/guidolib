src=../../../src

ALL =  elements.pdf guidoelt.pdf 4voices.pdf voices.pdf

%.pdf: %.dot
	dot -Tpdf -o $@ $< 

%.pdf: %.gmn
	guido2Image -f PDF $< 

all: $(ALL)

code: 
	g++ code.cpp -Wall -framework libguidoar -F$(HOME)/Library/Frameworks -I$(src)/guido -I$(src)/guido/abstract  -I$(src)/lib  -I$(src)/visitors  -I$(src)/parser -o countnotes

clean:
	rm -f $(ALL)