#
# 
#

.PHONY : macos Qt

MACOUT := GUIDOEngine.framework GUIDOEngine.tgz libGUIDOEngine.a

#===============================================================
# building the library on Mac OS X
#===============================================================
macos : $(MACOUT)

GUIDOEngine.framework :  MacOS/Release/GUIDOEngine.framework
	make -C MacOS
	cp -R MacOS/Release/GUIDOEngine.framework .

libGUIDOEngine.a : iOS/Release-iphoneos/libGUIDOEngine.a
	make -C iOS
	cp iOS/Release-iphoneos/libGUIDOEngine.a .

GUIDOEngine.tgz : GUIDOEngine.framework
	tar czf GUIDOEngine.tgz GUIDOEngine.framework

clean :
	make -C MacOS clean
	make -C iOS clean
	rm -rf $(MACOUT)

#===============================================================
# building the Qt resources
#===============================================================

Qt :
	make -C ../environments/Qt
