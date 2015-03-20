#
# This makefile is intended to create the specific documentation for the GUIDO engine javascript library
#

.PHONY: jsbinding.html

target = jsbinding.html
doc := ../../../doc/doxygen/html/

$(target): ../src/binding/main.cpp api
	awk -f adapters.awk ../src/binding/main.cpp > $(target)

api:
	cp -R $(doc) api
	
#$(target) : readme.md
#	doxygen

readme.md :
	awk -f binding.awk ../src/binding/main.cpp > readme.md

clean :
	rm -R html
	rm readme.md
