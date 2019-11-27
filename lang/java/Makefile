


SRC        = $(wildcard guidoengine/*.java)
SRCDRAWC   = $(wildcard drawcommand/*.java)
CLASS      = $(patsubst %.java, %.class, $(SRC) )
REMOVELIST = guidoengine/guidoscore.class guidoengine/guidopianoroll.class guidoengine/guidoscoremap.class
CLASSANDROID = $(filter-out $(REMOVELIST),$(CLASS))
CLASSDRAWC = $(patsubst %.java, %.class, $(SRCDRAWC) )
JNI        = $(addprefix jni/, $(subst /,_,$(patsubst guidoengine/%.java, guidoengine_%.h, $(SRC) )))
DOCFOLDER ?= doc

.PHONY : doc

all :
	$(MAKE) jar
	$(MAKE) jar-engine-android
#	$(MAKE) headers

help:
	@echo  This makefile is intended to manage guido java ressources.
	@echo  Available targets are:
	@echo "  class : to compile the library java code"
	@echo "  jar   : to build a jar file"
	@echo "  jar-engine-android : to build guidoengine jar for android (without use of awt class)"
	@echo "  doc   : to build the java documentation"
	@echo "  headers : to make the native headers"
	@echo "  guidoviewer : to compile and execute guido viewer"
	@echo "  factorysample : to compile and execute an example of factory"
	@echo "  tests : to compile and execute a test API"

class : $(CLASS) $(CLASSDRAWC)

jar : guidoengine.jar drawcommand.jar

jar-engine-android : guidoengine-android.jar
	jar cvf guidoengine-android.jar $(patsubst ../%, %, $(CLASSANDROID))

# not use $(CLASS) or $(CLASSDRAWC), it not include inner class declaration
guidoengine.jar : $(CLASS)
	jar cvf guidoengine.jar guidoengine/*.class

guidoengine-android.jar : $(CLASSANDROID)

drawcommand.jar : $(CLASSDRAWC)
	jar cvf drawcommand.jar drawcommand/*.class

tests : tests.class
	$(JAVA) -Djava.library.path=build tests 

guidoviewer : guidoviewer.class
	$(JAVA) -Djava.library.path=build guidoviewer 

guidoviewerSVG : guidoviewerSVG.class
	$(JAVA) -cp "svgSalamander.jar:guidoengine.jar:." -Djava.library.path=build guidoviewerSVG 

guidoviewerSVG.class : guidoviewerSVG.java
	javac -classpath "svgSalamander.jar:guidoengine.jar" guidoviewerSVG.java

factorysample : factorysample.class
	$(JAVA) -Djava.library.path=build factorysample 

clean :
	rm -f $(CLASS) *.class guidoengine/*.class drawcommand/*.class drawcommand.jar guidoengine.jar guidoengine-android.jar

doc: $(DOCFOLDER)
	javadoc -d $(DOCFOLDER) guidoengine/*.java

$(DOCFOLDER):
	mkdir $(DOCFOLDER)


##################################################################
# rule to make JNI headers
headers : $(JNI)

jni/guidoengine_%.h: guidoengine/%.class
	javah -jni -d jni $(patsubst guidoengine/%.class, guidoengine.%, $<)

##################################################################
# rule for java compiling
%.class: %.java
	javac $<


