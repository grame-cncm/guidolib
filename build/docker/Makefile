#
#
#

VERSION := 1.6.6

all:
	@echo building docker image...
	docker build -t guidolib:$(VERSION) .

list:
	docker images

run:
	docker run -t -i guidolib:$(VERSION) bash

clean:
	docker image rm -f guidolib:$(VERSION)
