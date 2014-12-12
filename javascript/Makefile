#
# GUIDOEngine Javascript
# This makefile is intended to compile the GUIDO engine as a javascript library
# It is based on emscript that must be available from the command line
# see http://kripken.github.io/emscripten-site/ for more info
#
# this makefile can run from any location, provide the 'srcdir' variable is correctly set
#
# The TOTAL_MEMORY is the maximum memory allocated by emscripten. This value is specific (100663296) to be compliant with the asm.js and prevent warning at execution.
# It can be increased if error occured.
# 
# Compilation option -Oz is discouraged, it provoke undefined results and assertion errors at runtime.
#

version := 1.00

srcdir := ../src/engine
subprojects :=  $(wildcard $(srcdir)/* src/*)
sources = $(wildcard $(srcdir)/*/*.cpp src/*/*.cpp)
objects = $(patsubst src%, obj/src%, $(patsubst ../src/engine%,obj%, $(patsubst %.cpp,%.o,$(sources))))

GCC = emcc
CXXFLAGS += $(addprefix -I, $(subprojects)) -DINDEPENDENTSVG -DJSON_ONLY -DCANVASSYSTEM -O3

library = libGUIDOEngine
target = $(library).js

##############################################################
# building the library
##############################################################
$(target) : $(objects)
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=0 -s ASM_JS=1 -s TOTAL_MEMORY=100663296 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0
clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

##############################################################
# packaging
##############################################################
examples = $(patsubst %.html,%, $(wildcard examples/*.html))

package: structure $(examples)
	cp libGUIDOEngine.js package/$(library)$(version)/
	cp readme.txt package/$(library)$(version)/
	cp changelog.txt package/$(library)$(version)/
	cp -R doc/ package/$(library)$(version)
	cp examples/readme.txt package/$(library)$(version)/examples/readme.txt
	cp js/jsBinaryDeviceParser.js package/$(library)$(version)/examples/binaryExport/js/
	cp js/jsCanvasDeviceFromBinary.js package/$(library)$(version)/examples/binaryExport/js/
	cp js/jsCanvasDevice.js package/$(library)$(version)/examples/canvasDevice/js/
	
structure:
	mkdir package
	mkdir package/$(library)$(version)
	cd package/$(library)$(version)
	mkdir package/$(library)$(version)/examples

examples/% :
	mkdir package/$(library)$(version)/$@
	mkdir package/$(library)$(version)/$@/html
	mkdir package/$(library)$(version)/$@/js/
	cp -R fonts/ package/$(library)$(version)/$@/
	cp -R header/ package/$(library)$(version)/$@/
	cp -R stylesheets/ package/$(library)$(version)/$@/
	cp $@.html package/$(library)$(version)/$@/html/
	cp js/jquery-2.1.1.min.js package/$(library)$(version)/$@/js/

##############################################################
# rules for building the library
##############################################################
obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/src/json/%.o: src/json/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/src/binding/%.o: src/binding/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) --bind -std=c++11 $< -o $@
	
obj/src/device/%.o: src/device/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@	

