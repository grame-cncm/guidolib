SRC = $(wildcard guidoengine/*.java guidoengine/drawcommand/*.java)
CLASS = $(patsubst %.java, %.class, $(SRC) )
JNI   = $(patsubst guidoengine/%.java, jni/guidoengine_%.h, $(SRC) )

ifeq ($(ARCH), 64)
	JAVA  = "C:\Program Files\Java\jdk1.6.0_20\bin\java"
else 
	JAVA  = java
endif


.PHONY: doc

help:
	@echo  This makefile is intended to manage guido java ressources.
	@echo  Available targets are:
	@echo "  class : to compile the jni java code"
	@echo "  jar   : to build a jni jar file"
	@echo "  doc   : to build the java documentation"
	@echo "  headers : to make the native headers"
	

class : $(CLASS)

jar : guidoengine.jar

guidoengine.jar : $(CLASS)
	jar cvf guidoengine.jar $(CLASS)

guidocrash : guidocrash.class
	$(JAVA) guidocrash 

guidoviewer : guidoviewer.class
	$(JAVA) guidoviewer 

factorysample : factorysample.class
	$(JAVA) factorysample 

clean :
	rm -f $(CLASS) *.class

doc:
	[ -d doc ] || mkdir doc
	javadoc -d doc guidoengine/*.java

##################################################################
# rule to make JNI headers
headers : $(JNI)

jni/guidoengine_%.h: guidoengine/%.class
	javah -jni -d jni $(patsubst guidoengine/%.class, guidoengine.%, $<) 

##################################################################
# rule for java compiling
%.class: %.java
	javac $< 

##################################################################
# platform specific utilities
win32 : GUIDOEngine.dll jniGUIDOEngine.dll
win64 : GUIDOEngine64.dll jniGUIDOEngine64.dll

GUIDOEngine.dll : ../cmake/release/GUIDOEngine.dll
	cp ../cmake/release/GUIDOEngine.dll .

jniGUIDOEngine.dll : cmake/release/jniGUIDOEngine.dll
	cp cmake/release/jniGUIDOEngine.dll .

GUIDOEngine64.dll : ../win64/release/GUIDOEngine64.dll
	cp ../win64/release/GUIDOEngine64.dll .

jniGUIDOEngine64.dll : win64/release/jniGUIDOEngine64.dll
	cp win64/release/jniGUIDOEngine64.dll .

macos: libjniGUIDOEngine.dylib

libjniGUIDOEngine.dylib : cmake/Release/libjniGUIDOEngine.dylib
	cp -f cmake/Release/libjniGUIDOEngine.dylib .

linux: libjniGUIDOEngine.so

libjniGUIDOEngine.so : cmake/libjniGUIDOEngine.so
	cp -f cmake/libjniGUIDOEngine.so .

