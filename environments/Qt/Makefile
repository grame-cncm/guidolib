
.PHONY: apps libs tools

all:
	$(MAKE) libs
	$(MAKE) apps
	$(MAKE) tools
	
help:
	@ echo " 'all'   :  build all the Qt resources (default)"
	@ echo " 'apps'  :  build Qt applications "
	@ echo " 'libs'  :  build Qt libraries "
	@ echo " 'tools' :  build Qt tools"
	@ echo " 'clean' :  clean all the Qt projects"

libs:
	$(MAKE) -C libs

apps:
	$(MAKE) -C apps

tools:
	$(MAKE) -C tools

install:
	$(MAKE) -C libs install
	$(MAKE) -C apps install


clean:
	$(MAKE) -C libs clean
	$(MAKE) -C apps clean
	$(MAKE) -C tools clean
