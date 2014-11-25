#include <emscripten.h>
#include <bind.h>
#include "GUIDOEngine.h"
#include "guidoengineadapter.h"
#include "GUIDOInternal.h"
#include "GuidoParser.h"

using namespace emscripten;

/* Structure and enum binding.
 * This structures can be created and manipulated in javascript side like json object and passed to C++ method.
 */
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
			.field("width", &GuidoPageFormat::width)
			.field("height", &GuidoPageFormat::height)
			.field("marginleft", &GuidoPageFormat::marginleft)
			.field("margintop", &GuidoPageFormat::margintop)
			.field("marginright", &GuidoPageFormat::marginright)
			.field("marginbottom", &GuidoPageFormat::marginbottom);

	emscripten::value_object<ParserError>("ParserError")
			.field("line", &ParserError::line)
			.field("col", &ParserError::col)
			.field("msg", &ParserError::msg);

	emscripten::value_object<GuidoVersion>("GuidoVersion")
			.field("major", &GuidoVersion::major)
			.field("minor", &GuidoVersion::minor)
			.field("sub", &GuidoVersion::sub)
			.field("str", &GuidoVersion::str);

	emscripten::enum_<GuidoErrCode>("GuidoErrCode")
			.value("guidoNoErr", GuidoErrCode::guidoNoErr)
			.value("guidoErrParse", guidoErrParse)
			.value("guidoErrMemory", GuidoErrCode::guidoErrMemory)
			.value("guidoErrFileAccess", GuidoErrCode::guidoErrFileAccess)
			.value("guidoErrUserCancel", GuidoErrCode::guidoErrUserCancel)
			.value("guidoErrNoMusicFont", GuidoErrCode::guidoErrNoMusicFont)
			.value("guidoErrNoTextFont", GuidoErrCode::guidoErrNoTextFont)
			.value("guidoErrBadParameter", GuidoErrCode::guidoErrBadParameter)
			.value("guidoErrInvalidHandle", GuidoErrCode::guidoErrInvalidHandle)
			.value("guidoErrNotInitialized", GuidoErrCode::guidoErrNotInitialized)
			.value("guidoErrActionFailed", GuidoErrCode::guidoErrActionFailed);
}

