

IGNORED := "shareLocation|splitChord|chord|bembel|merge|port|shortFermata|color|colour|symbol|unit"
TAGSDEST := docs/refs/tags
.PHONY: tagslist.txt


####################################################################
help:
	@echo "Guido Music Notation format"
	@echo "This Makefile is intended to generate the documentation"
	@echo "Available targets are:"
	@echo "  install  : install the required components"
	@echo "  build    : build the web site"
	@echo "  server   : launch the mkdoc server"
	@echo "  tagfiles : create the tags documention md files from the src files"
	@echo "  tagsindex: generates the tags index page"

####################################################################
build:
	mkdocs build

pdfbuild:
	export ENABLE_PDF_EXPORT=1 && mkdocs build

serve:
	mkdocs serve

tagfiles:
	rm -rf tmptags
	[ -d tmptags ] || mkdir tmptags
	awk -v OUT=tmptags -f maketag.awk ../../../src/engine/abstract/AR*.h
	mv -f tmptags/*.md $(TAGSDEST)

tagslist: tagslist.txt

tagslist.txt: 
	make -C ../../../build/ tags | egrep -v 'make|grep' | egrep -v $(IGNORED) | sort -u > tagslist.txt
	@echo Remove manually the 's' tag
	
chapters:
	cat tags.txt | cut -d':' -f 2 | sort -u
	
tagschapters:
	cat tags.txt | sed -e 's/\([^:]*\):\([^:]*\)/\2:\1/' | sed 's/ *//g' | sort -u
	
tagsindex:  docs/refs/index.md

docs/refs/index.md: tags.txt
	cat tags.txt | awk -f tagslist.awk  > docs/refs/index.md

install:
	pip install mkdocs
	pip install mkdocs-pdf-export-plugin
	pip install markdown-include
#	git clone https://github.com/tabatkins/railroad-diagrams.git
#	npm i railroad-diagrams

uninstall:
	pip uninstall -y mkdocs-material
	pip uninstall -y pymdown-extensions
	pip uninstall -y markdown-blockdiag
	pip uninstall -y mkdocs-pdf-export-plugin
