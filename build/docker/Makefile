#
#
#

VERSION := $(shell cat ../../version.txt)
NAME    := guidolib:$(VERSION)

all:
	@echo building $(NAME) docker image...
	docker build -t $(NAME) .

list:
	docker images

run:
	docker run -t -i $(NAME) bash

guido2svg: GMN  ?= sample.gmn
guido2svg: DIR  := $(shell dirname $(GMN))
guido2svg: FILE := $(shell basename $(GMN))
guido2svg: OUT  := $(shell basename $(FILE) .gmn).svg
guido2svg:
	docker run -t -w /grame/shared -v $(DIR):/grame/shared $(NAME) guido2svg -o $(OUT) $(FILE) 

rmstopped:
	@echo remove all stopped containers
	docker rm $(shell docker ps -a -q)

clean:
	docker image rm -f guidolib:$(VERSION)
