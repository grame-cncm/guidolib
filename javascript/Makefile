#
# GUIDOEngine Javascript
# This makefile is intended to compile the GUIDO engine as a javascript library
# It is based on emscript that must be available from the command line
# see https://github.com/kripken/emscripten/wiki fro more info
#
# this makefile can run from any location, provide the 'srcdir' variable is correctly set
#

srcdir := ../src/engine
subprojects :=  $(wildcard $(srcdir)/* json binding)
sources = $(wildcard $(srcdir)/*/*.cpp json/*.cpp binding/*.cpp)
objects = $(patsubst binding%, obj/binding%, $(patsubst json%,obj/json%, $(patsubst ../src/engine%,obj%, $(patsubst %.cpp,%.o,$(sources)))))

GCC = emcc
CXXFLAGS += $(addprefix -I, $(subprojects)) -DINDEPENDENTSVG -DJSON_ONLY -Oz
			
target = libGUIDOEngine.js

$(target) : $(objects)
	emcc -Oz --bind --memory-init-file 0 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=0 -s ASM_JS=1 -s TOTAL_MEMORY=83886080 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=1
clean:
	rm -f $(objects) $(target)
	
depend :
	makedepend -fMakefile -Y -- $(CXXFLAGS) -- $(sources)

obj/%.o: $(srcdir)/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/json/%.o: json/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@

obj/binding/%.o: binding/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) --bind -std=c++11 $< -o $@
