

system	:= $(shell uname -s)
ifeq ($(system), Darwin)
  lib =  -F../cmake/Release/ -framework guidoar
else
ifeq ($(system), MINGW32_NT-6.1)
  lib =  ../cmake/Release/guidoar.lib
else
  lib = -lguidoar
endif
endif


DESC  = $(wildcard desc/*.h)
TOOLS = $(patsubst desc/%.h, bin/%, $(DESC)) guidoduration
SRC   = $(patsubst bin/%, %.cpp, $(TOOLS))
CXXFLAGS = -I../src/interface $(lib)

makebase = sed -e 's/__desc__/$(1)/' template.cxx | sed -e 's/__T__/$(2)/' | sed -e 's/__operator__/$(3)/'
makecpp			= $(call makebase,$(1),$(2),$(3)) | sed -e 's/__strict__/strict_/' 	> $(1).cpp
makerelaxedcpp 	= $(call makebase,$(1),$(2),$(3)) | sed -e 's/__strict__//'  		> $(1).cpp

all: $(TOOLS)

src: $(SRC)	

guidomirror.cpp : 	template.cxx 	desc/guidomirror.h
	$(call makecpp,guidomirror,const char*,guidoGMirror)

guidotranspose.cpp : 	template.cxx 	desc/guidotranspose.h
	$(call makecpp,guidotranspose,const char*,guidoGTranpose)
	
guidotransposei.cpp : 	template.cxx 	desc/guidotransposei.h
	$(call makecpp,guidotransposei,int,guidoVTranpose)
	
guidotop.cpp : 			template.cxx desc/guidotop.h
	$(call makecpp,guidotop,const char*,guidoGTop)
	
guidotopn.cpp : 		template.cxx desc/guidotopn.h
	$(call makecpp,guidotopn,int,guidoVTop)
	
guidobottom.cpp : 		template.cxx desc/guidobottom.h
	$(call makecpp,guidobottom,const char*,guidoGBottom)
	
guidobottomn.cpp : 		template.cxx desc/guidobottomn.h
	$(call makecpp,guidobottomn,int,guidoVBottom)

guidohead.cpp : 		template.cxx desc/guidohead.h
	$(call makecpp,guidohead,const char*,guidoGHead)

guidoheadt.cpp : 		template.cxx desc/guidoheadt.h
	$(call makecpp,guidoheadt,rational,guidoVHead)

guidotail.cpp : 		template.cxx desc/guidotail.h
	$(call makecpp,guidotail,const char*,guidoGTail)

guidotailt.cpp : 		template.cxx desc/guidotailt.h
	$(call makecpp,guidotailt,rational,guidoVTail)

guidoevhead.cpp : 		template.cxx desc/guidoevhead.h
	$(call makecpp,guidoevhead,const char*,guidoGEHead)

guidoevheadn.cpp : 		template.cxx desc/guidoevheadn.h
	$(call makecpp,guidoevheadn,int,guidoVEHead)

guidoevtail.cpp : 		template.cxx desc/guidoevtail.h
	$(call makecpp,guidoevtail,const char*,guidoGETail)

guidoevtailn.cpp : 		template.cxx desc/guidoevtailn.h
	$(call makecpp,guidoevtailn,int,guidoVETail)

guidoseq.cpp : 			template.cxx desc/guidoseq.h
	$(call makerelaxedcpp,guidoseq,const char*,guidoGSeq)

guidopar.cpp : 			template.cxx desc/guidopar.h
	$(call makerelaxedcpp,guidopar,const char*,guidoGPar)

guidoparright.cpp : 	template.cxx desc/guidoparright.h
	$(call makecpp,guidoparright,const char*,guidoGRPar)

guidosetduration.cpp : 	template.cxx desc/guidosetduration.h
	$(call makecpp,guidosetduration,const char*,guidoGSetDuration)

guidosetdurationd.cpp : template.cxx desc/guidosetdurationd.h
	$(call makecpp,guidosetdurationd,rational,guidoVSetDuration)

guidomultduration.cpp : template.cxx desc/guidomultduration.h
	$(call makecpp,guidomultduration,float,guidoVMultDuration)

install:
	cp bin/* $(HOME)/bin

clean:
	rm -f $(SRC) $(TOOLS)

bin/%: %.cpp
	@[ -d bin ] || mkdir bin
	g++ -I../src/interface $(lib) $< -o $@
