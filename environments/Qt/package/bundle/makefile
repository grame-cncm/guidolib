start:
	@ echo "Target list : "
	@ echo "     - macosx : bundles GuidoEditor & GuidoSceneComposer apps"

include makeBundle

APP_PATH = ../../bin
QT_PLUGIN_PATH = /Developer/Applications/Qt/plugins/
QT_FRAMEWORK_PATH = /Library/Frameworks

striplist = $(shell find $(APP).app -name "*_debug*")

macosx:
	$(MAKE) macosx-app APP=GuidoSceneComposer
	$(MAKE) macosx-app APP=GuidoEditor

macosx-app:
	# Copy the application here
	rm -Rf $(APP).app
	cp -Rf $(APP_PATH)/$(APP).app .

	# Create the bundle's directories
	$(MAKE) mkdirTry DIR=$(APP).app/Contents/Resources
	$(MAKE) mkdirTry DIR=$(APP).app/Contents/Frameworks
	$(MAKE) mkdirTry DIR=$(APP).app/Contents/Plugins/imageformats

	# Copy and link the application to its frameworks
	$(MAKE) installFramework APP=$(APP) FWORK=QtCore      FWORK_PATH=$(QT_FRAMEWORK_PATH)       FWORK_VERSION=4
	$(MAKE) installFramework APP=$(APP) FWORK=QtGui       FWORK_PATH=$(QT_FRAMEWORK_PATH)       FWORK_VERSION=4
	$(MAKE) installFramework APP=$(APP) FWORK=QtXml       FWORK_PATH=$(QT_FRAMEWORK_PATH)       FWORK_VERSION=4
	$(MAKE) installFramework APP=$(APP) FWORK=GUIDOEngine FWORK_PATH=$(HOME)/Library/Frameworks FWORK_VERSION=B

	# Link the frameworks between them
	$(MAKE) linkFramework2Framework APP=$(APP) FWORK_1=QtGui     FWORK_1_VERSION=4   FWORK_2=QtCore       FWORK_2_VERSION=4
	$(MAKE) linkFramework2Framework APP=$(APP) FWORK_1=QtXml     FWORK_1_VERSION=4   FWORK_2=QtCore       FWORK_2_VERSION=4

	# Copy the Qt plugins
	$(MAKE) copyPlugin APP=$(APP) PLUGIN_FOLDER=imageformats PLUGIN_PATH=$(QT_PLUGIN_PATH) PLUGIN=libqgif.dylib
	$(MAKE) copyPlugin APP=$(APP) PLUGIN_FOLDER=imageformats PLUGIN_PATH=$(QT_PLUGIN_PATH) PLUGIN=libqjpeg.dylib
	$(MAKE) copyPlugin APP=$(APP) PLUGIN_FOLDER=imageformats PLUGIN_PATH=$(QT_PLUGIN_PATH) PLUGIN=libqtiff.dylib

	# Link the Qt plugins to the Qt frameworks
	$(MAKE) linkPlugin2Framework APP=$(APP) FWORK=QtCore FWORK_VERSION=4 PLUGIN_FOLDER=imageformats PLUGIN=libqgif.dylib
	$(MAKE) linkPlugin2Framework APP=$(APP) FWORK=QtCore FWORK_VERSION=4 PLUGIN_FOLDER=imageformats PLUGIN=libqjpeg.dylib
	$(MAKE) linkPlugin2Framework APP=$(APP) FWORK=QtCore FWORK_VERSION=4 PLUGIN_FOLDER=imageformats PLUGIN=libqtiff.dylib
	$(MAKE) linkPlugin2Framework APP=$(APP) FWORK=QtGui  FWORK_VERSION=4 PLUGIN_FOLDER=imageformats PLUGIN=libqgif.dylib
	$(MAKE) linkPlugin2Framework APP=$(APP) FWORK=QtGui  FWORK_VERSION=4 PLUGIN_FOLDER=imageformats PLUGIN=libqjpeg.dylib
	$(MAKE) linkPlugin2Framework APP=$(APP) FWORK=QtGui  FWORK_VERSION=4 PLUGIN_FOLDER=imageformats PLUGIN=libqtiff.dylib

	# Other resources
	cp ../../../src/guido2.ttf $(APP).app/Contents/Resources/guido2.ttf
	cp ../../$(APP)/rsc/guido.icns $(APP).app/Contents/Resources/

	$(MAKE) strip
	@ echo "Done." ;

strip:
	rm -rf $(striplist)
