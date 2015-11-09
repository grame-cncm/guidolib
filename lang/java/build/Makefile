


SRC        = $(wildcard ../guidoengine/*.java)
SRCDRAWC   = $(wildcard ../drawcommand/*.java)
CLASS      = $(patsubst %.java, %.class, $(SRC) )
REMOVELIST = ../guidoengine/guidoscore.class ../guidoengine/guidopianoroll.class ../guidoengine/guidoscoremap.class
CLASSANDROID = $(filter-out $(REMOVELIST),$(CLASS))
CLASSDRAWC = $(patsubst %.java, %.class, $(SRCDRAWC) )
JNI        = $(addprefix ../jni/, $(subst /,_,$(patsubst ../guidoengine/%.java, guidoengine_%.h, $(SRC) )))

ifeq ($(ARCH), 64)
	JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
else 
	JAVA  = java
endif


.PHONY : doc macos windows linux

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
	TARGET = macos
else
ifeq ($(system), MINGW32_NT-6.1)
	TARGET = windows
else
ifeq ($(system), MINGW32_NT-6.2)
	TARGET = windows
else
ifeq ($(system), Linux)
	TARGET = linux
endif
endif
endif
endif

# try to detect Visual Studio version used
ifeq ($(TARGET), windows)
 MSVC ?= C:\Program Files (x86)\Microsoft Visual Studio 12.0
 ifneq ($(wildcard "$(MSVC)"), "") 
  WIN32VS ?= "Visual Studio 12 2013"
  WIN64VS ?= "$(WIN32VS) Win64"
  endif
 else
 MSVC := C:\Program Files (x86)\Microsoft Visual Studio 10.0
 ifneq ($(wildcard "$(MSVC)"), "") 
  WIN32VS ?= "Visual Studio 10"
  WIN64VS ?= "$(WIN32VS) Win64"
 endif
endif


all :
	make jar
	make $(TARGET)
	make jar-engine-android
#	make headers

help:
	@echo  This makefile is intended to manage guido java ressources.
	@echo  Available targets are:
	@echo "  class : to compile the library java code"
	@echo "  jar   : to build a jar file"
	@echo "  jar-engine-android : to build guidoengine jar for android (without use of awt class)"
	@echo "  doc   : to build the java documentation"
	@echo "  headers : to make the native headers"
	@echo "  $(TARGET) : to make the native library"
	@echo "  guidoviewer : to compile and execute guido viewer"
	@echo "  factorysample : to compile and execute an example of factory"
	@echo "  tests : to compile and execute a test API"

class : $(CLASS) $(CLASSDRAWC)

jar : guidoengine.jar drawcommand.jar

jar-engine-android : guidoengine-android.jar
	cd .. && jar cvf guidoengine-android.jar $(patsubst ../%, %, $(CLASSANDROID))

# not use $(CLASS) or $(CLASSDRAWC), it not include inner class declaration
guidoengine.jar : $(CLASS)
	cd .. && jar cvf guidoengine.jar guidoengine/*.class

guidoengine-android.jar : $(CLASSANDROID)

drawcommand.jar : $(CLASSDRAWC)
	cd .. && jar cvf drawcommand.jar drawcommand/*.class

tests : ../tests.class
	cd .. && $(JAVA) -Djava.library.path=build tests 

guidoviewer : ../guidoviewer.class
	cd .. && $(JAVA) -Djava.library.path=build guidoviewer 

guidoviewerSVG : ../guidoviewerSVG.class
	cd .. && $(JAVA) -cp "svgSalamander.jar:guidoengine.jar:." -Djava.library.path=build guidoviewerSVG 

../guidoviewerSVG.class : ../guidoviewerSVG.java
	cd .. && javac -classpath "svgSalamander.jar:guidoengine.jar" guidoviewerSVG.java

factorysample : ../factorysample.class
	cd .. && $(JAVA) -Djava.library.path=build factorysample 

clean :
	rm -f $(CLASS) ../*.class ../guidoengine/*.class ../drawcommand/*.class ../drawcommand.jar ../guidoengine.jar ../guidoengine-android.jar
	make $(TARGET)clean

doc:
	[ -d ../doc ] || mkdir ../doc
	javadoc -d ../doc ../guidoengine/*.java

print-%:
	@echo '$*=$($*)'
	@echo '  origin = $(origin $*)'
	@echo '  flavor = $(flavor $*)'
	@echo '   value = $(value  $*)'

##################################################################
# rule to make JNI headers
headers : $(JNI)

../jni/guidoengine_%.h: ../guidoengine/%.class
	cd .. && javah -jni -d jni $(patsubst ../guidoengine/%.class, guidoengine.%, $<)

##################################################################
# rule for java compiling
%.class: %.java
	cd .. && javac $(subst ../,,$<)

#===============================================================
# building the jni library on linux
#===============================================================
.PHONY : linux/libjniGUIDOEngine.so linuxclean

linux : linux/Makefile libjniGUIDOEngine.so

libjniGUIDOEngine.so :  linux/Makefile  linux/libjniGUIDOEngine.so
	rm -rf libjniGUIDOEngine.so
	cp -R linux/libjniGUIDOEngine*.so* .

linux/libjniGUIDOEngine.so : CMakeLists.txt
	make -C linux
	
linux/Makefile : CMakeLists.txt
	[ -d linux ] || mkdir linux
	cd linux && cmake .. -G "Unix Makefiles"

linuxclean:
	rm -rf linux

#===============================================================
# building the tools on windows
#===============================================================
windows :
	make win32
	make win64

win32 : win32/guidojni.sln jniGUIDOEngine32 
win64 : win64/guidojni.sln jniGUIDOEngine64 

jniGUIDOEngine32 : win32/guidojni.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win32/guidojni.sln /maxcpucount:4 /p:Configuration=Release"
	rm -rf jniGUIDOEngine.*
	cp win32/Release/jniGUIDOEngine.dll .
	cp win32/Release/jniGUIDOEngine.lib .
	cp ../../../build/win32/release/GUIDOEngine.dll ../

win32/guidojni.sln : CMakeLists.txt
	[ -d win32 ] || mkdir win32
	cd win32 && cmake .. -G $(WIN32VS)

win64/guidojni.sln : CMakeLists.txt
	[ -d win64 ] || mkdir win64
	cd win64 && cmake .. -G $(WIN64VS)

jniGUIDOEngine64 : win64/guidojni.sln
	cmd /c "\"$(MSVC)\VC\vcvarsall.bat\" && msbuild win64/guidojni.sln /maxcpucount:4 /p:Configuration=Release"
	rm -rf jniGUIDOEngine64.*
	cp win64/Release/jniGUIDOEngine64.dll .
	cp win64/Release/jniGUIDOEngine64.lib .
	cp ../../../build/win64/release/GUIDOEngine64.dll ../

windowsclean :
	rm -rf win32 win64
	
#===============================================================
# building the tools on Mac OS X
#===============================================================
macos : macos/guidojni.xcodeproj libjniGUIDOEngine.dylib

jniGUIDOEngine : macos/guidojni.xcodeproj
	xcodebuild -jobs 8 -project macos/guidojni.xcodeproj -target ALL_BUILD -configuration Release

macos/guidojni.xcodeproj : CMakeLists.txt
	[ -d macos ] || mkdir macos
	cd macos && cmake .. -G Xcode
	
libjniGUIDOEngine.dylib :  jniGUIDOEngine macos/Release/libjniGUIDOEngine.dylib
	cp macos/Release/libjniGUIDOEngine*.dylib .
	cp macos/Release/libjniGUIDOEngine*.dylib ..

macosclean :
	rm -rf macos
