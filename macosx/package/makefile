start:
	@ echo "Target list : "
	@ echo "     - macosx : bundles GuidoQuartzViewer app"

include makeBundle

VERSION=1.20

PACKAGE_NAME = GuidoQuartzViewer-$(VERSION)

APP_PATH = ../build/Release
QT_PLUGIN_PATH = /Developer/Applications/Qt/plugins/
QT_FRAMEWORK_PATH = /Library/Frameworks

EXPORT  := svn export

striplist = $(shell find $(APP).app -name "*_debug*")

macosx:
	make common
	make macosx-app APP=GuidoQuartzViewer

macosx-app:
	# Copy the application here
	rm -Rf $(APP).app
	cp -Rf $(APP_PATH)/$(APP).app .

	# Create the bundle's directories
	make mkdirTry DIR=$(APP).app/Contents/Resources
	make mkdirTry DIR=$(APP).app/Contents/Frameworks

	# Copy and link the application to its frameworks
	make installFramework APP=$(APP) FWORK=GUIDOEngine FWORK_PATH=$(HOME)/Library/Frameworks FWORK_VERSION=A

	# Link the frameworks between them
	# ... none ...

	mv -f $(APP).app $(PACKAGE_NAME)

	make strip
	@ echo "Done." ;

common:
	[ -d $(PACKAGE_NAME)/doc ] || mkdir -p $(PACKAGE_NAME)/doc
	cp "../../doc/GUIDO-Music Notation Format.pdf" $(PACKAGE_NAME)/doc
	cp "../../doc/GUIDO-Format_de_Notation_Musicale.pdf" $(PACKAGE_NAME)/doc
	cp "../../doc/GUIDO_Additional_Specifications_1.3.8.pdf" $(PACKAGE_NAME)/doc
	cp -r "../../doc/html" $(PACKAGE_NAME)/doc
	$(EXPORT) ../../gmn-examples $(PACKAGE_NAME)/gmn-examples || echo " === WARNING ! gmn-examples NOT exported ==="
	cp ../../changelog.txt $(PACKAGE_NAME)

strip:
	rm -rf $(striplist)
