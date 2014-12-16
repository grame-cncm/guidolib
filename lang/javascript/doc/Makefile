#
# This makefile is intended to create the specific documentation for GUIDO engine javascript library
#

target = html

$(target) : readme.md
	doxygen

readme.md :
	awk -f binding.awk ../src/binding/main.cpp > readme.md

clean :
	rm -R html
	rm readme.md