EMSCRIPTEN_BINDINGS(Adapter) {
	// Binding C++ class adapter for guidoEngine
	emscripten::class_<GuidoEngineAdapter>("GuidoEngineAdapter")
			//.smart_ptr_constructor("GuidoEngineAdapter",&std::make_shared<GuidoEngineAdapter>)
			.constructor<>()
			.function("guidoInitWithIndependentSVG", &GuidoEngineAdapter::guidoInitWithIndependentSVG)
			.function("guidoShutdown", &GuidoEngineAdapter::guidoShutdown)
			.function("guidoAR2GR", select_overload<NodeGR*(NodeAR*)>(&GuidoEngineAdapter::guidoAR2GR), allow_raw_pointers())
			.function("guidoAR2GRSettings", select_overload<NodeGR*(NodeAR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::guidoAR2GR), allow_raw_pointers())
			.function("guidoUpdateGR", select_overload<GuidoErrCode(NodeGR*)>(&GuidoEngineAdapter::guidoUpdateGR), allow_raw_pointers())
			.function("guidoUpdateGRSettings", select_overload<GuidoErrCode(NodeGR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::guidoUpdateGR), allow_raw_pointers())
			.function("guidoFreeAR", &GuidoEngineAdapter::guidoFreeAR, allow_raw_pointers())
			.function("guidoFreeGR", &GuidoEngineAdapter::guidoFreeGR, allow_raw_pointers())
			.function("guidoGetErrorString", &GuidoEngineAdapter::guidoGetErrorString)
			.function("guidoGetDefaultLayoutSettings", &GuidoEngineAdapter::guidoGetDefaultLayoutSettings)
			.function("guidoCountVoices", &GuidoEngineAdapter::guidoCountVoices, allow_raw_pointers())
			.function("guidoGetPageCount", &GuidoEngineAdapter::guidoGetPageCount, allow_raw_pointers())
			.function("guidoGetSystemCount", &GuidoEngineAdapter::guidoGetSystemCount, allow_raw_pointers())
			.function("guidoDuration", &GuidoEngineAdapter::guidoDuration, allow_raw_pointers())
			.function("guidoFindEventPage", &GuidoEngineAdapter::guidoFindEventPage, allow_raw_pointers())
			.function("guidoFindPageAt", &GuidoEngineAdapter::guidoFindPageAt, allow_raw_pointers())
			.function("guidoGetPageDate", &GuidoEngineAdapter::guidoGetPageDate, allow_raw_pointers())
			.function("guidoSVGExport", select_overload<string (NodeGR*, int)>(&GuidoEngineAdapter::guidoSVGExport), allow_raw_pointers())
			.function("guidoAbstractExport", select_overload<string (NodeGR*, int)>(&GuidoEngineAdapter::guidoAbstractExport), allow_raw_pointers())
			.function("guidoBinaryExport", select_overload<string (NodeGR*, int)>(&GuidoEngineAdapter::guidoBinaryExport), allow_raw_pointers())
			.function("guidoSetDrawBoundingBoxes", &GuidoEngineAdapter::guidoSetDrawBoundingBoxes)
			.function("guidoGetDrawBoundingBoxes", &GuidoEngineAdapter::guidoGetDrawBoundingBoxes)
			.function("guidoGetPageFormat", &GuidoEngineAdapter::guidoGetPageFormat, allow_raw_pointers())
			.function("guidoSetDefaultPageFormat", &GuidoEngineAdapter::guidoSetDefaultPageFormat)
			.function("guidoGetDefaultPageFormat", &GuidoEngineAdapter::guidoGetDefaultPageFormat)
			.function("guidoUnit2CM", &GuidoEngineAdapter::guidoUnit2CM)
			.function("guidoCM2Unit", &GuidoEngineAdapter::guidoCM2Unit)
			.function("guidoUnit2Inches", &GuidoEngineAdapter::guidoUnit2Inches)
			.function("guidoInches2Unit", &GuidoEngineAdapter::guidoInches2Unit)
			.function("guidoResizePageToMusic", &GuidoEngineAdapter::guidoResizePageToMusic, allow_raw_pointers())
			.function("guidoGetVersion", &GuidoEngineAdapter::guidoGetVersion)
			.function("guidoCheckVersionNums", &GuidoEngineAdapter::guidoCheckVersionNums)
			.function("guidoGetLineSpace", &GuidoEngineAdapter::guidoGetLineSpace)
			.function("guidoMarkVoice", &GuidoEngineAdapter::guidoMarkVoice, allow_raw_pointers())
			.function("guidoOpenParser", &GuidoEngineAdapter::guidoOpenParser, allow_raw_pointers())
			.function("guidoCloseParser", &GuidoEngineAdapter::guidoCloseParser, allow_raw_pointers())
			.function("guidoFile2AR", &GuidoEngineAdapter::guidoFile2AR, allow_raw_pointers())
			.function("guidoString2AR", &GuidoEngineAdapter::guidoString2AR, allow_raw_pointers())
			.function("guidoGetStream", &GuidoEngineAdapter::guidoGetStream, allow_raw_pointers())
			.function("guidoStream2AR", &GuidoEngineAdapter::guidoStream2AR, allow_raw_pointers())
			.function("guidoParserGetErrorCode", &GuidoEngineAdapter::guidoParserGetErrorCode, allow_raw_pointers())
			.function("guidoOpenStream", &GuidoEngineAdapter::guidoOpenStream, allow_raw_pointers())
			.function("guidoCloseStream", &GuidoEngineAdapter::guidoCloseStream, allow_raw_pointers())
			.function("guidoWriteStream", &GuidoEngineAdapter::guidoWriteStream, allow_raw_pointers())
			.function("guidoResetStream", &GuidoEngineAdapter::guidoResetStream, allow_raw_pointers());

	// Black box object, just for passing argument pointer in method
	emscripten::class_<GuidoParser>("GuidoParser");
	emscripten::class_<NodeAR>("NodeAR");
	emscripten::class_<NodeGR>("NodeGR");
	emscripten::class_<GuidoStream>("GuidoStream");
}

