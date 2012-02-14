MAKE = make
BUILD =
SPEC =
DO_BUILD = " && $(MAKE)"
DO_CLEAN= " && $(MAKE) clean"
XCODE_BUILD= " && xcodebuild"
MSVC2005_BUILD=" && vcbuild"
# QPROJECT_CLEAN = " && ( [ -d tmpSrc ] || mkdir tmpSrc ) && rm -f tmpSrc/*.cpp && rm -f tmpSrc/*.h "
PROJECTS = GraphicsSceneMainWindow GuidoQt GuidoQtFontViewer GuidoEditor GuidoSceneComposer guido2image GuidoServer

nothing:
	@ echo "Clean build of all the apps."
	@ echo " 'unix' :     for unix/gcc "
	@ echo " 'macos' :    for mac/gcc "
	@ echo " 'xcode' :    for mac/gcc & xcode projects"
	@ echo " 'msvc'  :    for win32 msvc (msvc projects only) "
	@ echo " 'projects' : rebuilds all the qmake projects"
	@ echo " 'msvc2005' : for win32 mscvc2005 (uses vcbuild) "

projects:
	$(foreach qt, $(PROJECTS), make qmake PROJECT=$(qt);)

qmake:
	cd $(PROJECT) && qmake

unix:
	make common BUILD=$(DO_BUILD)

unix-clean:
	make clean CLEAN=$(DO_CLEAN)

macos:
	make macos-clean
	make common SPEC="-spec macx-g++" BUILD=$(DO_BUILD)

macos-clean:
	make clean SPEC="-spec macx-g++" CLEAN=$(DO_CLEAN)

xcode:
	make xcode-clean
	make common BUILD=$(XCODE_BUILD)

xcode-clean:
	make clean

msvc:
	make MAKE=make msvc-clean
	make MAKE=make common

msvc-clean:
	make MAKE=make clean

msvc2005:
	make MAKE=make msvc-clean
	make MAKE=make common BUILD=$(MSVC2005_BUILD)

common:
	$(MAKE) makeApp APP_NAME=GuidoQt
	$(MAKE) makeApp APP_NAME=GraphicsSceneMainWindow
	$(MAKE) makeApp APP_NAME=GuidoEditor
	$(MAKE) makeApp APP_NAME=GuidoQtFontViewer
	$(MAKE) makeApp APP_NAME=GuidoSceneComposer
	$(MAKE) makeApp APP_NAME=guido2image

makeApp:
	cd $(APP_NAME) && qmake $(SPEC) $(BUILD)

clean:
	$(MAKE) cleanApp APP_NAME=GuidoQt CLEAN_MOC=1
	$(MAKE) cleanApp APP_NAME=GraphicsSceneMainWindow CLEAN_MOC=1
	$(MAKE) cleanApp APP_NAME=GuidoEditor CLEAN_MOC=1
	$(MAKE) cleanApp APP_NAME=GuidoQtFontViewer CLEAN_MOC=0
	$(MAKE) cleanApp APP_NAME=GuidoSceneComposer CLEAN_MOC=1
	$(MAKE) cleanApp APP_NAME=guido2image CLEAN_MOC=1

cleanApp:
	cd $(APP_NAME) && qmake $(SPEC) $(CLEAN)
	( [ $(CLEAN_MOC) -eq 1 ] 						\
	&& ([ -d $(APP_NAME)/tmpSrc ] || mkdir $(APP_NAME)/tmpSrc) 		\
	&& rm -f $(APP_NAME)/tmpSrc/*.cpp && rm -f $(APP_NAME)/tmpSrc/*.h )	\
	|| [ 0 -eq 0 ]	

