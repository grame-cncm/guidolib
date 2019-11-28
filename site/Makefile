
MAKE 	?= make

MKDIR    := mkdocs
DOCDIR   := $(MKDIR)/docs
DOXDIR   := ../doc/doxygen
DOXDEST  := $(DOCDIR)/dox

EDITOR   := https://guidoeditor.grame.fr/
GMNDOC   := https://guidodoc.grame.fr/

.PHONY: tagslist.txt


####################################################################
help:
	@echo "======================================================="
	@echo "            Guido Project"
	@echo "This Makefile is intended to generate the documentation"
	@echo "======================================================="
	@echo "Available targets are:"
	@echo "  install  : install the required components"
	@echo "  build    : build the web site"
	@echo "  serve    : launch the mkdoc server"
	@echo "  all      : generates all the necessary files from the src code or from gmn files"
	@echo "Development specific targets are available:"
	@echo "  doxygen  : build the doxygen doc"
	@echo "Making the current version publicly available:"
	@echo "  publish  : make all + build, switch to gh-pages and copy to root"
	@echo "             commit and push are still manual operations"

test: 
	@echo EXAMPLESMENU: $(EXAMPLESMENU)

####################################################################
build:
	cd $(MKDIR) && mkdocs build

serve:
	cd $(MKDIR) && mkdocs serve

all:
	$(MAKE) doxygen
	$(MAKE) overview
	$(MAKE) javadoc
	$(MAKE) jsdoc
	$(MAKE) webdoc

publish:
	$(MAKE) build
	git checkout gh-pages
	cp -Rf $(MKDIR)/site/* ..
	@echo "Review the changes, add new files, commit and push manually"
	
####################################################################
overview:
	-[ -d $(DOCDIR)/overview ] && rm -rf $(DOCDIR)/overview
	cp -R Guido-project $(DOCDIR)/overview

####################################################################
javadoc: PWD=$(shell pwd)
javadoc:
	-[ -d $(DOCDIR)/javadoc ] && rm -rf $(DOCDIR)/javadoc
	make -C ../lang/java doc DOCFOLDER=$(PWD)/$(DOCDIR)/javadoc

####################################################################
jsdoc: JSDOC=../lang/javascript/doc
jsdoc: $(DOCDIR)/jsdoc
	make -C $(JSDOC) 
	cp $(JSDOC)/*.png $(DOCDIR)/jsdoc
	cp $(JSDOC)/jsbinding.html $(DOCDIR)/jsdoc/index.html
	cp $(JSDOC)/jsdoc.css $(DOCDIR)/jsdoc/

####################################################################
webdoc: WDOC=../doc/server
webdoc: $(DOCDIR)/webdoc
	make -C $(WDOC) html
	-[ -d $(DOCDIR)/webdoc ] && rm -rf $(DOCDIR)/webdoc
	cp -r $(WDOC)/build/html $(DOCDIR)/webdoc

$(DOCDIR)/webdoc:
	mkdir $(DOCDIR)/webdoc

####################################################################
doxygen:
	make -C $(DOXDIR)
	rm -rf $(DOXDEST)
	mkdir $(DOXDEST)
	mv -f $(DOXDIR)/api $(DOXDEST)
	mv -f $(DOXDIR)/internal $(DOXDEST)
 	

####################################################################
install:
	pip install mkdocs
	pip install mkdocs-pdf-export-plugin
	pip install markdown-include
	pip install mkdocs-bootswatch

uninstall:
	pip uninstall -y markdown-include
	pip uninstall -y mkdocs-pdf-export-plugin
