#
# pour recompiler la doc doxygen, ce dossier doit être au même
# niveau que le dossier des sources java (guidoengine)
#

report: Guido-Java-Report.pdf GUIDO-JNI.pdf
	@echo "==> combiner les sorties pdf en un seul document"

Guido-Java-Report.pdf : Guido-Java-Report.bib Guido-Java-Report.bbl

GUIDO-JNI.pdf :
	doxygen
	cp doxygen.sty latex
	-$(MAKE) -C latex
	-mv latex/refman.pdf GUIDO-JNI.pdf

clean:
	rm -rf *.aux *.bbl *.blg *.log *.out html latex *.pdf


%.bbl: %.bib
	pdflatex $(patsubst %.bib, %.tex, $<) 
	bibtex $(patsubst %.bib, %, $<)
	pdflatex $(patsubst %.bib, %.tex, $<) 
	pdflatex $(patsubst %.bib, %.tex, $<) 

%.aux : %.tex
	pdflatex $< 

%.pdf: %.tex
	pdflatex $< 
