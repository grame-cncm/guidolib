
.PHONY: apps libs tools

all:
	make libs
	make apps
	make tools
	
help:
	@ echo " 'all'   :  build all the Qt resources (default)"
	@ echo " 'apps'  :  build Qt applications "
	@ echo " 'libs'  :  build Qt libraries "
	@ echo " 'tools' :  build Qt tools"
	@ echo " 'clean' :  clean all the Qt projects"

libs:
	make -C libs

apps:
	make -C apps

tools:
	make -C tools

clean:
	make -C libs clean
	make -C apps clean
	make -C tools clean
