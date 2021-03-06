/*
  GUIDO Library
  Copyright (C) 2014	Grame

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr

*/

#include <string>
#include <emscripten.h>
#include <emscripten/bind.h>

#include "GUIDO2Midi.h"
#include "GUIDOEngine.h"
#include "GUIDOEngineAdapter.h"
#include "GUIDOFactoryAdapter.h"
#include "GUIDOInternal.h"
#include "GuidoParser.h"
#include "GUIDOPianoRollAdapter.h"
#include "GUIDOReducedProportionalAdapter.h"
#include "GUIDOScoreMap.h"
#include "map2json.h"
#include "PianoRoll.h"
#include "RProportional.h"

using namespace emscripten;

/*
 * Structure and enum binding.
 * This structures can be created and manipulated in javascript side like json object and passed to C++ method.
 */
EMSCRIPTEN_BINDINGS(CStruct) {
	
	value_object<Guido2MidiParams>("Guido2MidiParams")
			.field("fTempo", &Guido2MidiParams::fTempo)
			.field("fTicks", &Guido2MidiParams::fTicks)
			.field("fChan", &Guido2MidiParams::fChan)
			.field("fIntensity", &Guido2MidiParams::fIntensity)
			.field("fAccentFactor", &Guido2MidiParams::fAccentFactor)
			.field("fMarcatoFactor", &Guido2MidiParams::fMarcatoFactor)
			.field("fDFactor", &Guido2MidiParams::fDFactor)
			.field("fStaccatoFactor", &Guido2MidiParams::fStaccatoFactor)
			.field("fSlurFactor", &Guido2MidiParams::fSlurFactor)
			.field("fTenutoFactor", &Guido2MidiParams::fTenutoFactor)
			.field("fFermataFactor", &Guido2MidiParams::fFermataFactor);
	
	value_object<GuidoLayoutSettings>("GuidoLayoutSettings")
			.field("systemsDistance", &GuidoLayoutSettings::systemsDistance)
			.field("systemsDistribution", &GuidoLayoutSettings::systemsDistribution)
			.field("systemsDistribLimit", &GuidoLayoutSettings::systemsDistribLimit)
			.field("force", &GuidoLayoutSettings::force)
			.field("spring", &GuidoLayoutSettings::spring)
			.field("neighborhoodSpacing", &GuidoLayoutSettings::neighborhoodSpacing)
			.field("optimalPageFill", &GuidoLayoutSettings::optimalPageFill)
			.field("resizePage2Music", &GuidoLayoutSettings::resizePage2Music)
			.field("proportionalRenderingForceMultiplicator", &GuidoLayoutSettings::proportionalRenderingForceMultiplicator)
			.field("checkLyricsCollisions", &GuidoLayoutSettings::checkLyricsCollisions);

	value_object<GuidoDate>("GuidoDate")
			.field("num", &GuidoDate::num)
			.field("denom", &GuidoDate::denom);

	value_object<GuidoPageFormat>("GuidoPageFormat")
			.field("width", &GuidoPageFormat::width)
			.field("height", &GuidoPageFormat::height)
			.field("marginleft", &GuidoPageFormat::marginleft)
			.field("margintop", &GuidoPageFormat::margintop)
			.field("marginright", &GuidoPageFormat::marginright)
			.field("marginbottom", &GuidoPageFormat::marginbottom);

	value_object<ParserError>("ParserError")
			.field("line", &ParserError::line)
			.field("col", &ParserError::col)
			.field("msg", &ParserError::msg);

	value_object<GuidoVersion>("GuidoVersion")
			.field("major", &GuidoVersion::major)
			.field("minor", &GuidoVersion::minor)
			.field("sub", &GuidoVersion::sub)
			.field("str", &GuidoVersion::str);

	value_object<LimitParams>("LimitParams")
			.field("startDate", &LimitParams::startDate)
			.field("endDate", &LimitParams::endDate)
			.field("lowPitch", &LimitParams::lowPitch)
			.field("highPitch", &LimitParams::highPitch);

	enum_<GuidoErrCode>("GuidoErrCode")
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

	enum_<GuidoElementSelector>("GuidoElementSelector")
			.value("kGuidoPage", GuidoElementSelector::kGuidoPage)
			.value("kGuidoSystem", GuidoElementSelector::kGuidoSystem)
			.value("kGuidoSystemSlice", GuidoElementSelector::kGuidoSystemSlice)
			.value("kGuidoStaff", GuidoElementSelector::kGuidoStaff)
			.value("kGuidoBar", GuidoElementSelector::kGuidoBar)
			.value("kGuidoEvent", GuidoElementSelector::kGuidoEvent)
			.value("kGuidoScoreElementEnd", GuidoElementSelector::kGuidoScoreElementEnd);

	enum_<PianoRollType>("PianoRollType")
			.value("kSimplePianoRoll", PianoRollType::kSimplePianoRoll)
			.value("kTrajectoryPianoRoll", PianoRollType::kTrajectoryPianoRoll);

	// Pitch constants
	constant("kCLine", kCLine);
	constant("kCSharpLine", kCSharpLine);
	constant("kDLine", kDLine);
	constant("kDSharpLine", kDSharpLine);
	constant("kELine", kELine);
	constant("kFLine", kFLine);
	constant("kFSharpLine", kFSharpLine);
	constant("kGLine", kGLine);
	constant("kGSharpLine", kGSharpLine);
	constant("kALine", kALine);
	constant("kASharpLine", kASharpLine);
	constant("kBLine", kBLine);
	constant("kAutoLines", kAutoLines);
	constant("kNoLine", kNoLine);
}

