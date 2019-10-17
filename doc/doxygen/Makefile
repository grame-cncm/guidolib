
VERSION := $(shell cat ../../version.txt)
DOC = guidolib-$(VERSION)

all:
	doxygen
	doxygen Doxyall
	

clean:
	rm -rf API internal
