
system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	OPT="APP=.app"
endif


PROJECT := GuidoSceneComposer

all:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) $(OPT)

mac:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) APP=.app TARGET=mac

macos:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) APP=.app macos

ios:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) APP=.app ios

clean:
	$(MAKE) -f ../../Make.shared PROJECT=$(PROJECT) clean
	