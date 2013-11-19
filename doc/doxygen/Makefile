
VERSION := 1.53.8
DOC = guidolib-$(VERSION)

doc:
	mv Doxyfile __Doxyfile
	sed -e 's/--version--/$(VERSION)/' __Doxyfile > Doxyfile
	doxygen
	mv __Doxyfile Doxyfile
	cp doxygen.css html
	-make -C latex
	-mv latex/refman.pdf $(DOC).pdf
	-[ -d  $(DOC) ] && rm -rf $(DOC)
	mkdir $(DEST)$(DOC)
	cp -R html $(DEST)$(DOC)
	mv $(DOC).pdf $(DEST)$(DOC)


clean:
	rm -rf html latex $(DOC).pdf $(DOC).tgz $(DOC)
