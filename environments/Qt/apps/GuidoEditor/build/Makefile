
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	OPT="APP=.app"
endif


PROJECT := GuidoEditor

all:
	make -f ../../Make.shared PROJECT=$(PROJECT) $(OPT)

mac:
	make -f ../../Make.shared PROJECT=$(PROJECT) APP=.app TARGET=mac

macos:
	make -f ../../Make.shared PROJECT=$(PROJECT) APP=.app macos

ios:
	make -f ../../Make.shared PROJECT=$(PROJECT) APP=.app ios

clean:
	make -f ../../Make.shared PROJECT=$(PROJECT) clean
	