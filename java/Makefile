


SRC        = $(wildcard guidoengine/*.java)
SRCDRAWC   = $(wildcard drawcommand/*.java)
CLASS      = $(patsubst %.java, %.class, $(SRC) )
CLASSDRAWC = $(patsubst %.java, %.class, $(SRCDRAWC) )
JNI        = $(addprefix jni/, $(subst /,_,$(patsubst guidoengine/%.java, guidoengine_%.h, $(SRC) )))

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

jar : guidoengine.jar drawcommand.jar

guidoengine.jar : $(CLASS)
	jar cvf guidoengine.jar $(CLASS)

drawcommand.jar : $(CLASSDRAWC)
	jar cvf drawcommand.jar $(CLASSDRAWC)

guidocrash : guidocrash.class
	$(JAVA) guidocrash 

guidoviewer : guidoviewer.class
	$(JAVA) guidoviewer 

factorysample : factorysample.class
	$(JAVA) factorysample 

clean :
	rm -f $(CLASS) *.class guidoengine/*.class

doc:
	[ -d doc ] || mkdir doc
	javadoc -d doc guidoengine/*.java

print-%:
	@echo '$*=$($*)'
	@echo '  origin = $(origin $*)'
	@echo '  flavor = $(flavor $*)'
	@echo '   value = $(value  $*)'

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

