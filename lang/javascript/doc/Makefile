#
# This makefile is intended to create the specific documentation for the GUIDO engine javascript library
#

.PHONY: jsbinding.html

target = jsbinding.html

$(target): ../src/binding/main.cpp 
	awk -f binding.awk ../src/binding/main.cpp > $(target)

	
clean :
	rm -rf $(target)