/*
 * C++ class binding.
 * This classes can be used in javascript side.
 */
EMSCRIPTEN_BINDINGS(EngineAdapter) {
    function("ar2MIDIFile", &GuidoAR2MIDIFile, allow_raw_pointers());

	// Binding C++ class adapter for guidoEngine
	class_<GuidoEngineAdapter>("GuidoEngineAdapter")
			//.smart_ptr_constructor("GuidoEngineAdapter",&std::make_shared<GuidoEngineAdapter>)
			.constructor<>()
			.function("init", 				select_overload<GuidoErrCode(void)>(&GuidoEngineAdapter::init))
			.function("shutdown", 			&GuidoEngineAdapter::shutdown)
			.function("ar2gr", 				select_overload<NodeGR*(NodeAR*)>(&GuidoEngineAdapter::ar2gr), allow_raw_pointers())
			.function("ar2grSettings", 		select_overload<NodeGR*(NodeAR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::ar2gr), allow_raw_pointers())
			.function("updateGR", 			select_overload<GuidoErrCode(NodeGR*)>(&GuidoEngineAdapter::updateGR), allow_raw_pointers())
			.function("updateGRSettings", 	select_overload<GuidoErrCode(NodeGR*, const GuidoLayoutSettings &)>(&GuidoEngineAdapter::updateGR), allow_raw_pointers())
			.function("showElement", 		&GuidoEngineAdapter::showElement, allow_raw_pointers())
			.function("freeAR", 			&GuidoEngineAdapter::freeAR, allow_raw_pointers())
			.function("freeGR", 			&GuidoEngineAdapter::freeGR, allow_raw_pointers())
			.function("getErrorString", 	&GuidoEngineAdapter::getErrorString)
			.function("getDefaultLayoutSettings", &GuidoEngineAdapter::getDefaultLayoutSettings)
			.function("countVoices", 		&GuidoEngineAdapter::countVoices, allow_raw_pointers())
			.function("getPageCount", 		&GuidoEngineAdapter::getPageCount, allow_raw_pointers())
			.function("getSystemCount", 	&GuidoEngineAdapter::getSystemCount, allow_raw_pointers())
			.function("duration", 			&GuidoEngineAdapter::duration, allow_raw_pointers())
			.function("findEventPage", 		&GuidoEngineAdapter::findEventPage, allow_raw_pointers())
			.function("findPageAt", 		&GuidoEngineAdapter::findPageAt, allow_raw_pointers())
			.function("getPageDate", 		&GuidoEngineAdapter::getPageDate, allow_raw_pointers())
			.function("gr2SVG", 			select_overload<std::string (NodeGR*, int, bool, int)>(&GuidoEngineAdapter::gr2SVG), allow_raw_pointers())
			.function("gr2SVGColored", 		&GuidoEngineAdapter::gr2SVGColored, allow_raw_pointers())
			.function("abstractExport", 	select_overload<std::string (NodeGR*, int)>(&GuidoEngineAdapter::abstractExport), allow_raw_pointers())
			.function("binaryExport", 		select_overload<std::string (NodeGR*, int)>(&GuidoEngineAdapter::binaryExport), allow_raw_pointers())
			.function("javascriptExport", 		&GuidoEngineAdapter::javascriptExport, allow_raw_pointers())
			.function("setDrawBoundingBoxes", 	&GuidoEngineAdapter::setDrawBoundingBoxes)
			.function("getDrawBoundingBoxes", 	&GuidoEngineAdapter::getDrawBoundingBoxes)
			.function("getPageFormat", 			&GuidoEngineAdapter::getPageFormat, allow_raw_pointers())
			.function("setDefaultPageFormat", 	&GuidoEngineAdapter::setDefaultPageFormat)
			.function("getDefaultPageFormat", 	&GuidoEngineAdapter::getDefaultPageFormat)
			.function("unit2CM", 			&GuidoEngineAdapter::unit2CM)
			.function("cm2Unit", 			&GuidoEngineAdapter::cm2Unit)
			.function("unit2Inches", 		&GuidoEngineAdapter::unit2Inches)
			.function("inches2Unit", 		&GuidoEngineAdapter::inches2Unit)
			.function("resizePageToMusic",	&GuidoEngineAdapter::resizePageToMusic, allow_raw_pointers())
			.function("getVersion", 		&GuidoEngineAdapter::getVersion)
			.function("getVersionStr",		&GuidoEngineAdapter::getVersionStr)
			.function("checkVersionNums", 	&GuidoEngineAdapter::checkVersionNums)
			.function("getLineSpace", 		&GuidoEngineAdapter::getLineSpace)
			.function("markVoice", 			&GuidoEngineAdapter::markVoice, allow_raw_pointers())
			.function("openParser", 		&GuidoEngineAdapter::openParser, allow_raw_pointers())
			.function("closeParser", 		&GuidoEngineAdapter::closeParser, allow_raw_pointers())
			.function("file2AR", 			&GuidoEngineAdapter::file2AR, allow_raw_pointers())
			.function("string2AR", 			&GuidoEngineAdapter::string2AR, allow_raw_pointers())
			.function("getStream", 			&GuidoEngineAdapter::getStream, allow_raw_pointers())
			.function("stream2AR", 			&GuidoEngineAdapter::stream2AR, allow_raw_pointers())
			.function("parserGetErrorCode", &GuidoEngineAdapter::parserGetErrorCode, allow_raw_pointers())
			.function("openStream", 		&GuidoEngineAdapter::openStream, allow_raw_pointers())
			.function("closeStream", 		&GuidoEngineAdapter::closeStream, allow_raw_pointers())
			.function("writeStream", 		&GuidoEngineAdapter::writeStream, allow_raw_pointers())
			.function("resetStream", 		&GuidoEngineAdapter::resetStream, allow_raw_pointers())

			.function("getParsingTime", 	&GuidoEngineAdapter::getParsingTime, allow_raw_pointers())
			.function("getAR2GRTime", 		&GuidoEngineAdapter::getAR2GRTime, allow_raw_pointers())
			.function("getOnDrawTime", 		&GuidoEngineAdapter::getOnDrawTime, allow_raw_pointers());

	// Binding C++ class Map2json to have a javascript implementation of GuidoScoreMap
	class_<Map2json>("GUIDOScoreMap")
			.constructor<>()
			.function("getPageMap", 		&Map2json::getPageMap, allow_raw_pointers())
			.function("getStaffMap", 		&Map2json::getStaffMap, allow_raw_pointers())
			.function("getVoiceMap", 		&Map2json::getVoiceMap, allow_raw_pointers())
			.function("getSystemMap", 		&Map2json::getSystemMap, allow_raw_pointers())
			.function("getTime", 			&Map2json::getTime)
			.function("getPoint", 			&Map2json::getPoint)
			.function("getTimeMap", 		&Map2json::getTimeMap, allow_raw_pointers())
			.function("getPianoRollMap", 	&Map2json::getPianoRollMap, allow_raw_pointers());

	// Binding C++ class adapter for GuidoPianoRoll
	class_<GUIDOPianoRollAdapter>("GUIDOPianoRollAdapter")
			.constructor<>()
			.function("ar2PianoRoll", &GUIDOPianoRollAdapter::ar2PianoRoll, allow_raw_pointers())
			.function("destroyPianoRoll", &GUIDOPianoRollAdapter::destroyPianoRoll, allow_raw_pointers())
			.function("setLimits", &GUIDOPianoRollAdapter::setLimits, allow_raw_pointers())
			.function("enableKeyboard", &GUIDOPianoRollAdapter::enableKeyboard, allow_raw_pointers())
			.function("getKeyboardWidth", &GUIDOPianoRollAdapter::getKeyboardWidth, allow_raw_pointers())
			.function("enableAutoVoicesColoration", &GUIDOPianoRollAdapter::enableAutoVoicesColoration, allow_raw_pointers())
			.function("setRGBColorToVoice", &GUIDOPianoRollAdapter::setRGBColorToVoice, allow_raw_pointers())
			.function("setHtmlColorToVoice", &GUIDOPianoRollAdapter::setHtmlColorToVoice, allow_raw_pointers())
			.function("setColorToVoice", &GUIDOPianoRollAdapter::setColorToVoice, allow_raw_pointers())
			.function("removeColorToVoice", &GUIDOPianoRollAdapter::removeColorToVoice, allow_raw_pointers())
			.function("enableMeasureBars", &GUIDOPianoRollAdapter::enableMeasureBars, allow_raw_pointers())
			.function("setPitchLinesDisplayMode", &GUIDOPianoRollAdapter::setPitchLinesDisplayMode, allow_raw_pointers())
			.function("svgExport", &GUIDOPianoRollAdapter::svgExport, allow_raw_pointers())
			.function("javascriptExport", &GUIDOPianoRollAdapter::javascriptExport, allow_raw_pointers());

	// Binding C++ class adapter for GUIDOReducedProportional
	class_<GUIDOReducedProportionalAdapter>("GUIDOReducedProportionalAdapter")
			.constructor<>()
			.function("ar2RProportional", &GUIDOReducedProportionalAdapter::ar2RProportional, allow_raw_pointers())
			.function("destroyRProportional", &GUIDOReducedProportionalAdapter::destroyRProportional, allow_raw_pointers())
			.function("drawDurationLines", &GUIDOReducedProportionalAdapter::drawDurationLines, allow_raw_pointers())
			.function("enableAutoVoicesColoration", &GUIDOReducedProportionalAdapter::enableAutoVoicesColoration, allow_raw_pointers())
			.function("setRGBColorToVoice", &GUIDOReducedProportionalAdapter::setRGBColorToVoice, allow_raw_pointers())
			.function("setHtmlColorToVoice", &GUIDOReducedProportionalAdapter::setHtmlColorToVoice, allow_raw_pointers())
			.function("removeColorToVoice", &GUIDOReducedProportionalAdapter::removeColorToVoice, allow_raw_pointers())
			.function("enableMeasureBars", &GUIDOReducedProportionalAdapter::enableMeasureBars, allow_raw_pointers())
			.function("setLimits", &GUIDOReducedProportionalAdapter::setLimits, allow_raw_pointers())
			.function("svgExport", &GUIDOReducedProportionalAdapter::svgExport, allow_raw_pointers())
			.function("javascriptExport", &GUIDOReducedProportionalAdapter::javascriptExport, allow_raw_pointers());

	// Binding C++ class adapter for GuidoFactory
	class_<GUIDOFactoryAdapter>("GUIDOFactoryAdapter")
			.constructor<>()
			.function("openMusic", &GUIDOFactoryAdapter::openMusic)
			.function("closeMusic", &GUIDOFactoryAdapter::closeMusic, allow_raw_pointers())
			.function("openVoice", &GUIDOFactoryAdapter::openVoice)
			.function("closeVoice", &GUIDOFactoryAdapter::closeVoice)
			.function("openChord", &GUIDOFactoryAdapter::openChord)
			.function("closeChord", &GUIDOFactoryAdapter::closeChord)
			.function("insertCommata", &GUIDOFactoryAdapter::insertCommata)
			.function("openEvent", &GUIDOFactoryAdapter::openEvent)
			.function("closeEvent", &GUIDOFactoryAdapter::closeEvent)
			.function("addSharp", &GUIDOFactoryAdapter::addSharp)
			.function("addFlat", &GUIDOFactoryAdapter::addFlat)
			.function("setEventDots", &GUIDOFactoryAdapter::setEventDots)
			.function("setEventAccidentals", &GUIDOFactoryAdapter::setEventAccidentals)
			.function("setOctave", &GUIDOFactoryAdapter::setOctave)
			.function("setDuration", &GUIDOFactoryAdapter::setDuration)
			.function("openTag", &GUIDOFactoryAdapter::openTag)
			.function("openRangeTag", &GUIDOFactoryAdapter::openRangeTag)
			.function("endTag", &GUIDOFactoryAdapter::endTag)
			.function("closeTag", &GUIDOFactoryAdapter::closeTag)
			.function("addTagParameterString", &GUIDOFactoryAdapter::addTagParameterString)
			.function("addTagParameterInt", &GUIDOFactoryAdapter::addTagParameterInt)
			.function("addTagParameterFloat", &GUIDOFactoryAdapter::addTagParameterFloat)
			.function("setParameterName", &GUIDOFactoryAdapter::setParameterName)
			.function("setParameterUnit", &GUIDOFactoryAdapter::setParameterUnit);

	// Black box object, just for passing argument pointer in method to and from javascript.
	class_<GuidoParser>("GuidoParser");
	class_<NodeAR>("NodeAR");
	class_<NodeGR>("NodeGR");
	class_<GuidoStream>("GuidoStream");
	class_<PianoRoll>("PianoRoll");
	class_<RProportional>("RProportional");
}

