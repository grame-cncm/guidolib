#
#  make the library for iOS
#

all : guidolib.xcodeproj
	xcodebuild -jobs 8 -project guidolib.xcodeproj -target ALL_BUILD -configuration Debug
	xcodebuild -jobs 8 -project guidolib.xcodeproj -target ALL_BUILD -configuration Release

guidolib.xcodeproj : ../CMakeLists.txt
	cmake .. -G Xcode -DMIDIEXPORT=yes -DIOS=yes

clean : 
	rm -rf guidolib.build Release-* Debug-*