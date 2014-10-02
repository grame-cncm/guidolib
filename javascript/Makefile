#
# GUIDOEngine Javascript
# This makefile is intended to compile the GUIDO engine as a javascript library
# It is based on emscript that must be available from the command line
# see https://github.com/kripken/emscripten/wiki fro more info
#
# this makefile can run from any location, provide the 'srcdir' variable is correctly set
#

srcdir := ../src/engine
subprojects :=  $(wildcard $(srcdir)/*)
sources = $(wildcard $(srcdir)/*/*.cpp)
objects = $(patsubst ../src/engine%,obj%, $(patsubst %.cpp,%.o,$(sources)))

GCC = emcc
CXXFLAGS += $(addprefix -I, $(subprojects)) -DINDEPENDENTSVG

EXPORT := "['_GuidoSVGExportWithFontSpec_retCString',\
			'_GuidoInitWithIndependentSVG',\
			'_GuidoOpenParser',\
			'_GuidoString2AR',\
			'_GuidoCloseParser',\
			'_GuidoReleaseCString',\
			'_GuidoARretGR']"

target = libGUIDOEngine.js

$(target) : $(objects)
	emcc -O2 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=$(EXPORT)

clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@
