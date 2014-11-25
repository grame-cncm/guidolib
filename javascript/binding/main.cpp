#include <emscripten.h>
#include <bind.h>
#include "GUIDOEngine.h"
#include "guidoengineadapter.h"
#include "GUIDOInternal.h"
#include "GuidoParser.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(CStruct) {

    emscripten::value_object<GuidoLayoutSettings>("GuidoLayoutSettings")
        .field("systemsDistance", &GuidoLayoutSettings::systemsDistance)
        .field("systemsDistribution", &GuidoLayoutSettings::systemsDistribution)
        .field("systemsDistribLimit", &GuidoLayoutSettings::systemsDistribLimit)
        .field("force", &GuidoLayoutSettings::force)
        .field("spring", &GuidoLayoutSettings::spring)
        .field("neighborhoodSpacing", &GuidoLayoutSettings::neighborhoodSpacing)
        .field("optimalPageFill", &GuidoLayoutSettings::optimalPageFill)
        .field("resizePage2Music", &GuidoLayoutSettings::resizePage2Music);

    emscripten::value_object<GuidoDate>("GuidoDate")
        .field("num", &GuidoDate::num)
        .field("denom", &GuidoDate::denom);

    emscripten::value_object<GuidoPageFormat>("GuidoPageFormat")
        .field("systemsDistance", &GuidoPageFormat::width)
        .field("systemsDistribution", &GuidoPageFormat::height)
        .field("systemsDistribLimit", &GuidoPageFormat::marginleft)
        .field("force", &GuidoPageFormat::margintop)
        .field("spring", &GuidoPageFormat::marginright)
        .field("neighborhoodSpacing", &GuidoPageFormat::marginbottom);

    /*
    emscripten::class_<GuidoLayoutSettings>("GuidoLayoutSettings")
        .property("systemsDistance", &GuidoLayoutSettings::systemsDistance)
        .property("systemsDistribution", &GuidoLayoutSettings::systemsDistribution)
        .property("systemsDistribLimit", &GuidoLayoutSettings::systemsDistribLimit)
        .property("force", &GuidoLayoutSettings::force)
        .property("spring", &GuidoLayoutSettings::spring)
        .property("neighborhoodSpacing", &GuidoLayoutSettings::neighborhoodSpacing)
        .property("optimalPageFill", &GuidoLayoutSettings::optimalPageFill)
        .property("resizePage2Music", &GuidoLayoutSettings::resizePage2Music);
*/
}

EMSCRIPTEN_BINDINGS(Adapter) {
    emscripten::class_<GuidoEngineAdapter>("GuidoEngineAdapter")
            .smart_ptr_constructor("GuidoEngineAdapter",&std::make_shared<GuidoEngineAdapter>)
            //.constructor<>()
            .function("GuidoInit", &GuidoEngineAdapter::GuidoInit)
            .function("GuidoOpenParser", &GuidoEngineAdapter::GuidoOpenParser, allow_raw_pointers())
            .function("CloseParser", &GuidoEngineAdapter::CloseParser, allow_raw_pointers())
            .function("GuidoString2AR", &GuidoEngineAdapter::GuidoString2AR, allow_raw_pointers())
            .function("GuidoGetAR2GR", select_overload<NodeGR*(NodeAR*)>(&GuidoEngineAdapter::GuidoGetAR2GR), allow_raw_pointers())
            .function("GuidoGetAR2GRSettings", select_overload<NodeGR*(NodeAR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::GuidoGetAR2GR), allow_raw_pointers())
    // Just for testing
            .function("getSvg", &GuidoEngineAdapter::getSvg, allow_raw_pointers())
            .function("returnVector", &GuidoEngineAdapter::returnVector);

    // Black box object, just for passing argument pointer in method
    emscripten::class_<GuidoParser>("GuidoParser");
    emscripten::class_<NodeAR>("NodeAR");
    emscripten::class_<NodeGR>("NodeGR");
}

