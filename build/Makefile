#
# 
#

.PHONY : macos ios Qt

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = mac
else
ifeq ($(system), MINGW32_NT-6.1)

else

endif
endif


OUT  := GUIDOEngine.framework GUIDOEngine.tgz libGUIDOEngine.a
MACX := MacOS
IOS  := iOS

all :
	make $(TARGET)
	make Qt

#===============================================================
# building the library on Mac OS X
#===============================================================
mac :
	make macos
	make ios	

macos : $(MACX) GUIDOEngine.framework GUIDOEngine.tgz

$(MACX) :
	mkdir $(MACX)

GUIDOEngine.framework :  $(MACX)/guidolib.xcodeproj  $(MACX)/Release/GUIDOEngine.framework
	rm -rf GUIDOEngine.framework
	cp -R $(MACX)/Release/GUIDOEngine.framework .

$(MACX)/Release/GUIDOEngine.framework : CMakeLists.txt
	xcodebuild -jobs 8 -project $(MACX)/guidolib.xcodeproj -target ALL_BUILD -configuration Release

$(MACX)/guidolib.xcodeproj : CMakeLists.txt
	cd $(MACX) && cmake .. -G Xcode -DMIDIEXPORT=yes

GUIDOEngine.tgz : GUIDOEngine.framework
	tar czf GUIDOEngine.tgz GUIDOEngine.framework

#===============================================================
# building the library for iOS
#===============================================================
ios : $(IOS) libGUIDOEngine.a

$(IOS) :
	mkdir $(IOS)

libGUIDOEngine.a : $(IOS)/guidolib.xcodeproj $(IOS)/Release-iphoneos/libGUIDOEngine.a
	cp $(IOS)/Release-iphoneos/libGUIDOEngine.a .

$(IOS)/Release-iphoneos/libGUIDOEngine.a : CMakeLists.txt
	xcodebuild -jobs 8 -project $(IOS)/guidolib.xcodeproj -target ALL_BUILD -configuration Release
 
$(IOS)/guidolib.xcodeproj : CMakeLists.txt
	cd $(IOS) && cmake .. -G Xcode -DMIDIEXPORT=no -DIOS=yes

clean :
	rm -rf $(IOS) $(MACX) $(OUT)

#===============================================================
# building the Qt resources
#===============================================================

Qt :
	make -C ../environments/Qt
