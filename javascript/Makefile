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

srcdir := ../src/engine
subprojects :=  $(wildcard $(srcdir)/* json binding device)
sources = $(wildcard $(srcdir)/*/*.cpp json/*.cpp binding/*.cpp device/*.cpp)
objects = $(patsubst device%, obj/device%, $(patsubst binding%, obj/binding%, $(patsubst json%,obj/json%, $(patsubst ../src/engine%,obj%, $(patsubst %.cpp,%.o,$(sources))))))

GCC = emcc
CXXFLAGS += $(addprefix -I, $(subprojects)) -DINDEPENDENTSVG -DJSON_ONLY -DCANVASSYSTEM -O3
			
target = libGUIDOEngine.js

$(target) : $(objects)
	emcc -O3 --bind --memory-init-file 0 $(objects) -o $(target) -s ALLOW_MEMORY_GROWTH=0 -s ASM_JS=1 -s TOTAL_MEMORY=100663296 -s DISABLE_EXCEPTION_CATCHING=1 -s ASSERTIONS=0
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
	
obj/device/%.o: device/%.cpp
	@[ -d $(@D) ] || mkdir -p $(@D)
	$(GCC) -c $(CXXFLAGS) $< -o $@	
