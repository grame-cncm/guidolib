#include <emscripten.h>
#include <bind.h>
#include "GUIDOEngine.h"
#include "guidoengineadapter.h"
#include "GUIDOInternal.h"
#include "GuidoParser.h"

using namespace emscripten;

int main() {
  EM_ASM( allReady() );
}

GuidoLayoutSettings getSettings() {
	GuidoLayoutSettings settings;
	settings.systemsDistance = 1.1;
	settings.systemsDistribution = 4;
	return settings;
}

EMSCRIPTEN_BINDINGS(test) {
	emscripten::value_object<GuidoLayoutSettings>("GuidoLayoutSettings")
        .field("systemsDistance", &GuidoLayoutSettings::systemsDistance)
        .field("systemsDistribution", &GuidoLayoutSettings::systemsDistribution);

    emscripten::function("getSettings", &getSettings);
    emscripten::function("GuidoGetNewDefaultLayoutSettings", &GuidoGetNewDefaultLayoutSettings);
    emscripten::function("GuidoSetNewDefaultLayoutSettings", &GuidoSetNewDefaultLayoutSettings);
}

EMSCRIPTEN_BINDINGS(myAdapter) {
    emscripten::class_<GuidoEngineAdapter>("GuidoEngineAdapter")
            .smart_ptr_constructor("GuidoEngineAdapter",&std::make_shared<GuidoEngineAdapter>)
            //.constructor<>()
            .function("GuidoInit", &GuidoEngineAdapter::GuidoInit)
            .function("GuidoOpenParser", &GuidoEngineAdapter::GuidoOpenParser, allow_raw_pointers())
            .function("CloseParser", &GuidoEngineAdapter::CloseParser, allow_raw_pointers())
            .function("GuidoString2AR", &GuidoEngineAdapter::GuidoString2AR, allow_raw_pointers())
            .function("getSvg", &GuidoEngineAdapter::getSvg, allow_raw_pointers())
            .function("returnVector", &GuidoEngineAdapter::returnVector);

    emscripten::class_<GuidoParser>("GuidoParser");

   // emscripten::value_object<ARHandler>("ARHandler");
    emscripten::class_<NodeAR>("NodeAR");

    emscripten::register_vector<string>("VectorSTRING");
}

