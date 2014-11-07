#
# GUIDOEngine Javascript
# This makefile is intended to compile the GUIDO engine as a javascript library
# It is based on emscript that must be available from the command line
# see https://github.com/kripken/emscripten/wiki fro more info
#
# this makefile can run from any location, provide the 'srcdir' variable is correctly set
#

srcdir := ../src/engine
subprojects :=  $(wildcard $(srcdir)/* json)
sources = $(wildcard $(srcdir)/*/*.cpp json/*.cpp)
objects = $(patsubst json%,obj/json%, $(patsubst ../src/engine%,obj%, $(patsubst %.cpp,%.o,$(sources))))

GCC = emcc
CXXFLAGS += $(addprefix -I, $(subprojects)) -Ijson -DINDEPENDENTSVG -DJSON_ONLY -O2

EXPORT := "['_GuidoSVGExportWithFontSpec_retCString',\
			'_GuidoAbstractExport_retCString',\
			'_GuidoBinaryExport_retSize',\
			'_GuidoInitWithIndependentSVG',\
			'_GuidoOpenParser',\
			'_GuidoString2AR',\
			'_GuidoCloseParser',\
			'_GuidoReleaseCString',\
			'_GuidoCountVoices',\
			'_GuidoGetPageCount',\
			'_GuidoGetSystemCount',\
			'_GuidoMakeDate',\
			'_GuidoGetDateNum',\
			'_GuidoGetDateDenom',\
			'_GuidoFreeDate',\
			'_GuidoFreeAR',\
			'_GuidoFreeGR',\
			'_GuidoDuration_retDate',\
			'_GuidoFindEventPage_p',\
			'_GuidoFindPageAt_p',\
			'_GuidoGetPageDate_retDate',\
			'_GuidoGetPageMap_JSON',\
			'_GuidoGetStaffMap_JSON',\
			'_GuidoGetVoiceMap_JSON',\
			'_GuidoGetSystemMap_JSON',\
			'_GuidoARretGR']"

target = libGUIDOEngine.js

# emcc -O0 -g4 -s ASSERTIONS=2 -s SAFE_HEAP=1 -s ALIASING_FUNCTION_POINTERS=0 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=$(EXPORT)
$(target) : $(objects)
	emcc -O2 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=1 -s EXPORTED_FUNCTIONS=$(EXPORT)

clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/%.o: json/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@
