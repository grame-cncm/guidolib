#
# This makefile is intended to create the specific documentation for the GUIDO engine javascript library
#

.PHONY: jsbinding.html

target = jsbinding.html
doc := ../../../doc/doxygen/api/

$(target): ../src/binding/main.cpp api
	awk -f binding.awk ../src/binding/main.cpp > $(target)

api:
	cp -R $(doc) api
	
clean :
	rm -rf $(target) api
